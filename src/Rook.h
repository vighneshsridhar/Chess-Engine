#pragma once
#ifndef ChessGame_Rook_h
#define ChessGame_Rook_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Move.h"
#include "Functions.h"

namespace ChessGame {

	class Rook {
	public:
		Rook(PieceColor color);
		static std::vector<Move> getMoves(ChessBoard& chessBoard, ChessPiece& rook);
		static std::vector<Move> getCaptures(ChessBoard& chessBoard, ChessPiece& rook);

	private:
		PieceColor color;
		static int boardSize;
	};

}

#endif
