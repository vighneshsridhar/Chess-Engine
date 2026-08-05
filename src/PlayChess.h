#pragma once
#ifndef ChessGame_PlayChess_h
#define ChessGame_PlayChess_h

#include <SFML/Graphics.hpp>

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include "TextureManager.h"
#include "PromotionClicker.h"
#include "PGN.h"
#include "Move.h"
#include "Bitboard.h"
#include "Engine.h"
#include "Button.h"
#include "Position.h"

namespace ChessGame {

	class PlayChess {
	public:
		PlayChess();
		void draw(sf::RenderWindow& window, Button& playButton, bool playMode, Button& changePositionButton, bool changePositionMode, 
			Button& engineButton, bool engineMode, float squareSize, std::vector<std::vector<ChessPiece>> b, std::vector<std::vector<sf::Sprite>>& spritesBoard, 
			int initial_r, int initial_c) const;
		void printMove(Move* move, MoveNode*& root, MoveNode* orig_root, MoveNode* n);
		void playGame();

	private:
		int boardSize;
		ChessBoard startingPosition;
		PGN m;
	};

}

#endif