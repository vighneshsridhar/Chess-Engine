#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <stack>

#include "Play.h"
#include "Move.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "TextureManager.h"
#include "Functions.h"
#include "Bitboard.h"
#include "PromotionClicker.h"
#include "PGN.h"


namespace ChessGame {

    Play::Play() {
        boardSize = 8;
        textures = t.getTextures();
    };

    void Play::draw(sf::RenderWindow& window, float squareSize, std::vector<std::vector<ChessPiece>> chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard) const {
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
    }

    bool Play::makeMove(sf::RenderWindow& window, ChessBoard& b, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move move,
        std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move>& legalMoves) const {
        std::vector<std::vector<ChessPiece>> chessBoard = b.getChessBoard();

        sf::Vector2f square1 = move.getInitialSquare();
        sf::Vector2f square2 = move.getEndSquare();
        auto [initial_r, initial_c] = Functions::convertToSquare(square1);
        auto [r, c] = Functions::convertToSquare(square2);

        ChessPiece initialPiece = chessBoard[initial_r][initial_c];
        ChessPiece capturedPiece = move.getCapturedPiece();
        ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, sf::Vector2(20.f, 20.f));

        PromotionClicker p;
        bool isCapture = (capturedPiece.getPieceType() != PieceType::EMPTY);
        std::string pgnMove = m.convertMoveToPGN(move, isCapture, b);

        if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
            int j;

            if (initialPiece.getColor() == PieceColor::WHITE) {
                j = 0;
            }

            else {
                j = 1;
            }
            chessBoard[initial_r][initial_c] = empty;
            auto [i, didPromote] = p.promotePawn(window, chessBoard, initialPiece, spritesBoard, promotionSprites[j], square2);

            if (didPromote) {
                spritesBoard[r][c] = promotionSprites[j][i];
            }

            else {
                chessBoard[initial_r][initial_c] = initialPiece;
                chessBoard[initial_r][initial_c].setPosition(square1);
                return false;
            }
        }

        else {
            chessBoard[r][c] = chessBoard[initial_r][initial_c];
            chessBoard[r][c].setPosition(square2);
            spritesBoard[r][c] = spritesBoard[initial_r][initial_c];
        }

        if (initialPiece.getPieceType() == PieceType::PAWN) {

            if (std::abs(r - initial_r) == 2) {
                b.setEnPassantFile(c);
            }

            if (initial_c != c && capturedPiece.getPieceType() == PieceType::EMPTY) {
                int file = initialPiece.getColor() == PieceColor::WHITE ? r + 1 : r - 1;
                chessBoard[file][c] = empty;
                isCapture = true;
            }
        }

        if (initialPiece.getPieceType() == PieceType::KING) {
            b.setKingPosition(std::make_pair(r, c));

            if (c - initial_c == 2) {
                auto square3 = Functions::convertToPosition(r, 5);
                chessBoard[r][5] = chessBoard[r][7];
                chessBoard[r][5].setPosition(square3);
                chessBoard[r][7] = empty;
                spritesBoard[r][5] = spritesBoard[r][7];
            }

            if (c - initial_c == -2) {
                auto square3 = Functions::convertToPosition(r, 3);
                chessBoard[r][3] = chessBoard[r][0];
                chessBoard[r][3].setPosition(square3);
                chessBoard[r][0] = empty;
                spritesBoard[r][3] = spritesBoard[r][0];
            }
        }
        chessBoard[initial_r][initial_c] = empty;
        chessBoard[initial_r][initial_c].setPosition(square1);
        b.changeTurn();
        b.setChessBoard(chessBoard);
        legalMoves = b.getLegalMoves();
        m.printMove(pgnMove, b, legalMoves.size());
        isCapture = capturedPiece.getPieceType() != PieceType::EMPTY || isCapture;
        b.setEnPassantFile(-1);

        return true;
    }

    void Play::undoMove(ChessBoard& b, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move move, std::vector<std::vector<sf::Sprite>> promotionSprites, 
        std::vector<sf::Sprite> pawnSprites) const {
        std::vector<std::vector<ChessPiece>> chessBoard = b.getChessBoard();
        auto [r1, c1] = Functions::convertToSquare(move.getInitialSquare());
        auto [r2, c2] = Functions::convertToSquare(move.getEndSquare());

        if (chessBoard[r2][c2].getPieceType() == PieceType::KING) {

            if (c2 - c1 == 2) {
                chessBoard[r1][c2 + 1] = chessBoard[r1][c2 - 1];
            }

            else if (c2 - c1 == -2) {
                chessBoard[r1][c2 - 2] = chessBoard[r1][c2 + 1];
            }
        }
        ChessPiece capturedPiece = move.getCapturedPiece();
        chessBoard[r1][c1] = chessBoard[r2][c2];
        chessBoard[r1][c1].setPosition(move.getInitialSquare());
        chessBoard[r2][c2] = capturedPiece;
        int color = static_cast<int>(capturedPiece.getColor());
        int type = 5 - static_cast<int>(capturedPiece.getPieceType());

        if (type == 4) {

            if (color == 0) {
                spritesBoard[r1][c1] = pawnSprites[0];
            }

            else {
                spritesBoard[r1][c1] = pawnSprites[1];
            }
        }

        else if (type < 4) {
            spritesBoard[r1][c1] = promotionSprites[color][type];
        }
        b.changeTurn();
        b.setChessBoard(chessBoard);
    }

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

        ChessGame::ChessBoard b;
        std::vector<std::vector<ChessGame::ChessPiece>> chessBoard = b.getChessBoard();
        TextureManager t;

        ChessGame::ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, sf::Vector2(20.f, 20.f));
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

        std::vector<std::vector<sf::Sprite>> spritesBoard(boardSize, std::vector<sf::Sprite>(boardSize, whitePawnSprite));
        std::vector<std::vector<sf::Sprite>> promotionSprites = { { whiteQueenSprite, whiteRookSprite, whiteBishopSprite, whiteKnightSprite },
            { blackQueenSprite, blackRookSprite, blackBishopSprite, blackKnightSprite } };
        std::vector<sf::Sprite> pawnSprites = { whitePawnSprite, blackPawnSprite };

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

        std::vector<Move> legalMoves = b.getLegalMoves();
        std::string pgnMove;
        bool isCapture = false;
        std::stack<Move> previousMoves;
        std::stack<Move> nextMoves;
        std::stack<Move> q;
        int test = 0;

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

                        ChessPiece capturedPiece = chessBoard[r][c];
                        square1 = Functions::convertToPosition(initial_r, initial_c);
                        square2 = Functions::convertToPosition(r, c);
                        Move move = { square1, square2, -1, initialPiece, capturedPiece };

                        if (std::find(legalMoves.begin(), legalMoves.end(), move) != legalMoves.end()) {
                            
                            if (makeMove(window, b, spritesBoard, move, promotionSprites, legalMoves)) {
                                previousMoves.push(move);

                                if (!b.getTurn()) {
                                    m.incrementMove();
                                }
                            }
                        } 

                        else {
                            chessBoard[initial_r][initial_c].setPosition(square1);
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && previousMoves.size() > 0) {
                    undoMove(b, spritesBoard, previousMoves.top(), promotionSprites, pawnSprites);
                    nextMoves.push(previousMoves.top());
                    previousMoves.pop();
                    m.decrementMove();
                    legalMoves = b.getLegalMoves();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && nextMoves.size() > 0) {
                    bool _ = makeMove(window, b, spritesBoard, nextMoves.top(), promotionSprites, legalMoves);
                    previousMoves.push(nextMoves.top());
                    nextMoves.pop();
                    m.incrementMove();
                }
                chessBoard = b.getChessBoard();

                if (isDragging) {
                    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    x = mousePosition.x - dragOffset.x;
                    y = mousePosition.y - dragOffset.y;
                    chessBoard[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                    spritesBoard[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                }

                window.clear();
                draw(window, squareSize, chessBoard, spritesBoard);
                window.display();
            }
        }
    }
}