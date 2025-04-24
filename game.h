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

public:
    Game();
    ~Game();
    void loadTextures();
    void drawBoard(sf::RenderWindow& window);
    void drawPieces(sf::RenderWindow& window);
    void drawHighlights(sf::RenderWindow& window);
    void drawPromotion(sf::RenderWindow& window);

    void initializeBoard();
    void handleClick(int x, int y);
    bool isMoveLegal(Piece* selected, int sx, int sy, int dx, int dy);
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
};

#endif
