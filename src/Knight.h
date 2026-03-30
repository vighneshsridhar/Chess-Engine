#pragma once
#ifndef ChessGame_Knight_h
#define ChessGame_Knight_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Knight {
	public:
		Knight(PieceColor color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece knight);

	private:
		PieceColor color;
	};

}

#endif