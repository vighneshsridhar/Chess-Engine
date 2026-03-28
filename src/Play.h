#pragma once
#ifndef ChessGame_Play_h
#define ChessGame_Play_h

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include "PromotionClicker.h"

namespace ChessGame {

	class Play {
	public:
		Play();
		void playGame();

	private:
		int boardSize;
	};

}

#endif