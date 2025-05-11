#ifndef PIECE_H
#define PIECE_H

#include <SFML/Graphics.hpp>
#include <string>

enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN };
enum Color { WHITE, BLACK };

class Piece {
public:
    PieceType type;
    Color color;
    sf::Sprite sprite;
    bool hasMoved;

    Piece(PieceType t, Color c, const sf::Texture& texture);
    virtual ~Piece() {}

    // Marked const pointer for compatibility with const Game methods
    virtual bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) = 0;

    static std::string pieceToString(PieceType t, Color c);
};

// ----- Subclasses -----

class King : public Piece {
public:
    King(Color c, const sf::Texture& tex);
   bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

class Queen : public Piece {
public:
    Queen(Color c, const sf::Texture& tex);
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

class Rook : public Piece {
public:
    Rook(Color c, const sf::Texture& tex);
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

class Bishop : public Piece {
public:
    Bishop(Color c, const sf::Texture& tex);
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

class Knight : public Piece {
public:
    Knight(Color c, const sf::Texture& tex);
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

class Pawn : public Piece {
public:
    Pawn(Color c, const sf::Texture& tex);
    bool isValidMove(int sx, int sy, int dx, int dy, Piece* const board[8][8]) override;
};

#endif
