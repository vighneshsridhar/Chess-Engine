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
		boardSize = 8;
	};

	bool Bitboard::isValidBoard(ChessBoard& chessBoard) {
		chessBoard.changeTurn();
		auto [r, c] = chessBoard.getKingPosition();
		PieceColor color = chessBoard.whiteTurn() ? PieceColor::WHITE : PieceColor::BLACK;

		if (kingAttacked(chessBoard, r, c, color)) {
			chessBoard.changeTurn();
			return false;
		}
		chessBoard.changeTurn();

		return true;
	}

	bool Bitboard::kingAttacked(ChessBoard& chessBoard, int r, int c, PieceColor color) {
		PieceColor enemy = color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
		int boardSize = 8;
		int dr = color == PieceColor::WHITE ? -1 : 1;
		int pawnDirs[2][2] = { {dr, -1}, {dr, 1} };

		int s = r;
		int t = c;

		for (auto& d : pawnDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy && p.getPieceType() == PieceType::PAWN) {
					return true;
				}
			}
		}
		int knightDirs[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

		for (const auto& d : knightDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy && p.getPieceType() == PieceType::KNIGHT) {
					return true;
				}
			}
		}
		int bishopDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

		for (const auto& d : bishopDirs) {
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
		int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

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
		int kingDirs[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

		for (auto& d : kingDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy && p.getPieceType() == PieceType::KING) {
					return true;
				}
			}
		}

		return false;
	}

	ChessPiece& Bitboard::getSmallestAttacker(ChessBoard& chessBoard, int r, int c, PieceColor side) {
		PieceColor enemy = side == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
		int boardSize = 8;
		ChessPiece ans;
		int dr = side == PieceColor::WHITE ? -1 : 1;
		int pawnDirs[2][2] = { {dr, -1}, {dr, 1} };
		ChessPiece p;

		int s = r;
		int t = c;

		for (auto& d : pawnDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				p = chessBoard.pieceAt(s, t);

				if (p.getColor() == side && p.getPieceType() == PieceType::PAWN) {
					return p;
				}
			}
		}
		int kingDirs[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

		for (auto& d : kingDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				p = chessBoard.pieceAt(s, t);

				if (p.getColor() == side && p.getPieceType() == PieceType::KING) {
					return p;
				}
			}
		}
		int knightDirs[8][2] = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};

		for (const auto& d : knightDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				p = chessBoard.pieceAt(s, t);

				if (p.getColor() == side && p.getPieceType() == PieceType::KNIGHT) {
					return p;
				}
			}
		}
		int bishopDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };

		for (const auto& d : bishopDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy) {
					break;
				}

				if (p.getColor() == side) {

					if (p.getPieceType() == PieceType::BISHOP) {
						return p;
					}

					else if (p.getPieceType() == PieceType::QUEEN) {
						ans = p;
					}
				}
				s += d[0];
				t += d[1];
			}
		}
		int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

		for (auto& d : rookDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				p = chessBoard.pieceAt(s, t);

				if (p.getColor() == enemy) {
					break;
				}

				if (p.getColor() == side) {

					if (p.getPieceType() == PieceType::ROOK) {
						return p;
					}

					else if (p.getPieceType() == PieceType::QUEEN) {
						ans = p;
					}
					break;
				}
				s += d[0];
				t += d[1];
			}
		}

		return ans;
	}
}