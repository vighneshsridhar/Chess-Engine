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
		std::string convertSquareToPGN(int r, int c);
		std::string convertMoveToPGN(Move* move, size_t moveNumber, ChessBoard chessBoard, std::vector<Move> legalMoves);
		std::string generatePGN(MoveNode* root, ChessBoard chessBoard, int index);
		std::string checkOrCheckmate(ChessBoard chessBoard, size_t numLegalMoves);

	private:
		std::string pgn;
		int moveNumber;
	};
}

#endif