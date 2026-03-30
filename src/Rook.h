#pragma once
#ifndef ChessGame_Rook_h
#define ChessGame_Rook_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class Rook {
	public:
		Rook(PieceColor color);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece rook);

	private:
		PieceColor color;
	};

}

#endif
