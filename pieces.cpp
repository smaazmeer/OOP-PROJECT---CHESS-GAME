#include "pieces.h"
#include <cmath>
#include <iostream>

Piece::Piece(PieceType t, Color c, const sf::Texture& texture)
    : type(t), color(c), hasMoved(false) {
    sprite.setTexture(texture);
}

std::string Piece::pieceToString(PieceType t, Color c) {
    std::string result = (c == WHITE ? "White " : "Black ");
    switch (t) {
        case KING: return result + "King";
        case QUEEN: return result + "Queen";
        case ROOK: return result + "Rook";
        case BISHOP: return result + "Bishop";
        case KNIGHT: return result + "Knight";
        case PAWN: return result + "Pawn";
    }
    return "Unknown";
}

// --- King ---
King::King(Color c, const sf::Texture& tex) : Piece(KING, c, tex) {}

bool King::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    return (dxAbs <= 1 && dyAbs <= 1);
}

// --- Queen ---
Queen::Queen(Color c, const sf::Texture& tex) : Piece(QUEEN, c, tex) {}

bool Queen::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    if (sx == dx || sy == dy || dxAbs == dyAbs) {
        // Check if path is clear
        int stepX = (dx - sx == 0) ? 0 : (dx - sx) / dxAbs;
        int stepY = (dy - sy == 0) ? 0 : (dy - sy) / dyAbs;
        int x = sx + stepX, y = sy + stepY;
        while (x != dx || y != dy) {
            if (board[y][x] != nullptr) return false;
            x += stepX;
            y += stepY;
        }
        return true;
    }
    return false;
}

// --- Rook ---
Rook::Rook(Color c, const sf::Texture& tex) : Piece(ROOK, c, tex) {}

bool Rook::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    if (sx != dx && sy != dy) return false;
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    int stepX = (dx - sx == 0) ? 0 : (dx - sx) / dxAbs;
    int stepY = (dy - sy == 0) ? 0 : (dy - sy) / dyAbs;
    int x = sx + stepX, y = sy + stepY;
    while (x != dx || y != dy) {
        if (board[y][x] != nullptr) return false;
        x += stepX;
        y += stepY;
    }
    return true;
}

// --- Bishop ---
Bishop::Bishop(Color c, const sf::Texture& tex) : Piece(BISHOP, c, tex) {}

bool Bishop::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    if (dxAbs != dyAbs) return false;
    int stepX = (dx - sx) / dxAbs;
    int stepY = (dy - sy) / dyAbs;
    int x = sx + stepX, y = sy + stepY;
    while (x != dx || y != dy) {
        if (board[y][x] != nullptr) return false;
        x += stepX;
        y += stepY;
    }
    return true;
}

// --- Knight ---
Knight::Knight(Color c, const sf::Texture& tex) : Piece(KNIGHT, c, tex) {}

bool Knight::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    int dxAbs = std::abs(dx - sx);
    int dyAbs = std::abs(dy - sy);
    return (dxAbs == 2 && dyAbs == 1) || (dxAbs == 1 && dyAbs == 2);
}

// --- Pawn ---
Pawn::Pawn(Color c, const sf::Texture& tex) : Piece(PAWN, c, tex) {}

bool Pawn::isValidMove(int sx, int sy, int dx, int dy, Piece* board[8][8]) {
    int dir = (color == WHITE) ? -1 : 1;
    int startRow = (color == WHITE) ? 6 : 1;
    Piece* dest = board[dy][dx];

    // Standard 1-step forward
    if (sx == dx && dy - sy == dir && dest == nullptr)
        return true;

    // First move 2-step
    if (sx == dx && sy == startRow && dy - sy == 2 * dir &&
        board[sy + dir][sx] == nullptr && dest == nullptr)
        return true;

    // Captures
    if (std::abs(dx - sx) == 1 && dy - sy == dir && dest != nullptr && dest->color != color)
        return true;

    // En passant would be handled in Game.cpp

    return false;
}
