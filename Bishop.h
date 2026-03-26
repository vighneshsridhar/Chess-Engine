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
		Bishop(std::string color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece bishop);

	private:
		std::string color;
	};

}

#endif