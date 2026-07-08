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
		Engine(int maxDepth);
		Move iterative_deepening(ChessBoard chessBoard);
		int alphaBetaMax(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore);
		int alphaBetaMin(ChessBoard& chessBoard, int alpha, int beta, int depthLeft, unsigned long long h, int runningScore);

	private:
		int boardSize;
		int maxDepth;
		TranspositionTable tt;
		unsigned long long h;
		std::vector<std::vector<Move>> killerMoves;
		Evaluate e;
	};

}

#endif