#pragma once
#ifndef ChessGame_King_h
#define ChessGame_King_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class King {
	public:
		King(PieceColor color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece king);

	private:
		PieceColor color;
	};

}

#endif
