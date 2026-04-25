#pragma once
#ifndef ChessGame_PromotionClicker_h
#define ChessGame_PromotionClicker_h


#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	class PromotionClicker {
	public:
		PromotionClicker();
		static std::pair<int, bool> promotePawn(sf::RenderWindow& window, std::vector<std::vector<ChessPiece>>& b,
			ChessPiece pawn, std::vector<std::vector<sf::Sprite>>& spritesBoard, std::vector<sf::Sprite>& sprites, int r, int c);
	};
}

#endif