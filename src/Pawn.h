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
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece pawn);

	private:
		PieceColor color;
	};
}

#endif