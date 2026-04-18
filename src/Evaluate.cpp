#include <utility>

#include "Evaluate.h"
#include "ChessBoard.h"
#include "ChessPiece.h"



namespace ChessGame {

    Evaluate::Evaluate() {
        boardSize = 8;
        pieceValues = {
            {PieceType::PAWN, 100},
            {PieceType::KNIGHT, 320},
            {PieceType::BISHOP, 330},
            {PieceType::ROOK, 500},
            {PieceType::QUEEN, 900},
            {PieceType::KING, 0}
        };

        pawnActivityScoreWhite = { {0, 0, 0, 0, 0, 0, 0, 0},
            {50, 50, 50, 50, 50, 50, 50, 50},
            {10, 10, 20, 30, 30, 20, 10, 10},
            {5, 5, 10, 25, 25, 10, 5, 5},
            {0, 0, 0, 20, 20, 0, 0, 0},
            {5, -5, 10, 0, 0, 10, -5, 5},
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
            {-10, 5, 0, 0, 0, 0, 5, -10},
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

        for (auto& row : pawnActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        for (auto& row : knightActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        for (auto& row : bishopActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        for (auto& row : rookActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        for (auto& row : queenActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        for (auto& row : kingActivityScoreBlack) {
            std::reverse(row.begin(), row.end());
        }

        pieceTables = {
            {PieceType::PAWN, std::make_pair(pawnActivityScoreWhite, pawnActivityScoreBlack)},
            {PieceType::KNIGHT, std::make_pair(knightActivityScoreWhite, knightActivityScoreBlack)},
            {PieceType::BISHOP, std::make_pair(bishopActivityScoreWhite, bishopActivityScoreBlack)},
            {PieceType::ROOK, std::make_pair(rookActivityScoreWhite, rookActivityScoreBlack)},
            {PieceType::QUEEN, std::make_pair(queenActivityScoreWhite, queenActivityScoreBlack)},
            {PieceType::KING, std::make_pair(kingActivityScoreWhite, kingActivityScoreBlack)} };
    }

    int Evaluate::evaluatePosition(ChessBoard& chessBoard) {
        int score = 0;
        int boardSize = 8;
        std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();

        for (int r = 0; r < boardSize; r++) {

            for (int c = 0; c < boardSize; c++) {

                if (b[r][c].getColor() == PieceColor::WHITE) {
                    score += pieceValues[b[r][c].getPieceType()] + pieceTables[b[r][c].getPieceType()].first[r][c];
                }

                else if (b[r][c].getColor() == PieceColor::BLACK) {
                    score -= pieceValues[b[r][c].getPieceType()] + pieceTables[b[r][c].getPieceType()].second[r][c];
                }
            }
        }

        return score;
    }

    int Evaluate::quiescenceMax(ChessBoard& chessBoard, int alpha, int beta) {
        int stand_pat = evaluatePosition(chessBoard);
        int score;

        if (stand_pat >= beta) {
            return stand_pat;
        }

        if (alpha < stand_pat) {
            alpha = stand_pat;
        }
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        for (auto move : legalMoves) {

            if (move.isCapture()) {
                chessBoard.push(move);
                score = quiescenceMin(chessBoard, alpha, beta);
                chessBoard.unmakeMove(move);

                if (score >= beta) {
                    return beta;
                }

                if (score > alpha) {
                    alpha = score;
                }
            }
        }

        return alpha;
    }

    int Evaluate::quiescenceMin(ChessBoard& chessBoard, int alpha, int beta) {
        int stand_pat = evaluatePosition(chessBoard);
        int score;

        if (stand_pat <= alpha) {
            return stand_pat;
        }

        if (beta > stand_pat) {
            beta = stand_pat;
        }
        std::vector<Move> legalMoves = chessBoard.getLegalMoves();

        for (auto move : legalMoves) {

            if (move.isCapture()) {
                chessBoard.push(move);
                score = quiescenceMax(chessBoard, alpha, beta);
                chessBoard.unmakeMove(move);

                if (score <= alpha) {
                    return alpha;
                }

                if (score < beta) {
                    beta = score;
                }
            }
        }

        return beta;
    }
}