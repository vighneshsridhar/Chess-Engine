#pragma once
#ifndef ChessGame_Bishop_h
#define ChessGame_Bishop_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Bishop {
	public:
		Bishop(PieceColor color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece bishop);

	private:
		PieceColor color;
	};

}

#endif