#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <filesystem> // Requires C++17 or later
#include <windows.h>
#include <vector>

#include "Play.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include "Bitboard.h"
#include "PromotionClicker.h"
#include <bitset>


namespace ChessGame {

    Play::Play() {
        boardSize = 8;
    };

    void Play::playGame() {
        sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Chess Board with Pieces!");
        auto windowSize = window.getSize();
        float squareSize = std::min(windowSize.x, windowSize.y) / 8.f;

        bool isDragging = false;
        bool didPromote = false;
        sf::Vector2f dragOffset;
        sf::Vector2f mousePosition;
        sf::Vector2f a = { 20.f, 20.f };
        sf::Vector2f square1;
        sf::Vector2f square2;
        sf::Vector2f square3;
        std::unordered_map<std::string, sf::Texture> textures;

        ChessGame::ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, a);
        ChessGame::ChessPiece whitePawn(PieceType::PAWN, PieceColor::WHITE, a);
        ChessGame::ChessPiece whiteKnight(PieceType::KNIGHT, PieceColor::WHITE, a);
        ChessGame::ChessPiece whiteBishop(PieceType::BISHOP, PieceColor::WHITE, a);
        ChessGame::ChessPiece whiteRook(PieceType::ROOK, PieceColor::WHITE, a);
        ChessGame::ChessPiece whiteQueen(PieceType::QUEEN, PieceColor::WHITE, a);
        ChessGame::ChessPiece whiteKing(PieceType::KING, PieceColor::WHITE, a);

        ChessGame::ChessPiece blackPawn(PieceType::PAWN, PieceColor::BLACK, a);
        ChessGame::ChessPiece blackKnight(PieceType::KNIGHT, PieceColor::BLACK, a);
        ChessGame::ChessPiece blackBishop(PieceType::BISHOP, PieceColor::BLACK, a);
        ChessGame::ChessPiece blackRook(PieceType::ROOK, PieceColor::BLACK, a);
        ChessGame::ChessPiece blackQueen(PieceType::QUEEN, PieceColor::BLACK, a);
        ChessGame::ChessPiece blackKing(PieceType::KING, PieceColor::BLACK, a);

        textures["WHITE_PAWN"] = whitePawn.getTexture();
        textures["WHITE_KNIGHT"] = whiteKnight.getTexture();
        textures["WHITE_BISHOP"] = whiteBishop.getTexture();
        textures["WHITE_ROOK"] = whiteRook.getTexture();
        textures["WHITE_QUEEN"] = whiteQueen.getTexture();
        textures["WHITE_KING"] = whiteKing.getTexture();

        textures["BLACK_PAWN"] = blackPawn.getTexture();
        textures["BLACK_KNIGHT"] = blackKnight.getTexture();
        textures["BLACK_BISHOP"] = blackBishop.getTexture();
        textures["BLACK_ROOK"] = blackRook.getTexture();
        textures["BLACK_QUEEN"] = blackQueen.getTexture();
        textures["BLACK_KING"] = blackKing.getTexture();

        sf::Sprite whitePawnSprite(textures["WHITE_PAWN"]);
        sf::Sprite whiteKnightSprite(textures["WHITE_KNIGHT"]);
        sf::Sprite whiteBishopSprite(textures["WHITE_BISHOP"]);
        sf::Sprite whiteRookSprite(textures["WHITE_ROOK"]);
        sf::Sprite whiteQueenSprite(textures["WHITE_QUEEN"]);
        sf::Sprite whiteKingSprite(textures["WHITE_KING"]);

        sf::Sprite blackPawnSprite(textures["BLACK_PAWN"]);
        sf::Sprite blackKnightSprite(textures["BLACK_KNIGHT"]);
        sf::Sprite blackBishopSprite(textures["BLACK_BISHOP"]);
        sf::Sprite blackRookSprite(textures["BLACK_ROOK"]);
        sf::Sprite blackQueenSprite(textures["BLACK_QUEEN"]);
        sf::Sprite blackKingSprite(textures["BLACK_KING"]);

        ChessGame::ChessBoard b;
        std::vector<std::vector<ChessGame::ChessPiece>> chessBoard = b.getChessBoard();
        sf::Sprite transparentSprite(textures["WHITE_PAWN"]);
        transparentSprite.setColor({ 0, 255, 0, 255 });
        std::vector<std::vector<sf::Sprite>> spritesBoard(boardSize, std::vector<sf::Sprite>(boardSize, transparentSprite));
        std::vector<std::vector<sf::Sprite>> promotionSprites = { { whiteQueenSprite, whiteRookSprite, whiteBishopSprite, whiteKnightSprite },
            { blackQueenSprite, blackRookSprite, blackBishopSprite, blackKnightSprite } };

        for (int c = 0; c < boardSize; c++) {
            spritesBoard[1][c] = blackPawnSprite;
            spritesBoard[6][c] = whitePawnSprite;
        }
        spritesBoard[0] = { blackRookSprite, blackKnightSprite, blackBishopSprite, blackQueenSprite, blackKingSprite, blackBishopSprite, blackKnightSprite, blackRookSprite };
        spritesBoard[7] = { whiteRookSprite, whiteKnightSprite, whiteBishopSprite, whiteQueenSprite, whiteKingSprite, whiteBishopSprite, whiteKnightSprite, whiteRookSprite };

        PromotionClicker p;

        int initial_r = 0;
        int initial_c = 0;
        int r;
        int c;
        float x;
        float y;
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves = b.getLegalMoves();

        while (window.isOpen()) {

            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (event->is<sf::Event::MouseButtonPressed>()) {

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        initial_r = int(mousePosition.y / squareSize);
                        initial_c = int(mousePosition.x / squareSize);
                        x = squareSize * initial_c;
                        y = squareSize * initial_r;
                        isDragging = true;
                        dragOffset.x = mousePosition.x - spritesBoard[initial_r][initial_c].getPosition().x;
                        dragOffset.y = mousePosition.y - spritesBoard[initial_r][initial_c].getPosition().y;
                    }
                }

                if (event->is<sf::Event::MouseButtonReleased>()) {

                    if (isDragging) {
                        isDragging = false;
                        didPromote = false;
                        sf::Sprite initialSprite = spritesBoard[initial_r][initial_c];
                        sf::Vector2f spritePosition = initialSprite.getPosition();
                        ChessPiece initialPiece = chessBoard[initial_r][initial_c];
                        r = std::round(spritePosition.y / squareSize);
                        c = std::round(spritePosition.x / squareSize);
                        square1 = Functions::convertToPosition(initial_r, initial_c);
                        square2 = Functions::convertToPosition(r, c);
                        std::pair<sf::Vector2f, sf::Vector2f> move = std::make_pair(square1, square2);

                        if (std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end()) {

                            if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
                                int j;

                                if (initialPiece.getColor() == PieceColor::WHITE) {
                                    j = 0;
                                }

                                else {
                                    j = 1;
                                }
                                chessBoard[initial_r][initial_c] = empty;
                                auto [i, didPromote] = p.promotePawn(window, chessBoard, initialPiece, spritesBoard, promotionSprites[j], 
                                    square2);

                                if (didPromote) {
                                    spritesBoard[r][c] = promotionSprites[j][i];
                                    spritesBoard[r][c].setPosition(square2);
                                }

                                else {
                                    chessBoard[initial_r][initial_c] = initialPiece;
                                    chessBoard[initial_r][initial_c].setPosition(square1);
                                    continue;
                                }
                            }

                            else {
                                chessBoard[r][c] = chessBoard[initial_r][initial_c];
                                chessBoard[r][c].setPosition(square2);
                                spritesBoard[r][c] = spritesBoard[initial_r][initial_c];
                                spritesBoard[r][c].setPosition(square2);
                            }
                            
                            
                            if (initialPiece.getPieceType() == PieceType::KING) {
                                b.setKingPosition(std::make_pair(r, c));

                                if (c - initial_c == 2) {
                                    square3 = Functions::convertToPosition(r, 5);
                                    chessBoard[r][5] = chessBoard[r][7];
                                    chessBoard[r][5].setPosition(square3);
                                    chessBoard[r][7] = empty;
                                    spritesBoard[r][5] = spritesBoard[r][7];
                                    spritesBoard[r][5].setPosition(square3);
                                    spritesBoard[r][7] = transparentSprite;
                                }

                                if (c - initial_c == -2) {
                                    square3 = Functions::convertToPosition(r, 3);
                                    chessBoard[r][3] = chessBoard[r][0];
                                    chessBoard[r][3].setPosition(square3);
                                    chessBoard[r][0] = empty;
                                    spritesBoard[r][3] = spritesBoard[r][0];
                                    spritesBoard[r][3].setPosition(square3);
                                    spritesBoard[r][0] = transparentSprite;
                                }
                            }
                            chessBoard[initial_r][initial_c] = empty;
                            chessBoard[initial_r][initial_c].setPosition(square1);
                            spritesBoard[initial_r][initial_c] = transparentSprite;
                            b.changeTurn();
                            b.setChessBoard(chessBoard);
                            legalMoves = b.getLegalMoves();
                            // std::cout << "legal moves size = " << legalMoves.size() << std::endl;

                            if (legalMoves.size() == 0) {
                                
                                if (b.isCheckmate(legalMoves)) {
                                    std::string statement = b.getTurn() ? "Black wins!" : "White wins!";
                                    std::cout << "Checkmate! " << statement << std::endl;
                                }

                                else {
                                    std::cout << "Stalemate!" << std::endl;
                                }
                            }
                        }

                        else {
                            chessBoard[initial_r][initial_c].setPosition(square1);
                            spritesBoard[initial_r][initial_c].setPosition(square1);
                        }
                    }
                }

                if (isDragging) {
                    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    x = mousePosition.x - dragOffset.x;
                    y = mousePosition.y - dragOffset.y;
                    chessBoard[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                    spritesBoard[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
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

                for (int r = 0; r < boardSize; r++) {

                    for (int c = 0; c < boardSize; c++) {

                        if (chessBoard[r][c].getPieceType() != PieceType::EMPTY) {
                            sf::FloatRect bounds = spritesBoard[r][c].getLocalBounds();
                            spritesBoard[r][c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                            spritesBoard[r][c].setPosition(chessBoard[r][c].getPosition());
                            window.draw(spritesBoard[r][c]);
                        }
                    }
                }
                window.display();
            }
        }
    }
}