#pragma once
#ifndef ChessGame_Functions_h
#define ChessGame_Functions_h

#include <SFML/Graphics.hpp>
#include "ChessBoard.h"
#include "PromotionClicker.h"

namespace ChessGame {
    class Functions {
    public:
        static sf::Vector2f convertToPosition(int r, int c);
        static std::pair<int, int> convertToSquare(sf::Vector2f position);
        static int convertToNumber(sf::Vector2f position);
        static bool inBounds(float a, float b, float x1, float y1, float x2, float y2);
        static bool push(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, MoveNode* n, bool rightArrow,
            std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move>& legalMoves);
        static void undoMove(ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, MoveNode* n, std::vector<std::vector<sf::Sprite>> sprites);
    };
}

#endif