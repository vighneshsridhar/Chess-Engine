#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <bitset>
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
#include "Move.h"

namespace ChessGame {

    ChessBoard::ChessBoard() {
        boardSize = 8;
        wTurn = true;
        enPassantFile = -1;
        float squareSize = 100.f;
        int x = -1;
        int y = -1;

        ChessGame::ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, x, y);
        ChessGame::ChessPiece whitePawn(PieceType::PAWN, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteKnight(PieceType::KNIGHT, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteBishop(PieceType::BISHOP, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteRook(PieceType::ROOK, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteQueen(PieceType::QUEEN, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteKing(PieceType::KING, PieceColor::WHITE, x, y);

        ChessGame::ChessPiece blackPawn(PieceType::PAWN, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackKnight(PieceType::KNIGHT, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackBishop(PieceType::BISHOP, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackRook(PieceType::ROOK, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackQueen(PieceType::QUEEN, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackKing(PieceType::KING, PieceColor::BLACK, x, y);

        b = { {blackRook, blackKnight, blackBishop, blackQueen, blackKing, blackBishop, blackKnight, blackRook},
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

            for (int c = 0; c < boardSize; c++) {
                b[r][c].setCoordinates(r, c);
                b[r][c].setPosition(Functions::convertToPosition(r, c));
            }
        }
    };

    bool ChessBoard::whiteTurn() const {
        return wTurn;
    }

    void ChessBoard::changeTurn() {
        wTurn = !wTurn;
        return;
    }

    std::vector<std::vector<ChessPiece>> ChessBoard::getChessBoard() {
        return b;
    }

    void ChessBoard::setChessBoard(std::vector<std::vector<ChessPiece>> newChessBoard) {
        b = newChessBoard;
    }

    ChessPiece ChessBoard::pieceAt(int r, int c) {
        return b[r][c];
    }

    void ChessBoard::setPieceAt(ChessPiece piece, int r, int c) {
        b[r][c] = piece;
    }

    std::vector<Move> ChessBoard::getLegalMoves() {
        std::vector<Move> legalMoves;
        std::vector<Move> moves;

        for (int r = 0; r < boardSize; r++) {

            for (int c = 0; c < boardSize; c++) {
                ChessPiece piece = b[r][c];

                if ((wTurn && piece.getColor() != PieceColor::WHITE) || (!wTurn && piece.getColor() != PieceColor::BLACK)) {
                    continue;
                }
                moves = getPieceMoves(piece);

                for (auto& move : moves) {
                    auto [x, y] = move.getEndSquare();
                    ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, x, y);
                    ChessPiece old = b[x][y];
                    
                    b[x][y] = piece;
                    b[r][c] = empty;

                    if (piece.getPieceType() == PieceType::KING) {
                        setKingPosition(std::make_pair(x, y));
                    }
                    // check = isCheckOrCheckmate();
                    // move.setCheck(check);

                    if (Bitboard::isValidBoard(*this)) {
                        legalMoves.push_back(move);
                    }
                    b[r][c] = piece;
                    b[x][y] = old;

                    if (piece.getPieceType() == PieceType::KING) {
                        setKingPosition(std::make_pair(r, c));
                    }
                }
            }
        }
        return legalMoves;
    }

    std::vector<Move> ChessBoard::getPieceMoves(ChessPiece& piece) {
        std::vector<Move> moves;

        if (piece.getPieceType() == PieceType::PAWN) {
            moves = Pawn::getMoves(*this, piece);
        }

        if (piece.getPieceType() == PieceType::KNIGHT) {
            moves = Knight::getMoves(*this, piece);
        }

        if (piece.getPieceType() == PieceType::BISHOP) {
            moves = Bishop::getMoves(*this, piece);
        }

        if (piece.getPieceType() == PieceType::ROOK) {
            moves = Rook::getMoves(*this, piece);
        }

        if (piece.getPieceType() == PieceType::QUEEN) {
            moves = Queen::getMoves(*this, piece);
        }

        if (piece.getPieceType() == PieceType::KING) {
            moves = King::getMoves(*this, piece);
        }

        return moves;
    }

    void ChessBoard::push(Move move) {
        auto [initial_r, initial_c] = move.getInitialSquare();
        auto [r, c] = move.getEndSquare();

        ChessPiece initialPiece = b[initial_r][initial_c];
        ChessPiece capturedPiece = move.getCapturedPiece();
        ChessPiece empty;

        if (initialPiece.getPieceType() == PieceType::PAWN && (r == 0 || r == 7)) {
            b[r][c] = move.getPromotionPiece();
            b[r][c].setCoordinates(r, c);
        }

        else {
            b[r][c] = b[initial_r][initial_c];
            b[r][c].setCoordinates(r, c);
        }

        if (initialPiece.getPieceType() == PieceType::PAWN) {

            if (std::abs(r - initial_r) == 2) {
                this->setEnPassantFile(c);
            }

            if (move.isEnPassant()) {
                int rank = initialPiece.getColor() == PieceColor::WHITE ? r + 1 : r - 1;
                b[rank][c] = empty;
            }
        }

        if (initialPiece.getPieceType() == PieceType::KING) {
            this->setKingPosition(std::make_pair(r, c));
            int rookC = -1;
            int delta = -1;

            if (c - initial_c == 2) {
                rookC = 7;
                delta = -2;
            }

            if (c - initial_c == -2) {
                rookC = 0;
                delta = 3;
            }

            if (rookC != -1) {
                b[r][rookC + delta] = b[r][rookC];
                b[r][rookC + delta].setCoordinates(r, rookC + delta);
                b[r][rookC] = empty;
            }
        }
        b[r][c].setPieceHasMoved();
        b[initial_r][initial_c] = empty;
        b[initial_r][initial_c].setCoordinates(initial_r, initial_c);
        this->changeTurn();

        return;
    }

    void ChessBoard::unmakeMove(Move move) {
        this->changeTurn();
        auto [initial_r, initial_c] = move.getInitialSquare();
        auto [r, c] = move.getEndSquare();

        ChessPiece initialPiece = move.getAttacker();
        ChessPiece capturedPiece = move.getCapturedPiece();
        ChessPiece empty;
        b[r][c] = capturedPiece;
        b[initial_r][initial_c] = initialPiece;

        if (initialPiece.getPieceType() == PieceType::PAWN) {

            if (std::abs(r - initial_r) == 2) {
                this->setEnPassantFile(c);
            }

            if (move.isEnPassant()) {
                int rank = initialPiece.getColor() == PieceColor::WHITE ? r + 1 : r - 1;
                b[rank][c] = empty;
            }
        }

        if (initialPiece.getPieceType() == PieceType::KING) {
            this->setKingPosition(std::make_pair(initial_r, initial_c));

            if (c - initial_c == 2) {
                b[r][7] = b[r][5];
                b[r][7].setCoordinates(r, 7);
                b[r][5] = empty;
            }

            if (c - initial_c == -2) {
                auto square3 = Functions::convertToPosition(r, 0);
                b[r][0] = b[r][3];
                b[r][0].setCoordinates(r, 7);
                b[r][3] = empty;
            }
        }

        return;
    }

    std::pair<int, int> ChessBoard::getKingPosition() {
        if (wTurn) {
            return kingPosition["WHITE_KING"];
        }

        return kingPosition["BLACK_KING"];
    }

    void ChessBoard::setKingPosition(std::pair<int, int> coordinates) {
        if (wTurn) {
            kingPosition["WHITE_KING"] = coordinates;
        }

        else {
            kingPosition["BLACK_KING"] = coordinates;
        }
    }

    int ChessBoard::getEnPassantFile() {
        return enPassantFile;
    }

    void ChessBoard::setEnPassantFile(int file) {
        enPassantFile = file;
        return;
    }

    bool ChessBoard::isCheckOrCheckmate() {
        Bitboard bb;
        bool k = bb.isValidBoard(*this);
        
        return k;
    }
}