#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include "PlayChess.h"
#include "Move.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "TextureManager.h"
#include "Functions.h"
#include "Bitboard.h"
#include "PromotionClicker.h"
#include "PGN.h"
#include "Engine.h"


namespace ChessGame {

    PlayChess::PlayChess() {
        boardSize = 8;
        PGN m;
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
    std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move>& legalMoves, PGN& m) const {
        Move move = *n->move;
        chessBoard.push(move);
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();

        auto [initial_r, initial_c] = move.getInitialSquare();
        auto [r, c] = move.getEndSquare();

        ChessPiece initialPiece = move.getAttacker();
        ChessPiece capturedPiece = move.getCapturedPiece();
        PieceColor color = initialPiece.getColor();
        ChessPiece knight(PieceType::KNIGHT, color, r, c);
        ChessPiece bishop(PieceType::BISHOP, color, r, c);
        ChessPiece rook(PieceType::ROOK, color, r, c);
        ChessPiece queen(PieceType::QUEEN, color, r, c);
        std::vector<ChessPiece> promotionPieces = { queen, rook, bishop, knight };
        
        n->checkSymbol = move.isCheck() ? "+" : "";
        spritesBoard[r][c] = spritesBoard[initial_r][initial_c];

        if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
            int j;

            if (initialPiece.getColor() == PieceColor::WHITE) {
                j = 0;
            }

            else {
                j = 1;
            }
            auto [i, didPromote] = PromotionClicker::promotePawn(window, b, initialPiece, spritesBoard, promotionSprites[j], r, c);

            if (didPromote) {
                spritesBoard[r][c] = promotionSprites[j][i];
                n->move->setPromotionPiece(promotionPieces[i]);
            }

            else {
                return false;
            }
        }
        spritesBoard[r][c].setPosition(Functions::convertToPosition(r, c));
        b[r][c].setPosition(Functions::convertToPosition(r, c));

        if (initialPiece.getPieceType() == PieceType::KING) {

            if (c - initial_c == 2) {
                spritesBoard[r][5] = spritesBoard[r][7];
                b[r][5].setPosition(Functions::convertToPosition(r, 5));
            }

            if (c - initial_c == -2) {
                spritesBoard[r][3] = spritesBoard[r][0];
                b[r][3].setPosition(Functions::convertToPosition(r, 3));
            }
        }
        chessBoard.setChessBoard(b);
        legalMoves = chessBoard.getLegalMoves();
        chessBoard.setEnPassantFile(-1);

        return true;
    }

    void PlayChess::undoMove(ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move::MoveNode* n, std::vector<std::vector<sf::Sprite>> sprites) const {
        Move move = *n->move;
        auto [r1, c1] = move.getInitialSquare();
        auto [r2, c2] = move.getEndSquare();
        chessBoard.unmakeMove(move);
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();

        if (b[r1][c1].getPieceType() == PieceType::KING) {
            
            if (c2 - c1 == 2) {
                spritesBoard[r1][c2 + 1] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
                b[r1][c2 + 1].setPosition(Functions::convertToPosition(r1, c2 + 1));
            }

            else if (c2 - c1 == -2) {
                spritesBoard[r1][c2 - 2] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
                b[r1][c2 - 2].setPosition(Functions::convertToPosition(r1, c2 - 2));
            }
        }
        ChessPiece capturedPiece = move.getCapturedPiece();

        if (move.getPromotionPiece().getPieceType() == PieceType::EMPTY) {
            spritesBoard[r1][c1] = spritesBoard[r2][c2];
        }
        
        else {
            spritesBoard[r1][c1] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][0] : sprites[1][0];
        }
        b[r1][c1].setPosition(Functions::convertToPosition(r1, c1));

        if (move.isEnPassant()) {

            if (b[r1][c1].getColor() == PieceColor::WHITE) {
                spritesBoard[r2 + 1][c2] = sprites[1][0];
                b[r2 + 1][c2].setPosition(Functions::convertToPosition(r2 + 1, c2));
            }

            else {
                spritesBoard[r2 - 1][c2] = sprites[0][0];
                b[r2 - 1][c2].setPosition(Functions::convertToPosition(r2 - 1, c2));
            }
        }

        else if (capturedPiece.getPieceType() != PieceType::EMPTY) {
            int type = static_cast<int>(capturedPiece.getPieceType()) - 1;
            int color = static_cast<int>(capturedPiece.getColor());
            spritesBoard[r2][c2] = sprites[color][type];
            b[r2][c2].setPosition(Functions::convertToPosition(r2, c2));
        }
        chessBoard.setChessBoard(b);
    }

    void PlayChess::playGame() {
        sf::RenderWindow window(sf::VideoMode({ 800, 800 }), "Chess Board with Pieces!");
        auto windowSize = window.getSize();
        float squareSize = std::min(windowSize.x, windowSize.y) / 8.f;

        bool isDragging = false;
        bool didPromote = false;
        sf::Vector2f dragOffset;
        sf::Vector2f mousePosition;
        sf::Vector2f a;
        sf::Vector2f square1;
        sf::Vector2f square2;
        sf::Vector2f square3;

        ChessGame::ChessBoard chessBoard;
        ChessGame::ChessBoard startingPosition;
        startingPosition.changeTurn();
        std::vector<std::vector<ChessGame::ChessPiece>> b = chessBoard.getChessBoard();

        ChessGame::ChessPiece empty;
        std::unordered_map<std::string, sf::Texture> textures = TextureManager::getTextures();
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

        PGN m;
        std::string pgn;

        int initial_r = 0;
        int initial_c = 0;
        int r;
        int c;
        float x;
        float y;

        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        std::vector<Move::MoveNode*> children = {};
        size_t moveNumber = 0;
        Move::MoveNode* root = new Move::MoveNode{ nullptr, children, nullptr, moveNumber, b, {}, "" };
        Move::MoveNode* orig_root = root;

        int depth = 3;
        Engine e(depth);
        Move engineMove;
        bool makeEngineMove = false;

        while (window.isOpen()) {

            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (makeEngineMove) {
                    engineMove = e.iterative_deepening(chessBoard);
                    moveNumber = chessBoard.whiteTurn() ? moveNumber + 1 : moveNumber;
                    pgn = m.convertMoveToPGN(&engineMove, moveNumber, chessBoard, legalMoves);
                    std::cout << "Engine Move: " << pgn << std::endl;
                    makeEngineMove = false;
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

                        if (b[initial_r][initial_c].getPieceType() == PieceType::PAWN && std::abs(c - initial_c) > 0 && b[r][c].getPieceType() == PieceType::EMPTY) {
                            capturedPiece = b[initial_r][c];
                        }
                        b[initial_r][initial_c].setCoordinates(initial_r, initial_c);
                        Move* move = new Move(initial_r, initial_c, r, c, b[initial_r][initial_c], capturedPiece);
                        auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [move](const Move legalMove) {
                            return move->getInitialSquare() == legalMove.getInitialSquare() && move->getEndSquare() == legalMove.getEndSquare();
                        });

                        if (it != legalMoves.end()) {
                            std::vector<Move::MoveNode*> children;
                            moveNumber = chessBoard.whiteTurn() ? root->moveNumber + 1 : root->moveNumber;
                            Move::MoveNode* n = new Move::MoveNode{ move, children, root, moveNumber, b, legalMoves, "" };

                            if (makeMove(window, chessBoard, spritesBoard, n, promotionSprites, legalMoves, m)) {
                                bool newMove = true;

                                for (const auto node : root->children) {

                                    if (node->move->getInitialSquare() == move->getInitialSquare() && node->move->getEndSquare() == move->getEndSquare() && 
                                        node->move->getPromotionPiece().getPieceType() == move->getPromotionPiece().getPieceType()) {
                                        newMove = false;
                                        n = node;
                                    }
                                }

                                if (newMove) {
                                    root->children.push_back(n);
                                }
                                root = n;
                                pgn = m.generatePGN(orig_root, startingPosition, 0);
                                // std::cout << "\033[2J\033[H";
                                std::cout << pgn + "\n" << std::endl;
                                makeEngineMove = true;
                            }
                        } 

                        else {
                            b[initial_r][initial_c].setCoordinates(initial_r, initial_c);
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