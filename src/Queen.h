#pragma once
#ifndef ChessGame_Queen_h
#define ChessGame_Queen_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Move.h"
#include "Functions.h"

namespace ChessGame {

	class Queen {
	public:
		Queen(PieceColor color);
		std::vector<Move> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece queen);

	private:
		PieceColor color;
	};

}

#endif
