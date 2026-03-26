#include <SFML/Graphics.hpp>
#include "Functions.h"

namespace ChessGame {

    sf::Vector2f Functions::convertToPosition(int r, int c) {
        float squareSize = 100.f;
        return { c * squareSize, r * squareSize };
    }

    std::pair<int, int> Functions::convertToSquare(sf::Vector2f position) {
        float squareSize = 100.f;
        return { int(position.y / squareSize), int(position.x / squareSize) };
    }

    int Functions::convertToNumber(sf::Vector2f position) {
        auto [r, c] = convertToSquare(position);
        int num = r * 8 + c;

        return num;
    }

}