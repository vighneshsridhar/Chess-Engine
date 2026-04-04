#pragma once
#ifndef ChessGame_King_h
#define ChessGame_King_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "Move.h"
#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class King {
	public:
		King(PieceColor color);
		std::vector<Move> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece king);

	private:
		PieceColor color;
	};

}

#endif
