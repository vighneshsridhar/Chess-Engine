#include <utility>

#include "Evaluate.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include <iostream>



namespace ChessGame {

    Evaluate::Evaluate() {
        boardSize = 8;

        pieceValues[0] = 0;
        pieceValues[1] = 100;
        pieceValues[2] = 300;
        pieceValues[3] = 330;
        pieceValues[4] = 500;
        pieceValues[5] = 900;
        pieceValues[6] = 0;

        pawnActivityScoreWhite = { {0, 0, 0, 0, 0, 0, 0, 0},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {5, 5, 10, 25, 25, 10, 5, 5},
            {0, 0, 0, 30, 30, 0, 0, 0},
            {5, -5, 0, 0, 0, -20, -5, 5},
            {5, 10, 10, -35, -35, 10, 10, 5},
            {0, 0, 0, 0, 0, 0, 0, 0} };

        knightActivityScoreWhite = { { -50, -40, -30, -30, -30, -30, -40, -50},
                    { -40, -20, 0, 0, 0, 0, -20, -40 },
                    { -30, 0, 10, 15, 15, 10, 0, -30},
                {-30, 5, 15, 20, 20, 15, 5, -30},
                {-30, 0, 15, 20, 20, 15, 0, -30},
                {-30, 5, 10, 15, 15, 10, 5, -30},
                {-40, -20, 0, 5, 5, 0, -20, -40},
                {-50, -40, -30, -30, -30, -30, -40, -50} };

        bishopActivityScoreWhite = { { -20, -10, -10, -10, -10, -10, -10, -20},
            {-10, 0, 0, 0, 0, 0, 0, -10 },
            { -10, 0, 5, 10, 10, 5, 0, -10},
            {-10, 5, 5, 10, 10, 5, 5, -10},
            {-10, 0, 10, 10, 10, 10, 0, -10},
            {-10, 10, 10, 10, 10, 10, 10, -10},
            {-10, 5, 0, 5, 5, 0, 5, -10},
            {-20, -10, -10, -10, -10, -10, -10, -20} };

        rookActivityScoreWhite = { {0, 0, 0, 0, 0, 0, 0, 0},
                        {5, 10, 10, 10, 10, 10, 10, 5},
                        {-5, 0, 0, 0, 0, 0, 0, -5},
                        {-5, 0, 0, 0, 0, 0, 0, -5},
                        {-5, 0, 0, 0, 0, 0, 0, -5},
                        {-5, 0, 0, 0, 0, 0, 0, -5},
                        {-5, 0, 0, 0, 0, 0, 0, -5},
                        {0, 0, 0, 5, 5, 0, 0, 0} };

        queenActivityScoreWhite = { { -20, -10, -10, -5, -5, -10, -10, -20},
            {-10, 0, 0, 0, 0, 0, 0, -10},
            {-10, 0, 5, 5, 5, 5, 0, -10},
            {-5, 0, 5, 5, 5, 5, 0, -5},
            {0, 0, 5, 5, 5, 5, 0, -5},
            {-10, 5, 5, 5, 5, 5, 0, -10},
            {-10, 0, 5, 0, 0, 0, 0, -10},
            {-20, -10, -10, -5, -5, -10, -10, -20} };

        kingActivityScoreWhite = { { -30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-30, -40, -40, -50, -50, -40, -40, -30},
            {-20, -30, -30, -40, -40, -30, -30, -20},
            {-10, -20, -20, -20, -20, -20, -20, -10},
            {20, 20, 0, 0, 0, 0, 20, 20},
            {20, 30, 10, 0, 0, 10, 30, 20} };

        pawnActivityScoreBlack = pawnActivityScoreWhite;
        knightActivityScoreBlack = knightActivityScoreWhite;
        bishopActivityScoreBlack = bishopActivityScoreWhite;
        rookActivityScoreBlack = rookActivityScoreWhite;
        queenActivityScoreBlack = queenActivityScoreWhite;
        kingActivityScoreBlack = kingActivityScoreWhite;

        std::reverse(pawnActivityScoreBlack.begin(), pawnActivityScoreBlack.end());
        std::reverse(knightActivityScoreBlack.begin(), knightActivityScoreBlack.end());
        std::reverse(bishopActivityScoreBlack.begin(), bishopActivityScoreBlack.end());
        std::reverse(rookActivityScoreBlack.begin(), rookActivityScoreBlack.end());
        std::reverse(queenActivityScoreBlack.begin(), queenActivityScoreBlack.end());
        std::reverse(kingActivityScoreBlack.begin(), kingActivityScoreBlack.end());

        pieceTables[0] = std::make_pair(pawnActivityScoreWhite, pawnActivityScoreBlack);
        pieceTables[1] = std::make_pair(knightActivityScoreWhite, knightActivityScoreBlack);
        pieceTables[2] = std::make_pair(bishopActivityScoreWhite, bishopActivityScoreBlack);
        pieceTables[3] = std::make_pair(rookActivityScoreWhite, rookActivityScoreBlack);
        pieceTables[4] = std::make_pair(queenActivityScoreWhite, queenActivityScoreBlack);
        pieceTables[5] = std::make_pair(kingActivityScoreWhite, kingActivityScoreBlack);
    }

    int Evaluate::evaluatePosition(ChessBoard& chessBoard) {
        int score = 0;
        int boardSize = 8;

        for (int r = 0; r < boardSize; r++) {

            for (int c = 0; c < boardSize; c++) {
                ChessPiece piece = chessBoard.pieceAt(r, c);

                if (piece.getColor() == PieceColor::WHITE) {
                    score += pieceValues[static_cast<int>(piece.getPieceType())] + pieceTables[static_cast<int>(piece.getPieceType()) - 1].first[r][c];
                }

                else if (piece.getColor() == PieceColor::BLACK) {
                    score -= pieceValues[static_cast<int>(piece.getPieceType())] + pieceTables[static_cast<int>(piece.getPieceType()) - 1].second[r][c];
                }
            }
        }
        
        return score;
    }

    int Evaluate::quiescenceMax(ChessBoard& chessBoard, int alpha, int beta, unsigned long long h, TranspositionTable& tt, int runningScore) {
        TranspositionTable::TTEntry entry = tt.getTT(h);
        unsigned long long newH;

        if (entry.depth >= 0) {

            if (entry.flag == TranspositionTable::TTFlag::EXACT_EVAL) {
                return entry.eval;
            }

            if (entry.flag == TranspositionTable::TTFlag::LOWER_BOUND) {
                alpha = std::max(alpha, entry.eval);
            }

            if (entry.flag == TranspositionTable::TTFlag::UPPER_BOUND) {
                beta = std::min(beta, entry.eval);
            }

            if (alpha >= beta) {
                return entry.eval;
            }
        }
        // int bestValue = evaluatePosition(chessBoard);
        int bestValue = runningScore;
        Move bestMove;

        if (bestValue >= beta) {
            return bestValue;
        }

        if (alpha < bestValue) {
            alpha = bestValue;
        }
        std::vector<Move> captures = chessBoard.getCaptureMoves();

        if (captures.size() == 0) {

            if (chessBoard.isCheckOrCheckmate()) {
                auto e = std::numeric_limits<int>::min();
                return e;
            }

            return 0;
        }
        auto comp = [&](Move& a, Move& b) {
            return a.getCaptureScore() > b.getCaptureScore();
            };
        std::sort(captures.begin(), captures.end(), comp);

        for (auto& move : captures) {

            if (seeCapture(move, chessBoard) < 0) {
                continue;
            }
            chessBoard.push(move);
            newH = tt.updateHash(move, h);
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();
            
            auto captureValue = pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
            auto pieceSquare = pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r2][c2] -
                pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r1][c1];
            runningScore += captureValue;
            runningScore += pieceSquare;
            auto score = quiescenceMin(chessBoard, alpha, beta, newH, tt, runningScore);
            runningScore -= captureValue;
            runningScore -= pieceSquare;
            chessBoard.unmakeMove(move);

            if (score >= beta) {
                TranspositionTable::TTEntry entry = { score, 0, TranspositionTable::TTFlag::LOWER_BOUND, move };
                tt.updateTT(h, entry);

                return score;
            }

            if (score > bestValue) {
                bestValue = score;
                bestMove = move;
            }

            if (score > alpha) {
                alpha = score;
            }
        }
        entry = { bestValue, 0, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
        tt.updateTT(h, entry);

        return bestValue;
    }

    int Evaluate::quiescenceMin(ChessBoard& chessBoard, int alpha, int beta, unsigned long long h, TranspositionTable& tt, int runningScore) {
        TranspositionTable::TTEntry entry = tt.getTT(h);
        unsigned long long newH;

        if (entry.depth >= 0) {

            if (entry.flag == TranspositionTable::TTFlag::EXACT_EVAL) {
                return entry.eval;
            }

            if (entry.flag == TranspositionTable::TTFlag::LOWER_BOUND) {
                alpha = std::max(alpha, entry.eval);
            }

            if (entry.flag == TranspositionTable::TTFlag::UPPER_BOUND) {
                beta = std::min(beta, entry.eval);
            }

            if (alpha >= beta) {
                return entry.eval;
            }
        }

        int bestValue = runningScore;
        Move bestMove;

        if (bestValue <= alpha) {
            return bestValue;
        }

        if (beta > bestValue) {
            beta = bestValue;
        }
        std::vector<Move> captures = chessBoard.getCaptureMoves();

        if (captures.size() == 0) {

            if (chessBoard.isCheckOrCheckmate()) {
                auto e = std::numeric_limits<int>::max();
                return e;
            }

            return 0;
        }
        auto comp = [&](Move& a, Move& b) {
            return a.getCaptureScore() > b.getCaptureScore();
            };
        std::sort(captures.begin(), captures.end(), comp);

        for (auto& move : captures) {

            if (seeCapture(move, chessBoard) > 0) {
                continue;
            }
            chessBoard.push(move);
            newH = tt.updateHash(move, h);
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();

            auto captureValue = pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
            auto pieceSquare = pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r2][c2] -
                pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r1][c1];
            runningScore -= captureValue;
            runningScore -= pieceSquare;
            auto score = quiescenceMax(chessBoard, alpha, beta, newH, tt, runningScore);
            runningScore += captureValue;
            runningScore += pieceSquare;
            chessBoard.unmakeMove(move);

            if (score <= alpha) {
                TranspositionTable::TTEntry entry = { score, 0, TranspositionTable::TTFlag::UPPER_BOUND, move };
                tt.updateTT(h, entry);

                return score;
            }

            if (score < bestValue) {
                bestValue = score;
                bestMove = move;
            }

            if (score < beta) {
                beta = score;
            }
        }
        entry = { bestValue, 0, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
        tt.updateTT(h, entry);

        return bestValue;
    }

    int Evaluate::see(int r, int c, ChessBoard& chessBoard) {
        int value = 0;
        PieceColor side = chessBoard.whiteTurn() ? PieceColor::WHITE : PieceColor::BLACK;
        ChessPiece& attacker = Bitboard::getSmallestAttacker(chessBoard, r, c, side);
        ChessPiece& capturedPiece = chessBoard.pieceAt(r, c);
        bool wTurn = chessBoard.whiteTurn();

        if (attacker.getPieceType() != PieceType::EMPTY) {
            auto [s, t] = attacker.getCoordinates();
            Move move(s, t, r, c, attacker, capturedPiece);
            chessBoard.push(move);

            if (wTurn) {
                value = std::max(0, pieceValues[static_cast<int>(capturedPiece.getPieceType())] + see(r, c, chessBoard));
            }

            else {
                value = std::min(0, -pieceValues[static_cast<int>(capturedPiece.getPieceType())] + see(r, c, chessBoard));
            }
            chessBoard.unmakeMove(move);
        }

        return value;
    }

    int Evaluate::seeCapture(Move& move, ChessBoard& chessBoard) {
        int value = 0;
        ChessPiece& piece = move.getAttacker();
        bool wTurn = chessBoard.whiteTurn();
        auto [r, c] = move.getEndSquare();
        chessBoard.push(move);

        if (wTurn) {
            value = pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())] + see(r, c, chessBoard);
        }

        else {
            value = -pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())] + see(r, c, chessBoard);
        }
        chessBoard.unmakeMove(move);

        return value;
    }
}