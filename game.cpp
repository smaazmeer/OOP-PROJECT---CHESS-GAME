#include "Game.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>

Game::Game() {
    currentTurn = WHITE;
    awaitingPromotion = false;
    loadTextures();
    initializeBoard();

    font.loadFromFile("arial.ttf");
    promotionText.setFont(font);
    promotionText.setCharacterSize(24);
    promotionText.setFillColor(sf::Color::White);
    promotionText.setString("Promote pawn to (Q/R/B/N):");
    promotionText.setPosition(520, 50);
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

void Game::handleClick(int x, int y) {
    if (awaitingPromotion) return;

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

            // Check promotion
            if (piece->type == PAWN && (y == 0 || y == 7)) {
                awaitingPromotion = true;
                promotionPos = { x, y };
            } else {
                switchTurn();
            }
        }
        selected = false;
        legalMoves.clear();
    }
}

bool Game::isMoveLegal(Piece* selected, int sx, int sy, int dx, int dy) {
    if (!selected) return false;
    Piece* dest = board[dy][dx];
    if (dest && dest->color == selected->color) return false;
    if (!selected->isValidMove(sx, sy, dx, dy, board)) return false;
    return true;
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



