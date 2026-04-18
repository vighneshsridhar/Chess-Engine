#ifndef ChessGame_TranspositionTable_h
#define ChessGame_TranspositionTable_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <random>

#include "ChessPiece.h"
#include "ChessBoard.h"

namespace ChessGame {

	class TranspositionTable {
	public:
		TranspositionTable();
		unsigned long long updateHash(Move move, unsigned long long h);
		unsigned long long getInitialH();

	private:
		std::vector<std::vector<unsigned long long>> hashTable;
	};
}

#endif
