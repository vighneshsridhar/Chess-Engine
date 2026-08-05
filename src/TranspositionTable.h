#ifndef ChessGame_TranspositionTable_h
#define ChessGame_TranspositionTable_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <random>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Move.h"

namespace ChessGame {

	class TranspositionTable {
	public:
		TranspositionTable();

		enum class TTFlag {
			EXACT_EVAL,
			LOWER_BOUND,
			UPPER_BOUND
		};

		struct TTEntry {
			int eval;
			int depth;
			TTFlag flag = TTFlag::EXACT_EVAL;
			Move bestMove;
		};
		unsigned long long updateHash(Move& move, bool push, ChessBoard& chessBoard, unsigned long long h);
		unsigned long long getInitialH(ChessBoard& chessBoard);
		TTEntry& getTT(unsigned long long h);
		void updateTT(unsigned long long h, TTEntry& entry);
		void incrementCount(unsigned long long h);
		void decrementCount(unsigned long long h);
		int getPositionCount(unsigned long long h);

	private:
		std::vector<std::vector<unsigned long long>> hashTable;
		std::unordered_map<unsigned long long, TTEntry> tt;
		std::unordered_map<unsigned long long, int> count;
		int boardSize;
		std::stack<std::vector<bool>> castlingRights;
		std::stack<int> enPassantFiles;
	};
}

#endif
