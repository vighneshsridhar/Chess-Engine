#pragma once
#ifndef ChessGame_Play_h
#define ChessGame_Play_h

#include <SFML/Graphics.hpp>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include "TextureManager.h"
#include "PromotionClicker.h"
#include "PGN.h"

namespace ChessGame {

	class Play {
	public:
		Play();
		void draw(sf::RenderWindow& window, float squareSize, std::vector<std::vector<ChessPiece>> chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard) const;
		void undoMove(ChessBoard& b, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move move, std::vector<std::vector<sf::Sprite>> promotionSprites, std::vector<sf::Sprite> pawnSprites) const;
		bool makeMove(sf::RenderWindow& window, ChessBoard& b, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move move,
			std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move>& legalMoves) const;
		void playGame();
		PGN m;

	private:
		int boardSize;
		TextureManager t;
		std::unordered_map<std::string, sf::Texture> textures;
	};

}

#endif