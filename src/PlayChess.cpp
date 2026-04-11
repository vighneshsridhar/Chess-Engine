#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <stack>

#include "PlayChess.h"
#include "Move.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "TextureManager.h"
#include "Functions.h"
#include "Bitboard.h"
#include "PromotionClicker.h"
#include "PGN.h"


namespace ChessGame {

    PlayChess::PlayChess() {
        boardSize = 8;
        textures = t.getTextures();
    };

    void PlayChess::draw(sf::RenderWindow& window, float squareSize, std::vector<std::vector<ChessPiece>> b, std::vector<std::vector<sf::Sprite>>& spritesBoard) const {
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

                if (b[r][c].getPieceType() != PieceType::EMPTY) {
                    sf::FloatRect bounds = spritesBoard[r][c].getLocalBounds();
                    spritesBoard[r][c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                    spritesBoard[r][c].setPosition(b[r][c].getPosition());
                    window.draw(spritesBoard[r][c]);
                }
            }
        }
    }

    bool PlayChess::makeMove(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move::MoveNode* n,
    std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move*>& legalMoves, PGN& m) const {
        Move* move = n->move;
        std::vector<std::vector<ChessPiece>> b = n->b;

        sf::Vector2f square1 = move->getInitialSquare();
        sf::Vector2f square2 = move->getEndSquare();
        auto [initial_r, initial_c] = Functions::convertToSquare(square1);
        auto [r, c] = Functions::convertToSquare(square2);

        ChessPiece initialPiece = b[initial_r][initial_c];
        ChessPiece capturedPiece = move->getCapturedPiece();

        PromotionClicker p;

        if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
            int j;

            if (initialPiece.getColor() == PieceColor::WHITE) {
                j = 0;
            }

            else {
                j = 1;
            }
            auto [i, didPromote] = p.promotePawn(window, b, initialPiece, spritesBoard, promotionSprites[j], square2);

            if (didPromote) {
                spritesBoard[r][c] = promotionSprites[j][i];
                n->promotionPiece = b[r][c];
            }

            else {
                return false;
            }
        }

        else {
            spritesBoard[r][c] = spritesBoard[initial_r][initial_c];
        }

        if (initialPiece.getPieceType() == PieceType::KING) {
            chessBoard.setKingPosition(std::make_pair(r, c));

            if (c - initial_c == 2) {
                spritesBoard[r][5] = spritesBoard[r][7];
            }

            if (c - initial_c == -2) {
                spritesBoard[r][3] = spritesBoard[r][0];
            }
        }
        chessBoard.push(n);
        legalMoves = chessBoard.getLegalMoves();
        n->check = m.checkOrCheckmate(chessBoard, legalMoves.size());
        chessBoard.setEnPassantFile(-1);

        return true;
    }

    void PlayChess::undoMove(ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move::MoveNode* n, std::vector<std::vector<sf::Sprite>> sprites) const {
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
        Move* move = n->move;
        auto [r1, c1] = Functions::convertToSquare(move->getInitialSquare());
        auto [r2, c2] = Functions::convertToSquare(move->getEndSquare());

        if (b[r2][c2].getPieceType() == PieceType::KING) {
            
            if (c2 - c1 == 2) {
                spritesBoard[r1][c2 + 1] = b[r2][c2].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
            }

            else if (c2 - c1 == -2) {
                spritesBoard[r1][c2 + 1] = b[r2][c2].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
            }
        }
        ChessPiece capturedPiece = move->getCapturedPiece();

        if (n->promotionPiece.getPieceType() == PieceType::EMPTY) {
            spritesBoard[r1][c1] = spritesBoard[r2][c2];
        }
        
        else {
            spritesBoard[r1][c1] = b[r2][c2].getColor() == PieceColor::WHITE ? sprites[0][0] : sprites[1][0];
        }

        if (move->isEnPassant()) {

            if (b[r1][c1].getColor() == PieceColor::WHITE) {
                spritesBoard[r2 + 1][c2] = sprites[1][0];
            }

            else {
                spritesBoard[r2 - 1][c2] = sprites[0][0];
            }
        }

        else if (capturedPiece.getPieceType() != PieceType::EMPTY) {
            int type = static_cast<int>(capturedPiece.getPieceType()) - 1;
            int color = static_cast<int>(capturedPiece.getColor());
            spritesBoard[r2][c2] = sprites[color][type];
        }
        chessBoard.changeTurn();
        chessBoard.setChessBoard(n->b);
    }

    void PlayChess::playGame() {
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

        ChessGame::ChessBoard chessBoard;
        ChessGame::ChessBoard startingPosition;
        startingPosition.changeTurn();
        std::vector<std::vector<ChessGame::ChessPiece>> b = chessBoard.getChessBoard();
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
        std::vector<std::vector<sf::Sprite>> sprites = { {whitePawnSprite, whiteKnightSprite, whiteBishopSprite, whiteRookSprite, whiteQueenSprite, whiteKingSprite},
            {blackPawnSprite, blackKnightSprite, blackBishopSprite, blackRookSprite, blackQueenSprite, blackKingSprite} };

        for (int c = 0; c < boardSize; c++) {
            spritesBoard[1][c] = blackPawnSprite;
            spritesBoard[6][c] = whitePawnSprite;
        }
        spritesBoard[0] = { blackRookSprite, blackKnightSprite, blackBishopSprite, blackQueenSprite, blackKingSprite, blackBishopSprite, blackKnightSprite, blackRookSprite };
        spritesBoard[7] = { whiteRookSprite, whiteKnightSprite, whiteBishopSprite, whiteQueenSprite, whiteKingSprite, whiteBishopSprite, whiteKnightSprite, whiteRookSprite };

        PromotionClicker p;
        PGN m;
        std::string pgn;

        int initial_r = 0;
        int initial_c = 0;
        int r;
        int c;
        float x;
        float y;

        std::vector<Move*> legalMoves = chessBoard.getLegalMoves();
        std::vector<Move::MoveNode*> children = {};
        size_t moveNumber = 0;
        Move::MoveNode* root = new Move::MoveNode{ nullptr, children, nullptr, moveNumber, b, "", empty };
        Move::MoveNode* orig_root = root;

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
                        r = std::round(spritePosition.y / squareSize);
                        c = std::round(spritePosition.x / squareSize);

                        if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) {
                            continue;
                        }
                        ChessPiece capturedPiece = b[r][c];
                        square1 = Functions::convertToPosition(initial_r, initial_c);
                        square2 = Functions::convertToPosition(r, c);
                        b[initial_r][initial_c].setPosition(square1);
                        Move* move = new Move(square1, square2, -1, b[initial_r][initial_c], capturedPiece);
                        auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [move](const Move* legalMove) {
                            return move->getInitialSquare() == legalMove->getInitialSquare() && move->getEndSquare() == legalMove->getEndSquare();
                        });

                        if (it != legalMoves.end()) {
                            std::vector<Move::MoveNode*> children;
                            moveNumber = chessBoard.getTurn() ? root->moveNumber + 1 : root->moveNumber;
                            Move::MoveNode* n = new Move::MoveNode{ move, children, root, moveNumber, b, "", empty };
                            bool newMove = true;

                            for (const auto node : root->children) {

                                if (*node->move == *move) {
                                    newMove = false;
                                    n = node;
                                }
                            }

                            if (makeMove(window, chessBoard, spritesBoard, n, promotionSprites, legalMoves, m)) {

                                if (newMove) {
                                    root->children.push_back(n);
                                }
                                root = n;
                                pgn = m.generatePGN(orig_root, startingPosition, 0);
                                // std::cout << "\033[2J\033[H";
                                std::cout << pgn + "\n" << std::endl;
                            }
                        } 

                        else {
                            b[initial_r][initial_c].setPosition(square1);
                        }
                    }
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && root->move != nullptr) {
                    undoMove(chessBoard, spritesBoard, root, sprites);
                    root = root->parent;
                    legalMoves = chessBoard.getLegalMoves();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && root->children.size() > 0) {
                    root = root->children[0];
                    bool _ = makeMove(window, chessBoard, spritesBoard, root, promotionSprites, legalMoves, m);
                }
                b = chessBoard.getChessBoard();

                if (isDragging) {
                    sf::Vector2f mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    x = mousePosition.x - dragOffset.x;
                    y = mousePosition.y - dragOffset.y;
                    b[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                }

                window.clear();
                draw(window, squareSize, b, spritesBoard);
                window.display();
            }
        }
    }
}