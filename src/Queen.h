#pragma once
#ifndef ChessGame_Queen_h
#define ChessGame_Queen_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Queen {
	public:
		Queen(std::string color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece queen);

	private:
		std::string color;
	};

}

#endif
