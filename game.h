#ifndef GAME_H
#define GAME_H

#include "pieces.h"
#include <SFML/Graphics.hpp>
#include <map>

class Game {
private:
    Piece* board[8][8];
    Color currentTurn;
    sf::Texture textures[12];
    std::map<std::string, sf::Texture> textureMap;
    std::vector<sf::Vector2i> legalMoves;
    sf::Font font;
    sf::Text promotionText;
    bool awaitingPromotion;
    sf::Vector2i promotionPos;
    bool gameOver;
    sf::Text gameOverText;
    std::vector<std::string> moveHistory;
    std::vector<sf::Text> moveLogTexts;
    sf::Font logFont;

    std::string positionToChessNotation(int x, int y);
    void addMoveToHistory(int sx, int sy, int dx, int dy, Piece* movedPiece);

public:
    Game();
    ~Game();
    void makeAIMove();
    void loadTextures();
    void drawMoveHistory(sf::RenderWindow& window);
    void drawBoard(sf::RenderWindow& window, bool flip = false);
void drawPieces(sf::RenderWindow& window, bool flip = false);
void drawHighlights(sf::RenderWindow& window, bool flip = false);
void reset();

    void drawPromotion(sf::RenderWindow& window);
    void drawGameOver(sf::RenderWindow& window);

    void initializeBoard();
    bool handleClick(int x, int y);
    bool isMoveLegal(Piece* selected, int sx, int sy, int dx, int dy);
    bool isKingInCheck(Color c);  // ?? removed const
    bool isPathClear(int sx, int sy, int dx, int dy);

    void switchTurn();
    bool isInsideBoard(int x, int y);
    bool isUnderCheck(Color color);

    Piece* getPiece(int x, int y);
    void setPiece(int x, int y, Piece* piece);
    Piece* removePiece(int x, int y);

    void promotePawn(Color color, int x, int y, char choice);
    bool isAwaitingPromotion() const;
    void handlePromotionInput(char choice);
    bool isCheckmate(Color color);

    Color getCurrentTurn() const;
    void setGameOver(bool over);
    bool isGameOver() const;

    bool wasLastMoveCheckmate();
};

#endif
