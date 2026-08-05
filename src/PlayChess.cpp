#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include "PlayChess.h"


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
        
        const sf::Font font("Assets/arial.ttf");
        sf::Text playText(font, "Play");
        float offset = playMode ? 5.f : 0;
        sf::Vector2f localOffset = playText.getGlobalBounds().size / 2.0f + playText.getLocalBounds().position;
        playText.setOrigin(localOffset);
        sf::Vector2f position = p.getPosition() + p.getSize() / 2.0f;
        position.y += offset;
        playText.setPosition(position);
        playText.setCharacterSize(30);
        playText.setFillColor(sf::Color::Black);

        sf::Text engineText(font, "Analyze");
        offset = engineMode ? 5.f : 0;
        localOffset = engineText.getGlobalBounds().size / 2.0f + engineText.getLocalBounds().position;
        engineText.setOrigin(localOffset);
        position = e.getPosition() + e.getSize() / 2.0f;
        position.y += offset;
        engineText.setPosition(position);
        engineText.setCharacterSize(30);
        engineText.setFillColor(sf::Color::Black);

        sf::Text changePositionText(font, "Change Position");
        offset = changePositionMode ? 5.f : 0;
        localOffset = changePositionText.getGlobalBounds().size / 2.0f + changePositionText.getLocalBounds().position;
        changePositionText.setOrigin(localOffset);
        position = cp.getPosition() + cp.getSize() / 2.0f;
        position.y += offset;
        changePositionText.setPosition(position);
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
                    spritesBoard[r][c].setPosition(Functions::convertToPosition(r, c, squareSize));
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
        std::cout << pgn + "\n" << std::endl;
    }

    void PlayChess::playGame() {
        sf::RenderWindow window(sf::VideoMode({ 1200, 800 }), "Play a game of chess!");
        auto windowSize = window.getSize();
        float squareSize = std::min(windowSize.x, windowSize.y) / 8.f;

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
        sf::FloatRect spriteGlobalBounds;
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
        sf::Sprite emptySprite(textures["EMPTY_TEXTURE"]);
        sf::Sprite cursorSprite(textures["CURSOR"]);

        std::vector<std::vector<sf::Sprite>> spritesBoard(boardSize, std::vector<sf::Sprite>(boardSize, whitePawnSprite));
        std::vector<std::vector<sf::Sprite>> promotionSprites = { { whiteQueenSprite, whiteRookSprite, whiteBishopSprite, whiteKnightSprite },
            { blackQueenSprite, blackRookSprite, blackBishopSprite, blackKnightSprite } };
        std::vector<std::vector<sf::Sprite>> sprites = { {whitePawnSprite, whiteKnightSprite, whiteBishopSprite, whiteRookSprite, whiteQueenSprite, whiteKingSprite},
            {blackPawnSprite, blackKnightSprite, blackBishopSprite, blackRookSprite, blackQueenSprite, blackKingSprite} };
        std::vector<std::vector<sf::Sprite>> pieceSprites = { {blackPawnSprite, blackKnightSprite, blackBishopSprite, blackRookSprite, blackQueenSprite, blackKingSprite,
            cursorSprite}, {whitePawnSprite, whiteKnightSprite, whiteBishopSprite, whiteRookSprite, whiteQueenSprite, whiteKingSprite, emptySprite} };

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
        bool printPgn = false;

        int depth = 5;
        Engine e(depth, chessBoard);
        Move engineMove;
        bool makeEngineMove = false;
        bool playMode = true;
        bool playButtonClicked = false;
        bool engineMode = false;
        bool engineButtonClicked = false;
        bool changePositionMode = false;
        bool changePositionButtonClicked = false;
        int prevMode = 0;

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
                    printPgn = true;
                    makeEngineMove = false;
                }

                if (changePositionMode) {
                    std::cout << "In Change Position Mode" << std::endl;
                    Position::setupBoard(window, chessBoard, spritesBoard, pieceSprites);
                    changePositionMode = false;
                }

                if (event->is<sf::Event::MouseButtonPressed>()) {

                    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                        mousePosition = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        initial_r = int(mousePosition.y / squareSize);
                        initial_c = int(mousePosition.x / squareSize);
                        isDragging = true;

                        if (initial_r >= 0 && initial_r < boardSize && initial_c >= 0 && initial_c < boardSize) {
                            spriteGlobalBounds = spritesBoard[initial_r][initial_c].getGlobalBounds();
                            dragOffset.x = spriteGlobalBounds.size.x / 2.0f;
                            dragOffset.y = spriteGlobalBounds.size.y / 2.0f;
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
                        c = int(std::round(spritePosition.x / squareSize));

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
                                e.enginePush(*move, chessBoard);
                                printMove(move, root, orig_root, n);

                                if (legalMoves.size() > 0) {
                                    makeEngineMove = true;
                                }
                            }
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
                            prevMode = 2;
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

                            if (prevMode == 2) {
                                moveNumber = 0;
                                root = new MoveNode{ nullptr, {}, nullptr, moveNumber, b, {}, "" };
                                orig_root = root;
                            }
                            prevMode = 0;
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

                            if (prevMode == 2) {
                                moveNumber = 0;
                                root = new MoveNode{ nullptr, {}, nullptr, moveNumber, b, {}, "" };
                                orig_root = root;
                                chessBoard.setTurn(PieceColor::WHITE);
                            }
                            legalMoves = chessBoard.getLegalMoves();
                            prevMode = 1;
                            std::cout << "In Engine Mode" << std::endl;
                        }
                        engineButtonClicked = false;
                    }
                    isDragging = false;
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && root->move != nullptr && !changePositionMode) {
                    e.engineUnmakeMove(*root->move, chessBoard);
                    Functions::undoMove(chessBoard, spritesBoard, root, sprites);
                    root = root->parent;
                    legalMoves = chessBoard.getLegalMoves();
                }

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && root->children.size() > 0 && !changePositionMode) {
                    root = root->children[0];
                    bool _ = Functions::push(window, chessBoard, spritesBoard, root, true, promotionSprites, legalMoves);
                    e.enginePush(*(root->move), chessBoard);
                }
                b = chessBoard.getChessBoard();
                mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

                if (isDragging) {
                    x = mousePosition.x - dragOffset.x;
                    y = mousePosition.y - dragOffset.y;

                    if (initial_r >= 0 && initial_r < boardSize && initial_c >= 0 && initial_c < boardSize) {
                        b[initial_r][initial_c].setPosition(sf::Vector2f(x, y));
                    }
                }
                playButton.setMode(playMode);
                engineButton.setMode(engineMode);
                changePositionButton.setMode(changePositionMode);

                playButton.setFillColor(mousePosition.x, mousePosition.y);
                engineButton.setFillColor(mousePosition.x, mousePosition.y);
                changePositionButton.setFillColor(mousePosition.x, mousePosition.y);

                window.clear();
                draw(window, playButton, playMode, changePositionButton, changePositionMode, engineButton, engineMode, squareSize, b, spritesBoard, initial_r, initial_c);
                window.display();

                if (printPgn) {
                    std::cout << "Engine Move: " << pgn << std::endl;
                    printPgn = false;
                }
            }
        }
    }
}