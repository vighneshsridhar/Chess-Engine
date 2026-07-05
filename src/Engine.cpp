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
        int score;
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
            TranspositionTable::TTEntry entry = tt.getTT(h);

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

                if (wTurn) {
                    score = alphaBetaMin(chessBoard, alpha, beta, d - 1, newH);

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
                    score = alphaBetaMax(chessBoard, alpha, beta, d - 1, newH);

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

    int Engine::alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h) {
        if (depthLeft == 0) {
            auto start = std::chrono::high_resolution_clock::now();
            auto ans = e.quiescenceMax(chessBoard, alpha, beta, h, tt);
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
        TranspositionTable::TTEntry entry = tt.getTT(h);

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

        if (legalMoves.size() == 0) {
            return bestValue;
        }
        /*auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp); */
        Move bestMove;
        unsigned long long newH;

        for (int i = 0; i < legalMoves.size(); i++) {
            Move& move = legalMoves[i];
            newH = tt.updateHash(move, h);
            chessBoard.push(move);
            auto score = alphaBetaMin(chessBoard, alpha, beta, depthLeft - 1, newH);
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

    int Engine::alphaBetaMin(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h) {
        if (depthLeft == 0) {
            return e.quiescenceMin(chessBoard, alpha, beta, h, tt);
        }
        int bestValue = std::numeric_limits<int>::max();
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {
            return bestValue;
        }
        /*auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            }; */
        //std::sort(legalMoves.begin(), legalMoves.end(), comp);

        Move bestMove;
        unsigned long long newH;
        TranspositionTable::TTEntry entry = tt.getTT(h);

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
            newH = tt.updateHash(move, h);
            chessBoard.push(move);
            auto score = alphaBetaMax(chessBoard, alpha, beta, depthLeft - 1, newH);
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