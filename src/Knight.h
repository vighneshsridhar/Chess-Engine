#pragma once
#ifndef ChessGame_Knight_h
#define ChessGame_Knight_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Move.h"
#include "Functions.h"

namespace ChessGame {

	class Knight {
	public:
		Knight(PieceColor color);
		std::vector<Move> getMoves(ChessBoard chessBoard, ChessPiece knight);

	private:
		PieceColor color;
	};

}

#endif