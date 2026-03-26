#pragma once
#ifndef ChessGame_Functions_h
#define ChessGame_Functions_h

#include <SFML/Graphics.hpp>

namespace ChessGame {
    class Functions {
    public:
        static sf::Vector2f convertToPosition(int r, int c);
        static std::pair<int, int> convertToSquare(sf::Vector2f position);
        static int convertToNumber(sf::Vector2f position);
    };
}

#endif