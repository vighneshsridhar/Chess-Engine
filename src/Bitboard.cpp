#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Bitboard.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include <iostream>
#include <bitset>

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

	void Bitboard::updatePawnAttackSquares(ChessBoard b) {
		std::vector<std::vector<ChessPiece>> chessBoard = b.getChessBoard();
		int square1;
		int square2;

		for (int r1 = 0; r1 < boardSize; r1++) {

			for (int c1 = 0; c1 < boardSize; c1++) {
				square1 = r1 * 8 + c1;
				long long mask = (static_cast<long long>(1) << (numSquares - 1 - square1));

				if (chessBoard[r1][c1].getPieceType() != "PAWN" || (b.getTurn() && chessBoard[r1][c1].getColor() != "WHITE") || 
					(!b.getTurn() && chessBoard[r1][c1].getColor() != "BLACK") || r1 == 0 || r1 == 7) {
					continue;
				}

				if (chessBoard[r1][c1].getColor() == "WHITE") {

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

				if (chessBoard[r1][c1].getColor() == "BLACK") {

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

	void Bitboard::updateBitboard(ChessBoard b, bool castling) {
		std::vector<std::vector<ChessPiece>> chessBoard = b.getChessBoard();
		resetBitboard();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;

		long long mask;
		int square1;
		int square2;

		for (int r = 0; r < boardSize; r++) {

			for (int c = 0; c < boardSize; c++) {

				if (chessBoard[r][c].getPieceType() == "PAWN" || (b.getTurn() && chessBoard[r][c].getColor() != "WHITE") || (!b.getTurn() && chessBoard[r][c].getColor() != "BLACK")) {
					continue;
				}
				moves = b.getPieceMoves(chessBoard[r][c]);

				for (const auto& move : moves) {
					auto& [position1, position2] = move;
					auto [r1, c1] = Functions::convertToSquare(position1);
					auto [r2, c2] = Functions::convertToSquare(position2);
					square1 = Functions::convertToNumber(position1);
					square2 = Functions::convertToNumber(position2);
					// :cout << "r1 = " << r1 << " r2 = " << r2 << " c1 = " << c1 << " c2 = " << c2 << chessBoard[r1][c1].getPieceType() << std::endl;
					mask = (static_cast<long long>(1) << (numSquares - 1 - square1));
					squaresAttacked[square2] |= mask;
				}
			}
		}
		updatePawnAttackSquares(b);

		/*for (int r = 0; r < boardSize; r++) {

			for (int c = 0; c < boardSize; c++) {
				square1 = r * 8 + c;
				std::uint64_t u_value = static_cast<std::uint64_t>(squaresAttacked[square1]);

				// Create a bitset of 64 bits and print it directly
				std::cout << "square1 = " << square1 << " and Binary representation : " << std::bitset<64>(u_value) << std::endl;
			}
		} */

		return;
	}

	bool Bitboard::isValidBoard(ChessBoard b, bool castling) {
		std::pair<int, int> kingPosition = b.getKingPosition();
		int num = kingPosition.first * 8 + kingPosition.second;
		int castlingSquare;

		if (castling) {

			if (b.getTurn()) {
				
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