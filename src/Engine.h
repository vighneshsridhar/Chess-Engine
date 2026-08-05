#ifndef ChessGame_Engine_h
#define ChessGame_Engine_h


#include <SFML/Graphics.hpp>
#include <unordered_set>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Move.h"
#include "TranspositionTable.h"
#include "Evaluate.h"


namespace ChessGame {

	class Engine {
	public:
		Engine(int maxDepth, ChessBoard chessBoard);
		Move iterative_deepening(ChessBoard chessBoard);
		int alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore);
		int alphaBetaMin(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore);
		void enginePush(Move move, ChessBoard& chessBoard);
		void engineUnmakeMove(Move move, ChessBoard& chessBoard);

	private:
		int boardSize;
		int maxDepth;
		TranspositionTable tt;
		std::vector<std::vector<Move>> killerMoves;
		Evaluate e;
		std::stack<Move> bestMoves;
		std::stack<unsigned long long> prevH;
	};

}

#endif