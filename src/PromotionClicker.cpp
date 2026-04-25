#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include "PromotionClicker.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Functions.h"
#include "Move.h"



namespace ChessGame {

    PromotionClicker::PromotionClicker() {
    };

    std::pair<int, bool> PromotionClicker::promotePawn(sf::RenderWindow& window, std::vector<std::vector<ChessPiece>>& b,
    ChessPiece pawn, std::vector<std::vector<sf::Sprite>>& spritesBoard, std::vector<sf::Sprite>& sprites, int r, int c) {

        sf::Vector2f mousePosition;
        int boardSize = 8;
        auto windowSize = window.getSize();
        float squareSize = std::min(windowSize.x, windowSize.y) / 8.f;
        ChessPiece old = b[r][c];
        b[r][c] = pawn;
        b[r][c].setCoordinates(r, c);

        ChessPiece promotionPiece(PieceType::QUEEN, pawn.getColor(), r, c);
        sf::Vector2f promotionPosition = Functions::convertToPosition(r, c);

        PieceColor color = pawn.getColor();
        sf::FloatRect bounds;

        int i;
        int increment;

        if (color == PieceColor::WHITE) {
            i = 0;
            increment = 1;
        }

        else {
            i = 7;
            increment = -1;
        }
        sprites[0].setPosition(promotionPosition);
        i += increment;
        sf::Vector2f piecePosition = Functions::convertToPosition(i, c);

        sprites[1].setPosition(piecePosition);
        i += increment;
        piecePosition = Functions::convertToPosition(i, c);
        sprites[2].setPosition(piecePosition);
        i += increment;
        piecePosition = Functions::convertToPosition(i, c);
        sprites[3].setPosition(piecePosition);

        while (window.isOpen()) {

            while (const std::optional event = window.pollEvent()) {

                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (event->is<sf::Event::MouseButtonPressed>()) {

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    }
                }

                if (event->is<sf::Event::MouseButtonReleased>()) {
                    auto [x, y] = Functions::convertToSquare(mousePosition);

                    if (y == c && std::abs(r - x) <= 3) {

                        if (std::abs(r - x) == 1) {
                            promotionPiece.setPieceType(PieceType::ROOK);
                        }

                        if (std::abs(r - x) == 2) {
                            promotionPiece.setPieceType(PieceType::BISHOP);
                        }

                        if (std::abs(r - x) == 3) {
                            promotionPiece.setPieceType(PieceType::KNIGHT);
                        }
                        b[r][c] = promotionPiece;
                        b[r][c].setCoordinates(r, c);

                        return std::make_pair(std::abs(r - x), true);
                    }

                    else {
                        b[r][c] = old;
                        return std::make_pair(-1, false);
                    }
                }
                window.clear();

                for (int r = 0; r < boardSize; r++) {

                    for (int c = 0; c < boardSize; c++) {
                        sf::RectangleShape square({ squareSize, squareSize });
                        square.setPosition(sf::Vector2f(squareSize * c, squareSize * r));

                        if ((r + c) % 2 == 0) {
                            square.setFillColor(sf::Color(153, 204, 255));
                        }

                        else {
                            square.setFillColor(sf::Color(0, 0, 255));
                        }
                        window.draw(square);
                    }
                }
                sf::Vector2f position;

                for (int r = 0; r < boardSize; r++) {

                    for (int c = 0; c < boardSize; c++) {

                        if (b[r][c].getPieceType() != PieceType::EMPTY) {
                            bounds = spritesBoard[r][c].getLocalBounds();
                            position = Functions::convertToPosition(r, c);
                            spritesBoard[r][c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                            spritesBoard[r][c].setPosition(position);
                            window.draw(spritesBoard[r][c]);
                        }
                    }
                }
                sf::RectangleShape promotion({ squareSize, squareSize * 4 });
                promotion.setPosition(promotionPosition);
                window.draw(promotion);

                for (auto& promotionPiece : sprites) {
                    bounds = promotionPiece.getLocalBounds();
                    promotionPiece.setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                    window.draw(promotionPiece);
                }

                window.display();
            }
        }

        return std::make_pair(-1, false);
    }
}