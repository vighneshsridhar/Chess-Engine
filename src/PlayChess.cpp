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
    };

    void PlayChess::draw(sf::RenderWindow& window, Button& playButton, bool playMode, Button& changePositionButton, bool changePositionMode, 
        Button& engineButton, bool engineMode, float squareSize, std::vector<std::vector<ChessPiece>> b, std::vector<std::vector<sf::Sprite>>& spritesBoard, 
        int initial_r, int initial_c) const {
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
        sf::RectangleShape p = playButton.getButton();
        sf::RectangleShape e = engineButton.getButton();
        sf::RectangleShape cp = changePositionButton.getButton();

        sf::FloatRect globalBounds = p.getGlobalBounds();
        sf::FloatRect localBounds = p.getLocalBounds();
        
        const sf::Font font("Assets/arial.ttf");
        sf::Text playText(font, "Play");
        float offset = playMode ? 10.f : 0;
        playText.setOrigin(sf::Vector2f(localBounds.position.x + localBounds.size.x, localBounds.position.y + localBounds.size.y));
        playText.setPosition(sf::Vector2f(globalBounds.position.x + globalBounds.size.x, globalBounds.position.y + globalBounds.size.y + offset));
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::Black);

        globalBounds = e.getGlobalBounds();
        localBounds = e.getLocalBounds();
        sf::Text engineText(font, "Analyze");
        offset = engineMode ? 10.f : 0;
        engineText.setOrigin(sf::Vector2f(localBounds.position.x + localBounds.size.x, localBounds.position.y + localBounds.size.y));
        engineText.setPosition(sf::Vector2f(globalBounds.position.x + globalBounds.size.x, globalBounds.position.y + globalBounds.size.y + offset));
        engineText.setCharacterSize(30);
        engineText.setFillColor(sf::Color::Black);

        globalBounds = cp.getGlobalBounds();
        localBounds = cp.getLocalBounds();
        sf::Text changePositionText(font, "Change Position");
        offset = changePositionMode ? 10.f : 0;
        changePositionText.setOrigin(sf::Vector2f(localBounds.position.x + localBounds.size.x, localBounds.position.y + localBounds.size.y));
        changePositionText.setPosition(sf::Vector2f(globalBounds.position.x + globalBounds.size.x, globalBounds.position.y + globalBounds.size.y + offset));
        changePositionText.setCharacterSize(30);
        changePositionText.setFillColor(sf::Color::Black);

        window.draw(p);
        window.draw(playText);
        window.draw(e);
        window.draw(engineText);
        window.draw(cp);
        window.draw(changePositionText);

        for (int r = 0; r < boardSize; r++) {

            for (int c = 0; c < boardSize; c++) {

                if (b[r][c].getPieceType() != PieceType::EMPTY && (r != initial_r || c != initial_c)) {
                    sf::FloatRect bounds = spritesBoard[r][c].getLocalBounds();
                    spritesBoard[r][c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                    spritesBoard[r][c].setPosition(Functions::convertToPosition(r, c));
                    window.draw(spritesBoard[r][c]);
                }

                else if (r == initial_r && c == initial_c && b[initial_r][initial_c].getPieceType() != PieceType::EMPTY) {
                    sf::FloatRect bounds = spritesBoard[initial_r][initial_c].getLocalBounds();
                    spritesBoard[initial_r][initial_c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
                    spritesBoard[initial_r][initial_c].setPosition(b[initial_r][initial_c].getPosition());
                    window.draw(spritesBoard[initial_r][initial_c]);
                }
            }
        }
    }

    void PlayChess::printMove(Move* move, MoveNode*& root, MoveNode* orig_root, MoveNode* n) {
        bool newMove = true;
        std::string pgn;

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
    }

    void PlayChess::playGame() {
        sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Play a game of chess!");
        auto windowSize = window.getSize();
        float squareSize = std::min(windowSize.x, windowSize.y) / 8.f;
        // float extraSpaceX = std::abs(sf::Vector2f(windowSize).x - sf::Vector2f(windowSize).y);
        // float extraSpaceY = std::min(windowSize.x, windowSize.y);

        Button playButton(250.f, 50.f, squareSize * 8 + 50.f, squareSize * 3, true);
        sf::Vector2f playPosition = playButton.getPosition();
        sf::Vector2f playSize = playButton.getSize();

        Button engineButton(250.f, 50.f, squareSize * 8 + 50.f, squareSize * 4.5, false);
        sf::Vector2f enginePosition = engineButton.getPosition();
        sf::Vector2f engineSize = engineButton.getSize();
        
        Button changePositionButton(250.f, 50.f, squareSize * 8 + 50.f, squareSize * 6, false);
        sf::Vector2f position = changePositionButton.getPosition();
        sf::Vector2f positionSize = changePositionButton.getSize();

        bool isDragging = false;
        bool didPromote = false;
        sf::Vector2f dragOffset;
        sf::Vector2f mousePosition;
        sf::Vector2f a;
        sf::Vector2f square1;
        sf::Vector2f square2;
        sf::Vector2f square3;

        ChessBoard chessBoard;
        startingPosition.changeTurn();
        std::vector<std::vector<ChessGame::ChessPiece>> b = chessBoard.getChessBoard();

        ChessPiece empty;
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

        int initial_r = -1;
        int initial_c = -1;
        int r;
        int c;
        float x;
        float y;

        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        std::vector<MoveNode*> children = {};
        size_t moveNumber = 0;
        MoveNode* root = new MoveNode{ nullptr, children, nullptr, moveNumber, b, {}, "" };
        MoveNode* orig_root = root;
        std::string check;
        std::string pgn;

        int depth = 4;
        Engine e(depth);
        Move engineMove;
        bool makeEngineMove = false;
        bool playMode = true;
        bool playButtonClicked = false;
        bool engineMode = false;
        bool engineButtonClicked = false;
        bool changePositionMode = false;
        bool changePositionButtonClicked = false;

        while (window.isOpen()) {

            while (const std::optional event = window.pollEvent())
            {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (makeEngineMove && engineMode) {
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
                        isDragging = true;

                        if (initial_r >= 0 && initial_r < boardSize && initial_c >= 0 && initial_c < boardSize) {
                            dragOffset.x = mousePosition.x - spritesBoard[initial_r][initial_c].getPosition().x;
                            dragOffset.y = mousePosition.y - spritesBoard[initial_r][initial_c].getPosition().y;
                        }

                        else if (changePositionButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            changePositionButtonClicked = true;
                        }

                        else if (playButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            playButtonClicked = true;
                        }

                        else if (engineButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            engineButtonClicked = true;
                        }
                    }
                }

                if (event->is<sf::Event::MouseButtonReleased>()) {

                    if (isDragging && initial_r >= 0 && initial_r < boardSize && initial_c >= 0 && initial_c < boardSize) {
                        isDragging = false;
                        didPromote = false;
                        sf::Sprite initialSprite = spritesBoard[initial_r][initial_c];
                        sf::Vector2f spritePosition = initialSprite.getPosition();
                        r = int(std::round(spritePosition.y / squareSize));
                        c = std::round(spritePosition.x / squareSize);

                        if (r < 0 || r >= boardSize || c < 0 || c >= boardSize) {
                            continue;
                        }
                        ChessPiece initialPiece = b[initial_r][initial_c];
                        ChessPiece capturedPiece = b[r][c];

                        if (initialPiece.getPieceType() == PieceType::PAWN && std::abs(c - initial_c) > 0 && b[r][c].getPieceType() == PieceType::EMPTY) {
                            capturedPiece = b[initial_r][c];
                        }
                        initialPiece.setCoordinates(initial_r, initial_c);
                        Move* move = new Move(initial_r, initial_c, r, c, initialPiece, capturedPiece);
                        auto it = std::find_if(legalMoves.begin(), legalMoves.end(), [move](const Move legalMove) {
                            return move->getInitialSquare() == legalMove.getInitialSquare() && move->getEndSquare() == legalMove.getEndSquare();
                        });

                        if (it != legalMoves.end() && (playMode || engineMode)) {
                            std::vector<MoveNode*> children;
                            moveNumber = chessBoard.whiteTurn() ? root->moveNumber + 1 : root->moveNumber;
                            MoveNode* n = new MoveNode{ move, children, root, moveNumber, b, legalMoves, "" };

                            if (Functions::push(window, chessBoard, spritesBoard, n, false, promotionSprites, legalMoves)) {
                                printMove(move, root, orig_root, n);

                                if (legalMoves.size() > 0) {
                                    makeEngineMove = true;
                                }
                            }
                        }

                        else if (changePositionMode) {
                            b[r][c] = b[initial_r][initial_c];

                            if (initial_r != r || initial_c != c) {
                                b[initial_r][initial_c] = empty;
                            }
                            spritesBoard[r][c] = spritesBoard[initial_r][initial_c];
                            b[r][c].setCoordinates(r, c);
                            chessBoard.setChessBoard(b);
                        }

                        else {
                            b[initial_r][initial_c].setCoordinates(initial_r, initial_c);
                        }
                        initial_r = -1;
                        initial_c = -1;
                    }

                    else if (changePositionButtonClicked) {

                        if (changePositionButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            changePositionMode = true;
                            playMode = false;
                            engineMode = false;
                            std::cout << "In Change Position Mode" << std::endl;
                        }
                        changePositionButtonClicked = false;
                    }

                    else if (playButtonClicked) {

                        if (playButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            playMode = true;
                            engineMode = false;
                            changePositionMode = false;
                            b = chessBoard.getChessBoard();
                            legalMoves = chessBoard.getLegalMoves();
                            root = new MoveNode{ nullptr, {}, nullptr, 0, b, {}, "" };
                            orig_root = root;
                            std::cout << "In Play Mode" << std::endl;
                        }
                        playButtonClicked = false;
                    }

                    else if (engineButtonClicked) {

                        if (engineButton.mouseOverButton(mousePosition.x, mousePosition.y)) {
                            engineMode = true;
                            playMode = false;
                            changePositionMode = false;
                            makeEngineMove = true;
                            std::cout << "In Engine Mode" << std::endl;
                        }
                        engineButtonClicked = false;
                    }
                    isDragging = false;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && root->move != nullptr && !changePositionMode) {
                    Functions::undoMove(chessBoard, spritesBoard, root, sprites);
                    root = root->parent;
                    legalMoves = chessBoard.getLegalMoves();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && root->children.size() > 0 && !changePositionMode) {
                    root = root->children[0];
                    bool _ = Functions::push(window, chessBoard, spritesBoard, root, true, promotionSprites, legalMoves);
                }
                b = chessBoard.getChessBoard();

                if (isDragging) {
                    mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                    x = mousePosition.x - dragOffset.x;
                    y = mousePosition.y - dragOffset.y;

                    if (x >= 0 && x <= squareSize * 8 && y >= 0 && y <= squareSize * 8 && initial_r >= 0 && initial_r < boardSize && initial_c >= 0 && initial_c < boardSize) {
                        b[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                    }
                }
                mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));
                playButton.setMode(playMode);
                engineButton.setMode(engineMode);
                changePositionButton.setMode(changePositionMode);

                playButton.setFillColor(mousePosition.x, mousePosition.y);
                engineButton.setFillColor(mousePosition.x, mousePosition.y);
                changePositionButton.setFillColor(mousePosition.x, mousePosition.y);

                window.clear();
                draw(window, playButton, playMode, changePositionButton, changePositionMode, engineButton, engineMode, squareSize, b, spritesBoard, initial_r, initial_c);
                window.display();
            }
        }
    }
}