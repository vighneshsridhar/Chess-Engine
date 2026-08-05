#include <SFML/Graphics.hpp>
#include "Functions.h"

namespace ChessGame {

    sf::Vector2f Functions::convertToPosition(int r, int c, float squareSize) {
        return { c * squareSize, r * squareSize };
    }

    std::pair<int, int> Functions::convertToSquare(sf::Vector2f position, float squareSize) {
        return { int(position.y / squareSize), int(position.x / squareSize) };
    }

    int Functions::convertToNumber(sf::Vector2f position, float squareSize) {
        auto [r, c] = convertToSquare(position, squareSize);
        int num = r * 8 + c;

        return num;
    }

    bool Functions::inBounds(float a, float b, float x1, float y1, float x2, float y2) {
        return a >= x1 && a <= x2 && b >= y1 && b <= y2;
    }

    bool Functions::push(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, MoveNode* n, bool rightArrow,
    std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move>& legalMoves) {
        Move move = *n->move;
        chessBoard.push(move);
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
        float squareSize = 100.f;

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

        std::unordered_map<PieceType, int> pieceToIndex;
        pieceToIndex[PieceType::QUEEN] = 0;
        pieceToIndex[PieceType::ROOK] = 1;
        pieceToIndex[PieceType::BISHOP] = 2;
        pieceToIndex[PieceType::KNIGHT] = 3;
        spritesBoard[r][c] = spritesBoard[initial_r][initial_c];

        if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
            int j;

            if (initialPiece.getColor() == PieceColor::WHITE) {
                j = 0;
            }

            else {
                j = 1;
            }

            if (!rightArrow) {
                auto [i, didPromote] = PromotionClicker::promotePawn(window, b, initialPiece, spritesBoard, promotionSprites[j], r, c);

                if (didPromote) {
                    spritesBoard[r][c] = promotionSprites[j][i];
                    n->move->setPromotionPiece(promotionPieces[i]);
                }

                else {
                    return false;
                }
            }

            else {
                auto i = pieceToIndex[n->move->getPromotionPiece().getPieceType()];
                spritesBoard[r][c] = promotionSprites[j][i];
            }
        }
        spritesBoard[r][c].setPosition(Functions::convertToPosition(r, c, squareSize));
        b[r][c].setPosition(Functions::convertToPosition(r, c, squareSize));

        if (initialPiece.getPieceType() == PieceType::KING) {

            if (c - initial_c == 2) {
                spritesBoard[r][5] = spritesBoard[r][7];
                b[r][5].setPosition(Functions::convertToPosition(r, 5, squareSize));
            }

            if (c - initial_c == -2) {
                spritesBoard[r][3] = spritesBoard[r][0];
                b[r][3].setPosition(Functions::convertToPosition(r, 3, squareSize));
            }
        }
        chessBoard.setChessBoard(b);
        legalMoves = chessBoard.getLegalMoves();

        if (chessBoard.isCheckOrCheckmate()) {
            n->checkSymbol = legalMoves.size() == 0 ? "#" : "+";
        }

        return true;
    }

    void Functions::undoMove(ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, MoveNode* n, std::vector<std::vector<sf::Sprite>> sprites) {
        Move move = *n->move;
        auto [r1, c1] = move.getInitialSquare();
        auto [r2, c2] = move.getEndSquare();
        chessBoard.unmakeMove(move);
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
        float squareSize = 100.f;

        if (b[r1][c1].getPieceType() == PieceType::KING) {

            if (c2 - c1 == 2) {
                spritesBoard[r1][c2 + 1] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
                b[r1][c2 + 1].setPosition(Functions::convertToPosition(r1, c2 + 1, squareSize));
            }

            else if (c2 - c1 == -2) {
                spritesBoard[r1][c2 - 2] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][3] : sprites[1][3];
                b[r1][c2 - 2].setPosition(Functions::convertToPosition(r1, c2 - 2, squareSize));
            }
        }
        ChessPiece capturedPiece = move.getCapturedPiece();

        if (move.getPromotionPiece().getPieceType() == PieceType::EMPTY) {
            spritesBoard[r1][c1] = spritesBoard[r2][c2];
        }

        else {
            spritesBoard[r1][c1] = b[r1][c1].getColor() == PieceColor::WHITE ? sprites[0][0] : sprites[1][0];
        }
        b[r1][c1].setPosition(Functions::convertToPosition(r1, c1, squareSize));

        if (move.isEnPassant()) {

            if (b[r1][c1].getColor() == PieceColor::WHITE) {;
                spritesBoard[r2 + 1][c2] = sprites[1][0];
                b[r2 + 1][c2].setPosition(Functions::convertToPosition(r2 + 1, c2, squareSize));
            }

            else {
                spritesBoard[r2 - 1][c2] = sprites[0][0];
                b[r2 - 1][c2].setPosition(Functions::convertToPosition(r2 - 1, c2, squareSize));
            }
        }

        else if (capturedPiece.getPieceType() != PieceType::EMPTY) {
            int type = static_cast<int>(capturedPiece.getPieceType()) - 1;
            int color = static_cast<int>(capturedPiece.getColor());
            spritesBoard[r2][c2] = sprites[color][type];
            b[r2][c2].setPosition(Functions::convertToPosition(r2, c2, squareSize));
        }
        chessBoard.setChessBoard(b);
    }
}