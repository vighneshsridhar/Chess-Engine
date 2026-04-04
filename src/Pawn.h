#pragma once
#ifndef ChessGame_Pawn_h
#define ChessGame_Pawn_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "ChessBoard.h"
#include "Functions.h"

namespace ChessGame {

	class Pawn{
	public:
		Pawn(PieceColor color);
		std::vector<Move> getMoves(ChessBoard b, ChessPiece pawn);

	private:
		PieceColor color;
	};
}

#endif