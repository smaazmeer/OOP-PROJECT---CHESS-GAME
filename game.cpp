#include "Game.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

Game::Game() {
    currentTurn = WHITE;
    awaitingPromotion = false;
    gameOver = false;
    loadTextures();
    initializeBoard();

    font.loadFromFile("arial.ttf");
    promotionText.setFont(font);
    promotionText.setCharacterSize(24);
    promotionText.setFillColor(sf::Color::White);
    promotionText.setString("Promote pawn to (Q/R/B/N):");
    promotionText.setPosition(520, 50);

    gameOverText.setFont(font);
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(200, 250); // Center position
    gameOverText.setString("Game Over!");
    logFont.loadFromFile("arial.ttf");

    

 
 
}


Game::~Game() {
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            delete board[y][x];
}

void Game::loadTextures() {
    std::string names[] = { "king", "queen", "rook", "bishop", "knight", "pawn" };
    std::string colors[] = { "white", "black" };
    for (const auto& color : colors) {
        for (const auto& name : names) {
            std::string file = "images/" + color + "_" + name + ".png";
            sf::Texture tex;
            tex.loadFromFile(file);
            textureMap[color + "_" + name] = tex;
        }
    }
}

void Game::initializeBoard() {
    // Clear the board
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            board[y][x] = nullptr;

    Color colors[] = { WHITE, BLACK };
    int backRows[] = { 7, 0 };
    int pawnRows[] = { 6, 1 };

    for (int i = 0; i < 2; ++i) {
        Color c = colors[i];
        int back = backRows[i];
        int pawn = pawnRows[i];

        board[back][0] = new Rook(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_rook")]);
        board[back][1] = new Knight(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_knight")]);
        board[back][2] = new Bishop(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_bishop")]);
        board[back][3] = new Queen(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_queen")]);
        board[back][4] = new King(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_king")]);
        board[back][5] = new Bishop(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_bishop")]);
        board[back][6] = new Knight(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_knight")]);
        board[back][7] = new Rook(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_rook")]);

        for (int x = 0; x < 8; ++x)
            board[pawn][x] = new Pawn(c, textureMap[(c == WHITE ? "white" : "black") + std::string("_pawn")]);
    }
}

void Game::drawBoard(sf::RenderWindow& window) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            sf::RectangleShape square(sf::Vector2f(80, 80));
            square.setPosition(x * 80, y * 80);
            bool isLight = (x + y) % 2 == 0;
            square.setFillColor(isLight ? sf::Color(240, 217, 181) : sf::Color(181, 136, 99));
            window.draw(square);
        }
    }
}

void Game::drawPieces(sf::RenderWindow& window) {
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board[y][x];
            if (piece != nullptr) {
                // Set the position of the piece sprite
                piece->sprite.setPosition(x * 80, y * 80);
                
                // Scale the sprite to fit within the square
                piece->sprite.setScale(80.0f / piece->sprite.getTexture()->getSize().x, 
                                       80.0f / piece->sprite.getTexture()->getSize().y);
                
                // Draw the piece
                window.draw(piece->sprite);
            }
        }
    }
}


void Game::drawHighlights(sf::RenderWindow& window) {
    for (auto& pos : legalMoves) {
        sf::CircleShape highlight(15);
        highlight.setFillColor(sf::Color(0, 255, 0, 150));
        highlight.setPosition(pos.x * 80 + 32, pos.y * 80 + 32);
        window.draw(highlight);
    }
}

bool Game::handleClick(int x, int y) {
    if (awaitingPromotion || gameOver) return false;

    static bool selected = false;
    static int sx, sy;

    if (!selected) {
        Piece* piece = board[y][x];
        if (piece && piece->color == currentTurn) {
            selected = true;
            sx = x;
            sy = y;
            legalMoves.clear();

            for (int dy = 0; dy < 8; ++dy)
                for (int dx = 0; dx < 8; ++dx)
                    if (piece->isValidMove(sx, sy, dx, dy, board))
                        legalMoves.push_back({ dx, dy });
        }
    } else {
        Piece* piece = board[sy][sx];
        if (isMoveLegal(piece, sx, sy, x, y)) {
            Piece* dest = board[y][x];
            delete dest;
            board[y][x] = piece;
            board[sy][sx] = nullptr;
            piece->hasMoved = true;
            if (piece->type == KING && std::abs(x - sx) == 2) {
    int rookFromX = (x > sx) ? 7 : 0;
    int rookToX = (x > sx) ? x - 1 : x + 1;
    Piece* rook = board[sy][rookFromX];
    board[sy][rookToX] = rook;
    board[sy][rookFromX] = nullptr;
    rook->hasMoved = true;
}
      addMoveToHistory(sx, sy, x, y, piece);
            // Promotion
            if (piece->type == PAWN && (y == 0 || y == 7)) {
                awaitingPromotion = true;
                promotionPos = { x, y };
            } else {
                switchTurn();
            }

            // Checkmate
            if (isCheckmate(currentTurn)) {
                gameOver = true;
            }

            selected = false;
            legalMoves.clear();
            return true; // ? Valid move made
        }

        selected = false;
        legalMoves.clear();
    }

    return false; // ? No move made
}



bool Game::isMoveLegal(Piece* selected, int sx, int sy, int dx, int dy) {
    if (!selected) return false;
    Piece* dest = board[dy][dx];
    if (dest && dest->color == selected->color) return false;
    if (!selected->isValidMove(sx, sy, dx, dy, board)) return false;

    // --- Begin temporary simulation ---
    Piece* temp = board[dy][dx];
    board[dy][dx] = selected;
    board[sy][sx] = nullptr;

    bool inCheck = isKingInCheck(currentTurn);

    // Revert
    board[sy][sx] = selected;
    board[dy][dx] = temp;

    return !inCheck;
}
bool Game::isKingInCheck(Color c)  {
    int kingX = -1, kingY = -1;
    
    // Find the king
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (board[y][x] && board[y][x]->type == KING && board[y][x]->color == c) {
                kingX = x;
                kingY = y;
            }

    if (kingX == -1 || kingY == -1) return true; // King not found (shouldn't happen)

    // Check if any opposing piece can attack the king
    for (int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
            if (board[y][x] && board[y][x]->color != c)
                if (board[y][x]->isValidMove(x, y, kingX, kingY, board))
                    return true;

    return false;
}


void Game::switchTurn() {
    currentTurn = (currentTurn == WHITE) ? BLACK : WHITE;
    

}

bool Game::isInsideBoard(int x, int y) {
    return x >= 0 && x < 8 && y >= 0 && y < 8;
}

Piece* Game::getPiece(int x, int y) {
    return isInsideBoard(x, y) ? board[y][x] : nullptr;
}

void Game::setPiece(int x, int y, Piece* piece) {
    if (isInsideBoard(x, y)) board[y][x] = piece;
}

Piece* Game::removePiece(int x, int y) {
    Piece* p = getPiece(x, y);
    if (p) board[y][x] = nullptr;
    return p;
}

void Game::promotePawn(Color color, int x, int y, char choice) {
    Piece* old = board[y][x];
    delete old;

    switch (toupper(choice)) {
        case 'Q': board[y][x] = new Queen(color, textureMap[(color == WHITE ? "white" : "black") + std::string("_queen")]); break;
        case 'R': board[y][x] = new Rook(color, textureMap[(color == WHITE ? "white" : "black") + std::string("_rook")]); break;
        case 'B': board[y][x] = new Bishop(color, textureMap[(color == WHITE ? "white" : "black") + std::string("_bishop")]); break;
        case 'N': board[y][x] = new Knight(color, textureMap[(color == WHITE ? "white" : "black") + std::string("_knight")]); break;
        default: board[y][x] = new Queen(color, textureMap[(color == WHITE ? "white" : "black") + std::string("_queen")]);
    }

    awaitingPromotion = false;
    switchTurn();
}

bool Game::isAwaitingPromotion() const {
    return awaitingPromotion;
}

void Game::handlePromotionInput(char choice) {
    promotePawn(currentTurn, promotionPos.x, promotionPos.y, choice);
}

void Game::drawPromotion(sf::RenderWindow& window) {
    if (awaitingPromotion) {
        window.draw(promotionText);
    }
}
bool Game::isCheckmate(Color color) {
    // First, check if the king is in check
    if (!isKingInCheck(color)) return false;

    // Then, check if the player has any valid moves to get out of check
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            Piece* piece = board[y][x];
            if (piece && piece->color == color) {
                for (int dy = 0; dy < 8; ++dy) {
                    for (int dx = 0; dx < 8; ++dx) {
                        if (isMoveLegal(piece, x, y, dx, dy)) {
                            return false; // There's at least one valid move
                        }
                    }
                }
            }
        }
    }
    return true; // No valid moves and the king is in check -> checkmate
}

Color Game::getCurrentTurn() const { return currentTurn; }
void Game::setGameOver(bool over) { gameOver = over; }
bool Game::isGameOver() const { return gameOver; }
bool Game::wasLastMoveCheckmate() {
    return gameOver && !isKingInCheck(currentTurn); // gameOver triggered without check? then it's checkmate
}

std::string Game::positionToChessNotation(int x, int y) {
    char file = 'a' + x;
    char rank = '8' - y;
    return std::string(1, file) + std::string(1, rank);
}

void Game::addMoveToHistory(int sx, int sy, int dx, int dy, Piece* movedPiece) {
    std::string move = (currentTurn == WHITE ? "White: " : "Black: ");
    move += positionToChessNotation(sx, sy) + " ? " + positionToChessNotation(dx, dy);
    
    sf::Text moveText;
    moveText.setFont(logFont);
    moveText.setCharacterSize(16);
    moveText.setFillColor(sf::Color::White);
    moveText.setString(move);
    moveText.setPosition(680, 40 + moveLogTexts.size() * 20);

    moveLogTexts.push_back(moveText);
    moveHistory.push_back(move);
}

void Game::drawMoveHistory(sf::RenderWindow& window) {
    const int maxVisible = 25; // show only last 25 moves
    int totalMoves = moveLogTexts.size();
    int startIndex = totalMoves > maxVisible ? totalMoves - maxVisible : 0;

    for (int i = startIndex; i < totalMoves; ++i) {
        sf::Text text = moveLogTexts[i];
        text.setPosition(680, 40 + (i - startIndex) * 20); // reposition so it fits screen
        window.draw(text);
    }
}
int getPieceValue(PieceType type) {
    switch (type) {
        case PAWN: return 1;
        case KNIGHT:
        case BISHOP: return 3;
        case ROOK: return 5;
        case QUEEN: return 9;
        case KING: return 100;
        default: return 0;
    }
}

void Game::makeAIMove() {
    struct Move {
        int sx, sy, dx, dy;
        int score;
    };

    std::vector<Move> allMoves;

    for (int sy = 0; sy < 8; ++sy) {
        for (int sx = 0; sx < 8; ++sx) {
            Piece* piece = board[sy][sx];
            if (piece && piece->color == BLACK) {
                for (int dy = 0; dy < 8; ++dy) {
                    for (int dx = 0; dx < 8; ++dx) {
                        if (isMoveLegal(piece, sx, sy, dx, dy)) {
                            Piece* target = board[dy][dx];
                            int score = 0;

                            if (target) {
                                score = getPieceValue(target->type); // prioritize captures
                            }

                            allMoves.push_back({sx, sy, dx, dy, score});
                        }
                    }
                }
            }
        }
    }

    if (allMoves.empty()) return;

    // Pick the highest scoring move
    Move best = allMoves[0];
    for (auto& m : allMoves) {
        if (m.score > best.score)
            best = m;
    }

    // Execute best move
    Piece* piece = board[best.sy][best.sx];
    Piece* captured = board[best.dy][best.dx];
    delete captured;
    board[best.dy][best.dx] = piece;
    board[best.sy][best.sx] = nullptr;
    piece->hasMoved = true;

    addMoveToHistory(best.sx, best.sy, best.dx, best.dy, piece);

    // Promote pawns automatically
    if (piece->type == PAWN && (best.dy == 0 || best.dy == 7)) {
        board[best.dy][best.dx] = new Queen(BLACK, textureMap["black_queen"]);
    }

    if (isCheckmate(WHITE)) {
        gameOver = true;
    }

    switchTurn();
}



