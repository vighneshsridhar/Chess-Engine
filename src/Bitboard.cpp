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
	};

	bool Bitboard::isValidBoard(ChessBoard& chessBoard) {
		auto [r, c] = chessBoard.getKingPosition();
		ChessPiece kg = chessBoard.pieceAt(r, c);
		PieceColor color = kg.getColor();
		sf::Vector2f kingPosition = Functions::convertToPosition(r, c);
		ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, r, c);

		if (kingAttacked(chessBoard, r, c, color)) {
			chessBoard.setPieceAt(kg, r, c);
			return false;
		}

		return true;
	}

	bool Bitboard::kingAttacked(ChessBoard& chessBoard, int r, int c, PieceColor color) {
		PieceColor enemy = color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
		std::vector<std::vector<int>> pawnDirs;
		int boardSize = 8;

		if (color == PieceColor::WHITE) {
			pawnDirs = { { 1, -1 }, { 1, 1 } };
		}

		else {
			pawnDirs = { { -1, -1 }, {-1, 1} };
		}
		int s = r;
		int t = c;

		for (auto& d : pawnDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy && p.getPieceType() == PieceType::PAWN){
					return true;
				}
			}
		}
		std::vector<std::vector<int>> knightDirs = { {-1, 2}, {1, 2}, {1, 2}, {1, 2}, {2, 1}, {2, 1}, {2, 1}, {2, 1} };

		for (const auto& d: knightDirs) {
			s = r + d[0];
			t = c + d[1];
			
			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy && p.getPieceType() == PieceType::KNIGHT) {
					return true;
				}
			}
		}
		static int bishopDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

		for (auto& d : bishopDirs) {
			s = r + d[0];
			t = c + d[1];
			
			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == color) {
					break;
				}

				if (p.getColor() == enemy) {

					if (p.getPieceType() == PieceType::BISHOP || p.getPieceType() == PieceType::QUEEN) {
						return true;
					}
					break;
				}
				s += d[0];
				t += d[1];
			}
		}
		static int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

		for (auto& d : rookDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == color) {
					break;
				}

				if (p.getColor() == enemy) {

					if (p.getPieceType() == PieceType::ROOK || p.getPieceType() == PieceType::QUEEN) {
						return true;
					}
					break;
				}
				s += d[0];
				t += d[1];
			}
		}

		return false;
	}
}