#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <iostream>

#include "Pawn.h"
#include "Move.h"
#include "ChessBoard.h"
#include "Functions.h"


namespace ChessGame {

	Pawn::Pawn(PieceColor color): color(color) {
	};

	std::vector<Move> Pawn::getMoves(ChessBoard& chessBoard, ChessPiece& pawn) {
		PieceColor color = pawn.getColor();
		PieceColor enemy = color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;
		int boardSize = 8;

		std::vector<Move> moves;
		auto [r, c] = pawn.getCoordinates();
		int dr = color == PieceColor::WHITE ? -1 : 1;
		int startRow = color == PieceColor::WHITE ? 6 : 1;
		int promotionRow = color == PieceColor::WHITE ? 1 : 6;
		int pawnDirs[3][2] = { {dr, 0}, {dr, 1}, {dr, -1} };

		int enPassantFile = chessBoard.getEnPassantFile();

		int s;
		int t;

		for (auto& d : pawnDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto piece1 = chessBoard.pieceAt(s, t);

				if (t == c) {

					if (piece1.getPieceType() == PieceType::EMPTY) {
						Move move(r, c, s, t, pawn, piece1);

						if (s != promotionRow) {
							moves.push_back(move);
						}
						
						else {
							promotion(move, color, moves);
						}

						if (r == startRow) {
							auto piece2 = chessBoard.pieceAt(s + d[0], t);

							if (piece2.getPieceType() == PieceType::EMPTY) {
								Move move(r, c, s + d[0], t, pawn, piece2);
								moves.push_back(move);
							}
						}
					}
				}

				else {
					auto piece3 = chessBoard.pieceAt(s, t);

					if (piece3.getColor() == enemy) {
						Move move(r, c, s, t, pawn, piece3);

						if (s != promotionRow) {
							moves.push_back(move);
						}

						else {
							promotion(move, color, moves);
						}
					}

					if (enPassantFile != -1 && r == startRow + dr * 3) {

						if (enPassantFile == c + 1) {
							Move move(r, c, s, c + 1, pawn, chessBoard.pieceAt(s, c + 1));
							moves.push_back(move);
						}

						if (enPassantFile == c - 1) {
							Move move(r, c, s, c - 1, pawn, chessBoard.pieceAt(s, c - 1));
							moves.push_back(move);
						}
					}
				}
			}
		}

		return moves;
	}

	void Pawn::promotion(Move move, PieceColor color, std::vector<Move>& moves) {
		auto [r, c] = move.getEndSquare();
		ChessPiece knight(PieceType::KNIGHT, color, r, c);
		ChessPiece bishop(PieceType::BISHOP, color, r, c);
		ChessPiece rook(PieceType::ROOK, color, r, c);
		ChessPiece queen(PieceType::QUEEN, color, r, c);

		move.setPromotionPiece(knight);
		moves.push_back(move);
		move.setPromotionPiece(bishop);
		moves.push_back(move);
		move.setPromotionPiece(rook);
		moves.push_back(move);
		move.setPromotionPiece(queen);
		moves.push_back(move);

		return;
	}

	std::vector<Move> Pawn::getCaptures(ChessBoard& chessBoard, ChessPiece& pawn) {
		PieceColor color = pawn.getColor();
		PieceColor enemy = color == PieceColor::WHITE ? PieceColor::BLACK : PieceColor::WHITE;

		std::vector<Move> captures;
		int boardSize = 8;
		auto [r, c] = pawn.getCoordinates();
		int dr = color == PieceColor::WHITE ? -1 : 1;
		int startRow = color == PieceColor::WHITE ? 6 : 1;
		int promotionRow = color == PieceColor::WHITE ? 1 : 6;
		int pawnDirs[2][2] = { {dr, 1}, {dr, -1} };

		int enPassantFile = chessBoard.getEnPassantFile();

		int s;
		int t;

		for (auto& d : pawnDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto piece1 = chessBoard.pieceAt(s, t);

				if (piece1.getColor() == enemy) {
					Move move(r, c, s, t, pawn, piece1);

					if (s != promotionRow) {
						captures.push_back(move);
					}

					else {
						promotion(move, color, captures);
					}
				}

				if (enPassantFile != -1 && r == startRow + dr * 3) {

					if (enPassantFile == c + 1) {
						Move move(r, c, s, c + 1, pawn, chessBoard.pieceAt(s, c + 1));
						captures.push_back(move);
					}

					if (enPassantFile == c - 1) {
						Move move(r, c, s, c - 1, pawn, chessBoard.pieceAt(s, c - 1));
						captures.push_back(move);
					}
				}
			}
		}

		return captures;
	}
}