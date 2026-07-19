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

    Engine::Engine(int maxDepth) : maxDepth(maxDepth) {
        boardSize = 8;
        h = 0;
        killerMoves.resize(maxDepth, std::vector<Move>(2));

        for (int k = 0; k < maxDepth; k++) {
            killerMoves[k][0] = Move();
            killerMoves[k][1] = Move();
        }
    };

    Move Engine::iterative_deepening(ChessBoard chessBoard) {
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        Move bestMove;
        unsigned long long oldH = h;
        unsigned long long newH;

        int runningScore = e.evaluatePosition(chessBoard);
        bool wTurn = chessBoard.whiteTurn();

        for (int d = 1; d < maxDepth + 1; d++) {
            int bestScore = wTurn ? std::numeric_limits<int>::min() : std::numeric_limits<int>::max();
            int alpha = std::numeric_limits<int>::min();
            int beta = std::numeric_limits<int>::max();
            auto comp = [&, d](Move& a, Move& b) {
                return a.getOrderingScore(killerMoves, d - 1) >
                    b.getOrderingScore(killerMoves, d - 1);
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
                newH = tt.updateHash(move, oldH);
                auto [r1, c1] = move.getInitialSquare();
                auto [r2, c2] = move.getEndSquare();

                if (wTurn) {
                    auto captureValue = e.pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
                    auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r2][c2] -
                        e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r1][c1];
                    runningScore += captureValue;
                    runningScore += pieceSquare;
                    auto score = alphaBetaMin(chessBoard, alpha, beta, d - 1, newH, runningScore);
                    runningScore -= captureValue;
                    runningScore -= pieceSquare;

                    if (score > bestScore) {
                        bestMove = move;
                        bestScore = score;
                        h = newH;

                        if (score > alpha) {
                            alpha = score;
                        }
                    }
                }

                else {
                    auto captureValue = e.pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
                    auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r2][c2] -
                        e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r1][c1];
                    runningScore -= captureValue;
                    runningScore -= pieceSquare;
                    auto score = alphaBetaMax(chessBoard, alpha, beta, d - 1, newH, runningScore);
                    runningScore += captureValue;
                    runningScore += pieceSquare;

                    if (score < bestScore) {
                        bestMove = move;
                        bestScore = score;
                        h = newH;

                        if (score < beta) {
                            beta = score;
                        }
                    }
                }
                chessBoard.unmakeMove(move);
            }
            entry = { bestScore, d, TranspositionTable::TTFlag::EXACT_EVAL, bestMove };
            tt.updateTT(h, entry);
        }

        return bestMove;
    }

    int Engine::alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore) {
        if (depthLeft == 0) {
            auto start = std::chrono::high_resolution_clock::now();
            auto ans = e.quiescenceMax(chessBoard, alpha, beta, h, tt, runningScore);
            auto end = std::chrono::high_resolution_clock::now();

            /*std::cout << "Quiescence: "
                << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
                << " ms\n"; */
            return ans;
        }

        int bestValue = std::numeric_limits<int>::min();
        auto start = std::chrono::high_resolution_clock::now();
        auto legalMoves = chessBoard.getLegalMoves();
        auto end = std::chrono::high_resolution_clock::now();

        /*std::cout << "Legal move generation: "
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
            << " ms\n"; */

        if (legalMoves.size() == 0) {

            if (chessBoard.isCheckOrCheckmate()) {
                return bestValue;
            }

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

        if (entry.depth != -1) {

            for (int i = 0; i < legalMoves.size(); i++) {

                if (legalMoves[i] == entry.bestMove) {
                    std::swap(legalMoves[0], legalMoves[i]);
                    break;
                }
            }
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);
        Move bestMove;
        unsigned long long newH;

        for (int i = 0; i < legalMoves.size(); i++) {
            Move& move = legalMoves[i];

            if (move.isCapture() && e.seeCapture(move, chessBoard) < 0) {
                continue;
            }
            newH = tt.updateHash(move, h);
            chessBoard.push(move);
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();
            
            auto captureValue = e.pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
            auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r2][c2] -
                e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].first[r1][c1];
            runningScore += captureValue;
            runningScore += pieceSquare;
            auto score = alphaBetaMin(chessBoard, alpha, beta, depthLeft - 1, newH, runningScore);
            runningScore -= captureValue;
            runningScore -= pieceSquare;

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
        if (depthLeft == 0) {
            return e.quiescenceMin(chessBoard, alpha, beta, h, tt, runningScore);
        }
        int bestValue = std::numeric_limits<int>::max();
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {
            
            if (chessBoard.isCheckOrCheckmate()) {
                return bestValue;
            }

            return 0;
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);

        Move bestMove;
        unsigned long long newH;
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

        if (entry.depth != -1) {

            for (int i = 0; i < legalMoves.size(); i++) {

                if (legalMoves[i] == entry.bestMove) {
                    std::swap(legalMoves[0], legalMoves[i]);
                    break;
                }
            }
        }

        for (int i = 0; i < legalMoves.size(); i++) {
            Move& move = legalMoves[i];

            if (move.isCapture() && e.seeCapture(move, chessBoard) > 0) {
                continue;
            }
            newH = tt.updateHash(move, h);
            chessBoard.push(move);
            auto [r1, c1] = move.getInitialSquare();
            auto [r2, c2] = move.getEndSquare();

            auto captureValue = e.pieceValues[static_cast<int>(move.getCapturedPiece().getPieceType())];
            auto pieceSquare = e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r2][c2] -
                e.pieceTables[static_cast<int>(move.getAttacker().getPieceType()) - 1].second[r1][c1];
            runningScore -= captureValue;
            runningScore -= pieceSquare;
            auto score = alphaBetaMax(chessBoard, alpha, beta, depthLeft - 1, newH, runningScore);
            runningScore += captureValue;
            runningScore += pieceSquare;
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
}