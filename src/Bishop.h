#pragma once
#ifndef ChessGame_Bishop_h
#define ChessGame_Bishop_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "Move.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Bishop {
	public:
		Bishop(PieceColor color);
		static std::vector<Move> getMoves(ChessBoard& chessBoard, ChessPiece& bishop);

	private:
		PieceColor color;
	};

}

#endif