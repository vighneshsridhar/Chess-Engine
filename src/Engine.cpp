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

    /*Move Engine::analyze(ChessBoard& chessBoard) {
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        Move bestMove;
        int bestScore = 0;
        constexpr int alpha = std::numeric_limits<int>::min();
        constexpr int beta = std::numeric_limits<int>::max();
        int h = tt_zobrist.getInitialH();
        tt.insert({ h, 0 });

        if (chessBoard.whiteTurn()) {
            bestScore = alphaBetaMax(chessBoard, alpha, beta, depth, h);
        }

        else {
            bestScore = alphaBetaMin(chessBoard, alpha, beta, depth, h);
        }

        return bestMove;
    } */

    Move Engine::iterative_deepening(ChessBoard chessBoard) {
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();
        Move bestMove;
        unsigned long long newH;
        unsigned long long oldH = h;
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

            for (auto& move : legalMoves) {
                chessBoard.push(move);
                newH = tt_zobrist.updateHash(move, oldH);

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
        }

        return bestMove;
    }

    int Engine::alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h) {
        if (depthLeft == 0) {
            return e.quiescenceMax(chessBoard, alpha, beta, tt, h);
        }

        if (tt.find(h) != tt.end() && tt_depth[h] >= depthLeft) {
            return tt[h];
        }
        int bestValue = std::numeric_limits<int>::min();
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {
            return e.quiescenceMax(chessBoard, alpha, beta, tt, h);
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);
        Move bestMove;
        unsigned long long newH;

        for (auto& move : legalMoves) {
            newH = tt_zobrist.updateHash(move, h);
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
                tt[h] = score;
                tt_depth[h] = depthLeft;

                return score;
            }
        }
        tt[h] = bestValue;
        tt_depth[h] = depthLeft;

        return bestValue;
    }

    int Engine::alphaBetaMin(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h) {
        if (depthLeft == 0) {
            return e.quiescenceMin(chessBoard, alpha, beta, tt, h);
        }

        if (tt.find(h) != tt.end() && tt_depth[h] >= depthLeft) {
            return tt[h];
        }
        int bestValue = std::numeric_limits<int>::max();
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        if (legalMoves.size() == 0) {
            return e.quiescenceMin(chessBoard, alpha, beta, tt, h);
        }
        auto comp = [&, depthLeft](Move& a, Move& b) {
            return a.getOrderingScore(killerMoves, depthLeft) >
                b.getOrderingScore(killerMoves, depthLeft);
            };
        std::sort(legalMoves.begin(), legalMoves.end(), comp);

        Move bestMove;
        unsigned long long newH;

        for (auto& move : legalMoves) {
            newH = tt_zobrist.updateHash(move, h);
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
                tt[h] = score;
                tt_depth[h] = depthLeft;

                return score;
            }
        }
        tt[h] = bestValue;
        tt_depth[h] = depthLeft;

        return bestValue;
    }
}