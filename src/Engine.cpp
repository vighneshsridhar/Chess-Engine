#include <utility>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>
#include <limits>
#include <unordered_set>

#include "Engine.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Evaluate.h"
#include "Bitboard.h"



namespace ChessGame {

    Engine::Engine(int maxDepth, ChessBoard chessBoard) : maxDepth(maxDepth) {
        boardSize = 8;
        killerMoves.resize(maxDepth, std::vector<Move>(2));

        for (int k = 0; k < maxDepth; k++) {
            killerMoves[k][0] = Move();
            killerMoves[k][1] = Move();
        }
        prevH.push(tt.getInitialH(chessBoard));
    };

    Move Engine::iterative_deepening(ChessBoard chessBoard) {
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        unsigned long long h = prevH.top();
        Move bestMove;
        unsigned long long newH;

        int runningScore = e.evaluatePosition(chessBoard);

        for (int d = 1; d < maxDepth + 1; d++) {
            int bestScore = chessBoard.whiteTurn() ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
            int alpha = std::numeric_limits<int>::min();
            int beta = std::numeric_limits<int>::max();
            auto comp = [&, d](Move& a, Move& b) {
                return a.getOrderingScore(killerMoves, d - 1) + (a.isCapture() && e.seeCapture(a, chessBoard) <= 0 ? 15000 : 0) >
                    b.getOrderingScore(killerMoves, d - 1) + (b.isCapture() && e.seeCapture(b, chessBoard) <= 0 ? 15000 : 0);
                    // a.getOrderingScore(killerMoves, d - 1) >
                    // b.getOrderingScore(killerMoves, d - 1);
                };
            std::sort(legalMoves.begin(), legalMoves.end(), comp);
            TranspositionTable::TTEntry& entry = tt.getTT(h);

            if (entry.depth != -1) {

                for (int i = 0; i < legalMoves.size(); i++) {

                    if (legalMoves[i] == entry.bestMove) {
                        std::swap(legalMoves[0], legalMoves[i]);
                        break;
                    }
                }
            }

            for (auto& move : legalMoves) {
                chessBoard.push(move);
                newH = tt.updateHash(move, true, chessBoard, h);
                tt.incrementCount(newH);

                const ChessPiece& capturedPiece = move.getCapturedPiece();
                auto [r1, c1] = move.getInitialSquare();
                auto [r2, c2] = move.getEndSquare();
                auto [s, t] = capturedPiece.getCoordinates();

                if (!chessBoard.whiteTurn()) {
                    auto captureValue = e.pieceValues[static_cast<int>(capturedPiece.getPieceType())] + 
                        e.pieceTables[static_cast<int>(capturedPiece.getPieceType())].second[s][t];
                    auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].first[r2][c2] -
                        e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].first[r1][c1];
                    runningScore += captureValue;
                    runningScore += pieceSquare;
                    auto score = alphaBetaMin(chessBoard, alpha, beta, d - 1, newH, runningScore);
                    runningScore -= captureValue;
                    runningScore -= pieceSquare;

                    if (score > bestScore) {
                        bestMove = move;
                        bestScore = score;

                        if (score > alpha) {
                            alpha = score;
                        }
                    }
                }

                else {
                    auto captureValue = e.pieceValues[static_cast<int>(capturedPiece.getPieceType())] + 
                        e.pieceTables[static_cast<int>(capturedPiece.getPieceType())].first[s][t];
                    auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].second[r2][c2] -
                        e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].second[r1][c1];
                    runningScore -= captureValue;
                    runningScore -= pieceSquare;
                    auto score = alphaBetaMax(chessBoard, alpha, beta, d - 1, newH, runningScore);
                    runningScore += captureValue;
                    runningScore += pieceSquare;

                    if (score < bestScore) {
                        bestMove = move;
                        bestScore = score;

                        if (score < beta) {
                            beta = score;
                        }
                    }
                }
                tt.decrementCount(newH);
                newH = tt.updateHash(move, false, chessBoard, newH);
                chessBoard.unmakeMove(move);
            }
            entry = { bestScore, d, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
            tt.updateTT(h, entry);
        }

        return bestMove;
    }

    int Engine::alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore) {
        if (tt.getPositionCount(h) >= 3) {
            return 0;
        }
        TranspositionTable::TTEntry& entry = tt.getTT(h);

        if (entry.depth >= depthLeft) {

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
        int bestValue = std::numeric_limits<int>::min();
        auto legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {

            if (chessBoard.isCheckOrCheckmate()) {
                return bestValue + 1;
            }

            return 0;
        }

        if (depthLeft == 0) {
            return e.quiescenceMax(chessBoard, alpha, beta, depthLeft, h, tt, runningScore);
        }

        if (entry.depth != std::numeric_limits<int>::min()) {

            for (int i = 0; i < legalMoves.size(); i++) {

                if (legalMoves[i] == entry.bestMove) {
                    std::swap(legalMoves[0], legalMoves[i]);
                    break;
                }
            }
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) + (a.isCapture() && e.seeCapture(a, chessBoard) >= 0 ? 7000 : 0) >
                b.getOrderingScore(killerMoves, depthLeft) + (b.isCapture() && e.seeCapture(b, chessBoard) >= 0 ? 7000 : 0);
                // a.getOrderingScore(killerMoves, depthLeft) >
                // b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);
        Move bestMove;
        unsigned long long newH;

        for (int i = 0; i < legalMoves.size(); i++) {
            Move& move = legalMoves[i];
            chessBoard.push(move);
            newH = tt.updateHash(move, true, chessBoard, h);
            tt.incrementCount(newH);
            const ChessPiece& capturedPiece = move.getCapturedPiece();
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();
            auto [s, t] = capturedPiece.getCoordinates();
            
            auto captureValue = e.pieceValues[static_cast<int>(capturedPiece.getPieceType())] + 
                e.pieceTables[static_cast<int>(capturedPiece.getPieceType())].second[s][t];
            auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].first[r2][c2] -
                e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].first[r1][c1];
            runningScore += captureValue;
            runningScore += pieceSquare;
            auto score = alphaBetaMin(chessBoard, alpha, beta, depthLeft - 1, newH, runningScore);
            runningScore -= captureValue;
            runningScore -= pieceSquare;
            tt.decrementCount(newH);
            newH = tt.updateHash(move, false, chessBoard, newH);
            chessBoard.unmakeMove(move);

            if (score > bestValue) {
                bestValue = score;
                bestMove = move;

                if (score > alpha) {
                    alpha = score;
                }
            }

            if (score >= beta) {

                if (!move.isCapture()) {
                    killerMoves[depthLeft][1] = killerMoves[depthLeft][0];
                    killerMoves[depthLeft][0] = move;
                }
                entry = { score, depthLeft, TranspositionTable::TTFlag::LOWER_BOUND, move };
                tt.updateTT(h, entry);

                return score;
            }
        }
        entry = { bestValue, depthLeft, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
        tt.updateTT(h, entry);

        return bestValue;
    }

    int Engine::alphaBetaMin(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore) {
        if (tt.getPositionCount(h) >= 3) {
            return 0;
        }
        TranspositionTable::TTEntry& entry = tt.getTT(h);

        if (entry.depth >= depthLeft) {

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
        int bestValue = std::numeric_limits<int>::max();
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {

            if (chessBoard.isCheckOrCheckmate()) {
                return bestValue - 1;
            }

            return 0;
        }

        if (depthLeft == 0) {
            return e.quiescenceMin(chessBoard, alpha, beta, depthLeft, h, tt, runningScore);
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) + (a.isCapture() && e.seeCapture(a, chessBoard) <= 0 ? 15000 : 0) >
                b.getOrderingScore(killerMoves, depthLeft) + (b.isCapture() && e.seeCapture(b, chessBoard) <= 0 ? 15000 : 0);
                // a.getOrderingScore(killerMoves, depthLeft) >
                // b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);

        Move bestMove;
        unsigned long long newH;

        if (entry.depth != std::numeric_limits<int>::min()) {

            for (int i = 0; i < legalMoves.size(); i++) {

                if (legalMoves[i] == entry.bestMove) {
                    std::swap(legalMoves[0], legalMoves[i]);
                    break;
                }
            }
        }

        for (int i = 0; i < legalMoves.size(); i++) {
            Move& move = legalMoves[i];
            chessBoard.push(move);
            newH = tt.updateHash(move, true, chessBoard, h);
            tt.incrementCount(newH);
            const ChessPiece& capturedPiece = move.getCapturedPiece();
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();
            auto [s, t] = capturedPiece.getCoordinates();

            auto captureValue = e.pieceValues[static_cast<int>(capturedPiece.getPieceType())] + 
                e.pieceTables[static_cast<int>(capturedPiece.getPieceType())].first[s][t];
            auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].second[r2][c2] -
                e.pieceTables[static_cast<int>(move.getAttacker().getPieceType())].second[r1][c1];
            runningScore -= captureValue;
            runningScore -= pieceSquare;
            auto score = alphaBetaMax(chessBoard, alpha, beta, depthLeft - 1, newH, runningScore);
            runningScore += captureValue;
            runningScore += pieceSquare;
            tt.decrementCount(newH);
            newH = tt.updateHash(move, false, chessBoard, newH);
            chessBoard.unmakeMove(move);

            if (score < bestValue) {
                bestValue = score;
                bestMove = move;

                if (score < beta) {
                    beta = score;
                }
            }

            if (score <= alpha) {

                if (!move.isCapture()) {
                    killerMoves[depthLeft][1] = killerMoves[depthLeft][0];
                    killerMoves[depthLeft][0] = move;
                }
                entry = { score, depthLeft, TranspositionTable::TTFlag::UPPER_BOUND, move };
                tt.updateTT(h, entry);

                return score;
            }
        }
        entry = { bestValue, depthLeft, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
        tt.updateTT(h, entry);

        return bestValue;
    }

    void Engine::enginePush(Move move, ChessBoard& chessBoard) {
        unsigned long long h = tt.updateHash(move, true, chessBoard, prevH.top());
        tt.incrementCount(h);
        prevH.push(h);
        bestMoves.push(move);

        return;
    }

    void Engine::engineUnmakeMove(Move move, ChessBoard& chessBoard) {
        tt.decrementCount(prevH.top());
        unsigned long long h = tt.updateHash(move, false, chessBoard, prevH.top());
        prevH.pop();
        bestMoves.pop();

        return;
    }
}