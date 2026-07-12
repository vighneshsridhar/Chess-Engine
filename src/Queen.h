#pragma once
#ifndef ChessGame_Queen_h
#define ChessGame_Queen_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Move.h"
#include "Functions.h"

namespace ChessGame {

	class Queen {
	public:
		Queen(PieceColor color);
		static std::vector<Move> getMoves(ChessBoard& chessBoard, ChessPiece& queen);
		static std::vector<Move> getCaptures(ChessBoard& chessBoard, ChessPiece& queen);

	private:
		PieceColor color;
	};

}

#endif
