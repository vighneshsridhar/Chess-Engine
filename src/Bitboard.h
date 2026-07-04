#pragma once
#ifndef ChessGame_Bitboard_h
#define ChessGame_Bitboard_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include "Functions.h"

namespace ChessGame {

	class Bitboard {
	public:
		Bitboard();
		struct GMagic {
			long long mask;
			long long magic;
		};
		static bool isValidBoard(ChessBoard chessBoard);
		static bool kingAttacked(ChessBoard chessBoard, int r, int c, PieceColor color);

	private:
		int numSquares;
		int boardSize;
		std::vector<long long> squaresAttacked;
	};

}

#endif