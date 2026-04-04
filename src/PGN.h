#pragma once
#ifndef ChessGame_PGN_h
#define ChessGame_PGN_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "Functions.h"
#include "Move.h"
#include "ChessPiece.h"
#include "ChessBoard.h"

namespace ChessGame {

	class PGN {
	public:
		PGN();
		std::string convertSquareToPGN(int r, int c) const;
		std::string convertMoveToPGN(Move move, bool isCapture, ChessBoard b) const;
		void printMove(std::string ans, ChessBoard b, size_t numLegalMoves) const;
		void incrementMove();
		void decrementMove();

	private:
		int moveNumber;
	};
}

#endif