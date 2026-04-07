#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <bitset>

#include "Bitboard.h"
#include "Move.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {
	Bitboard::Bitboard() {
		numSquares = 64;
		boardSize = 8;
		squaresAttacked = { 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0 };
	};

	void Bitboard::updatePawnAttackSquares(ChessBoard chessBoard) {
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		int square1;
		int square2;

		for (int r1 = 0; r1 < boardSize; r1++) {

			for (int c1 = 0; c1 < boardSize; c1++) {
				square1 = r1 * 8 + c1;
				long long mask = (static_cast<long long>(1) << (numSquares - 1 - square1));

				if (b[r1][c1].getPieceType() != PieceType::PAWN || (chessBoard.getTurn() && b[r1][c1].getColor() != PieceColor::WHITE) || 
					(!chessBoard.getTurn() && b[r1][c1].getColor() != PieceColor::BLACK) || r1 == 0 || r1 == 7) {
					continue;
				}

				if (b[r1][c1].getColor() == PieceColor::WHITE) {

					if (c1 == 0) {
						square2 = (r1 - 1) * 8 + 1;
						squaresAttacked[square2] |= mask;
					}

					else if (c1 == 7) {
						square2 = (r1 - 1) * 8 + 6;
						squaresAttacked[square2] |= mask;
					}

					else {
						square2 = (r1 - 1) * 8 + c1 + 1;
						squaresAttacked[square2] |= mask;
						square2 = (r1 - 1) * 8 + c1 - 1;
						squaresAttacked[square2] |= mask;
					}
				}

				if (b[r1][c1].getColor() == PieceColor::BLACK) {

					if (c1 == 0) {
						square2 = (r1 + 1) * 8 + 1;
						squaresAttacked[square2] |= mask;
					}

					else if (c1 == 7) {
						square2 = (r1 + 1) * 8 + 6;
						squaresAttacked[square2] |= mask;
					}

					else {
						square2 = (r1 + 1) * 8 + c1 + 1;
						squaresAttacked[square2] |= mask;
						square2 = (r1 + 1) * 8 + c1 - 1;
						squaresAttacked[square2] |= mask;
					}
				}
			}
		}
	}

	std::vector<long long> Bitboard::getBitboard() {
		return squaresAttacked;
	}

	void Bitboard::resetBitboard() {
		squaresAttacked = { 0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0,
							0, 0, 0, 0, 0, 0, 0, 0 };
	}

	void Bitboard::updateBitboard(ChessBoard chessBoard, bool castling) {
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		resetBitboard();
		std::vector<Move*> moves;

		long long mask;
		int square1;
		int square2;

		for (int r = 0; r < boardSize; r++) {

			for (int c = 0; c < boardSize; c++) {

				if (b[r][c].getPieceType() == PieceType::PAWN || (chessBoard.getTurn() && b[r][c].getColor() != PieceColor::WHITE) || (!chessBoard.getTurn() && b[r][c].getColor() != PieceColor::BLACK)) {
					continue;
				}
				moves = chessBoard.getPieceMoves(b[r][c]);

				for (const auto& move : moves) {
					auto position1 = move->getInitialSquare();
					auto position2 = move->getEndSquare();
					auto [r1, c1] = Functions::convertToSquare(position1);
					auto [r2, c2] = Functions::convertToSquare(position2);
					square1 = Functions::convertToNumber(position1);
					square2 = Functions::convertToNumber(position2);
					mask = (static_cast<long long>(1) << (numSquares - 1 - square1));
					squaresAttacked[square2] |= mask;
				}
			}
		}
		updatePawnAttackSquares(chessBoard);

		return;
	}

	bool Bitboard::isValidBoard(ChessBoard chessBoard, bool castling) {
		std::pair<int, int> kingPosition = chessBoard.getKingPosition();
		int num = kingPosition.first * 8 + kingPosition.second;
		int castlingSquare;

		if (castling) {

			if (chessBoard.getTurn()) {
				
				if (num == 62) {
					castlingSquare = 61;
				}

				else {
					castlingSquare = 59;
				}
			}

			else {

				if (num == 6) {
					castlingSquare = 5;
				}

				else {
					castlingSquare = 3;
				}
			}

			return squaresAttacked[num] == 0 && squaresAttacked[castlingSquare] == 0;
		}
		
		return squaresAttacked[num] == 0;
	}
}