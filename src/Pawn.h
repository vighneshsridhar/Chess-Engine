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
		static std::vector<Move> getMoves(ChessBoard& chessBoard, ChessPiece& pawn);
		static void promotion(Move move, PieceColor color, std::vector<Move>& moves);
		static std::vector<Move> getCaptures(ChessBoard& chessBoard, ChessPiece& pawn);

	private:
		PieceColor color;
		static int boardSize;
	};
}

#endif