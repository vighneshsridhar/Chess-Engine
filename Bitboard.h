#pragma once
#ifndef ChessGame_Bitboard_h
#define ChessGame_Bitboard_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Bitboard {
	public:
		Bitboard();
		void updatePawnAttackSquares(ChessBoard b);
		std::vector<long long> getBitboard();
		void resetBitboard();
		void updateBitboard(ChessBoard b, bool castling);
		bool isValidBoard(ChessBoard b, bool castling);

	private:
		int numSquares;
		int boardSize;
		std::vector<long long> squaresAttacked;
	};

}

#endif