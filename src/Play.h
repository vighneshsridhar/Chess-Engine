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
#include "Move.h"

namespace ChessGame {

	class Play {
	public:
		Play();
		void draw(sf::RenderWindow& window, float squareSize, std::vector<std::vector<ChessPiece>> b, std::vector<std::vector<sf::Sprite>>& spritesBoard) const;
		void undoMove(ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move::MoveNode* n, std::vector<std::vector<sf::Sprite>> sprites) const;
		bool makeMove(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, Move::MoveNode* n,
			std::vector<std::vector<sf::Sprite>>& promotionSprites, std::vector<Move*>& legalMoves, PGN& m) const;
		void playGame();

	private:
		int boardSize;
		TextureManager t;
		PGN m;
		std::unordered_map<std::string, sf::Texture> textures;
	};

}

#endif