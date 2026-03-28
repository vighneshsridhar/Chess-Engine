#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Bitboard.h"

namespace ChessGame {
    ChessBoard::ChessBoard() {
        boardSize = 8;
        whiteTurn = true;
        float squareSize = 100.f;
        float x = 20.f;
        float y = 20.f;

        ChessGame::ChessPiece empty("EMPTY_SQUARE", "NONE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whitePawn("PAWN", "WHITE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteKnight("KNIGHT", "WHITE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteBishop("BISHOP", "WHITE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteRook("ROOK", "WHITE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteQueen("QUEEN", "WHITE", sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteKing("KING", "WHITE", sf::Vector2f(x, y));

        ChessGame::ChessPiece blackPawn("PAWN", "BLACK", sf::Vector2f(x, y));
        ChessGame::ChessPiece blackKnight("KNIGHT", "BLACK", sf::Vector2f(x, y));
        ChessGame::ChessPiece blackBishop("BISHOP", "BLACK", sf::Vector2f(x, y));
        ChessGame::ChessPiece blackRook("ROOK", "BLACK", sf::Vector2f(x, y));
        ChessGame::ChessPiece blackQueen("QUEEN", "BLACK", sf::Vector2f(x, y));
        ChessGame::ChessPiece blackKing("KING", "BLACK", sf::Vector2f(x, y));

        chessBoard = { {blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackBishop, blackKnight, blackRook},
            {blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn, blackPawn},
        {empty, empty, empty, empty, empty, empty, empty, empty},
        {empty, empty, empty, empty, empty, empty, empty, empty},
        {empty, empty, empty, empty, empty, empty, empty, empty},
        {empty, empty, empty, empty, empty, empty, empty, empty},
        {whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn, whitePawn},
        {whiteRook, whiteKnight, whiteBishop, whiteQueen, whiteKing, whiteBishop, whiteKnight, whiteRook} };
        kingPosition["WHITE_KING"] = std::make_pair(7, 4);
        kingPosition["BLACK_KING"] = std::make_pair(0, 4);

        for (int r = 0; r < boardSize; r++) {
            y = squareSize * r;

            for (int c = 0; c < boardSize; c++) {
                x = squareSize * c;
                chessBoard[r][c].setPosition(sf::Vector2f(x, y));
            }
        }
    };

    bool ChessBoard::getTurn() {
        return whiteTurn;
    }

    void ChessBoard::changeTurn() {
        whiteTurn = !whiteTurn;
        return;
    }

    std::vector<std::vector<ChessPiece>> ChessBoard::getChessBoard() {
        return chessBoard;
    }

    void ChessBoard::setChessBoard(std::vector<std::vector<ChessPiece>> newChessBoard) {
        chessBoard = newChessBoard;
    }

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> ChessBoard::getLegalMoves() {
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves;
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;
        Bitboard bb;
        std::vector<long long> x;
        sf::Vector2f position;
        bool castling = false;

        for (int r = 0; r < boardSize; r++) {

            for (int c = 0; c < boardSize; c++) {
                ChessPiece piece = chessBoard[r][c];

                if ((whiteTurn && piece.getColor() != "WHITE") || (!whiteTurn && piece.getColor() != "BLACK")) {
                    continue;
                }
                position = Functions::convertToPosition(r, c);
                moves = getPieceMoves(piece);

                for (const auto& move : moves) {
                    auto [x, y] = Functions::convertToSquare(move.second);
                    ChessPiece empty("EMPTY_SQUARE", "NONE", position);
                    ChessPiece old = chessBoard[x][y];
                    
                    chessBoard[x][y] = piece;
                    chessBoard[r][c] = empty;

                    if (piece.getPieceType() == "KING") {
                        setKingPosition(std::make_pair(x, y));

                        if (std::abs(y - c) == 2) {
                            castling = true;
                        }
                    }
                    this->changeTurn();
                    bb.updateBitboard(*this, castling);
                    this->changeTurn();

                    if (bb.isValidBoard(*this, castling)) {
                        legalMoves.push_back(move);
                    }
                    chessBoard[r][c] = piece;
                    chessBoard[x][y] = old;

                    if (piece.getPieceType() == "KING") {
                        setKingPosition(std::make_pair(r, c));
                    }
                    castling = false;
                }
            }
        }
        return legalMoves;
    }

    std::vector<std::pair<sf::Vector2f, sf::Vector2f>> ChessBoard::getPieceMoves(ChessPiece piece) {
        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;

        if (piece.getPieceType() == "PAWN") {

            if (piece.getColor() == "WHITE") {
                Pawn pawn("WHITE");
                moves = pawn.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                Pawn pawn("BLACK");
                moves = pawn.getMoves(chessBoard, piece);
            }
        }

        if (piece.getPieceType() == "KNIGHT") {

            if (piece.getColor() == "WHITE") {
                Knight knight("WHITE");
                moves = knight.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                Knight knight("BLACK");
                moves = knight.getMoves(chessBoard, piece);
            }
        }

        if (piece.getPieceType() == "BISHOP") {

            if (piece.getColor() == "WHITE") {
                Bishop bishop("WHITE");
                moves = bishop.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                Bishop bishop("BLACK");
                moves = bishop.getMoves(chessBoard, piece);
            }
        }

        if (piece.getPieceType() == "ROOK") {

            if (piece.getColor() == "WHITE") {
                Rook rook("WHITE");
                moves = rook.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                Rook rook("BLACK");
                moves = rook.getMoves(chessBoard, piece);
            }
        }

        if (piece.getPieceType() == "QUEEN") {

            if (piece.getColor() == "WHITE") {
                Queen queen("WHITE");
                moves = queen.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                Queen queen("BLACK");
                moves = queen.getMoves(chessBoard, piece);
            }
        }

        if (piece.getPieceType() == "KING") {

            if (piece.getColor() == "WHITE") {
                King king("WHITE");
                moves = king.getMoves(chessBoard, piece);
            }

            if (piece.getColor() == "BLACK") {
                King king("BLACK");
                moves = king.getMoves(chessBoard, piece);
            }
        }

        return moves;
    }

    std::pair<int, int> ChessBoard::getKingPosition() {
        if (whiteTurn) {
            return kingPosition["WHITE_KING"];
        }

        return kingPosition["BLACK_KING"];
    }

    void ChessBoard::setKingPosition(std::pair<int, int> coordinates) {
        if (whiteTurn) {
            kingPosition["WHITE_KING"] = coordinates;
        }

        else {
            kingPosition["BLACK_KING"] = coordinates;
        }
    }

    bool ChessBoard::isCheckmate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves) {
        if (legalMoves.size() == 0) {
            Bitboard bb;
            this->changeTurn();
            bb.updateBitboard(*this, false);
            this->changeTurn();
            auto [l, m] = getKingPosition();
            std::vector<long long> squaresAttacked = bb.getBitboard();
            int k = l * 8 + m;

            if (squaresAttacked[k] != 0) {
                return true;
            }
        }
        return false;
    }

    bool ChessBoard::isStalemate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves) {
        if (legalMoves.size() == 0) {
            Bitboard bb;
            this->changeTurn();
            bb.updateBitboard(*this, false);
            this->changeTurn();
            auto [l, m] = getKingPosition();
            std::vector<long long> squaresAttacked = bb.getBitboard();
            int k = l * 8 + m;

            if (squaresAttacked[k] == 0) {
                return true;
            }
        }
        return false;
    }
}