#ifndef ChessGame_Evaluate_h
#define ChessGame_Evaluate_h


#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_set>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "TranspositionTable.h"
#include "Bitboard.h"


namespace ChessGame {

	class Evaluate {
	public:
		Evaluate();
		int evaluatePosition(ChessBoard& chessBoard);
		int quiescenceMax(ChessBoard& chessBoard, int alpha, int beta, unsigned long long h, TranspositionTable& tt, int runningScore);
		int quiescenceMin(ChessBoard& chessBoard, int alpha, int beta, unsigned long long h, TranspositionTable& tt, int runningScore);
		int seeMax(int r, int c, ChessBoard& chessBoard);
		int seeMin(int r, int c, ChessBoard& chessBoard);
		int seeCapture(Move& move, ChessBoard& chessBoard);
		int pieceValues[7];
		std::pair<std::vector<std::vector<int>>, std::vector<std::vector<int>>> pieceTables[6];

	private:
		int boardSize;

		std::vector<std::vector<int>> pawnActivityScoreWhite;
		std::vector<std::vector<int>> knightActivityScoreWhite;
		std::vector<std::vector<int>> bishopActivityScoreWhite;
		std::vector<std::vector<int>> rookActivityScoreWhite;
		std::vector<std::vector<int>> queenActivityScoreWhite;
		std::vector<std::vector<int>> kingActivityScoreWhite;

		std::vector<std::vector<int>> pawnActivityScoreBlack;
		std::vector<std::vector<int>> knightActivityScoreBlack;
		std::vector<std::vector<int>> bishopActivityScoreBlack;
		std::vector<std::vector<int>> rookActivityScoreBlack;
		std::vector<std::vector<int>> queenActivityScoreBlack;
		std::vector<std::vector<int>> kingActivityScoreBlack;
	};

}

#endif