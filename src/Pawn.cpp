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

	std::vector<Move> Pawn::getMoves(ChessBoard chessBoard, ChessPiece pawn) {
		std::vector<std::vector<ChessGame::ChessPiece>> b = chessBoard.getChessBoard();
		sf::Vector2f position = pawn.getPosition();
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int enPassantFile = chessBoard.getEnPassantFile();
		
		if (pawn.getColor() == PieceColor::WHITE) {
			
			if (r > 0 && b[r - 1][c].getPieceType() == PieceType::EMPTY) {
				y = Functions::convertToPosition(r - 1, c);
				Move move(position, y, -1, pawn, b[r - 1][c]);
				moves.push_back(move);

				if (r == 6 && b[r - 2][c].getPieceType() == PieceType::EMPTY) {
					y = Functions::convertToPosition(r - 2, c);
					Move move(position, y, -1, pawn, b[r - 2][c]);
					moves.push_back(move);
				}
			}

			if (r > 0 && c - 1 >= 0 && b[r - 1][c - 1].getColor() == PieceColor::BLACK) {
				y = Functions::convertToPosition(r - 1, c - 1);
				Move move(position, y, -1, pawn, b[r - 1][c - 1]);
				moves.push_back(move);
			}

			if (r > 0 && c + 1 < boardSize && b[r - 1][c + 1].getColor() == PieceColor::BLACK) {
				y = Functions::convertToPosition(r - 1, c + 1);
				Move move(position, y, -1, pawn, b[r - 1][c + 1]);
				moves.push_back(move);
			}

			if (r == 3 && c + 1 == enPassantFile) {
				y = Functions::convertToPosition(r - 1, c + 1);
				Move move(position, y, -1, pawn, b[r - 1][c + 1]);
				moves.push_back(move);
			}

			if (r == 3 && c - 1 == enPassantFile) {
				y = Functions::convertToPosition(r - 1, c - 1);
				Move move(position, y, -1, pawn, b[r - 1][c - 1]);
				moves.push_back(move);
			}
		}

		if (pawn.getColor() == PieceColor::BLACK) {

			if (r < 7 && b[r + 1][c].getPieceType() == PieceType::EMPTY) {
				y = Functions::convertToPosition(r + 1, c);
				Move move(position, y, -1, pawn, b[r + 1][c]);
				moves.push_back(move);

				if (r == 1 && b[r + 2][c].getPieceType() == PieceType::EMPTY) {
					y = Functions::convertToPosition(r + 2, c);
					Move move(position, y, -1, pawn, b[r + 2][c]);
					moves.push_back(move);
				}
			}

			if (r < 7 && c - 1 >= 0 && b[r + 1][c - 1].getColor() == PieceColor::WHITE) {
				y = Functions::convertToPosition(r + 1, c - 1);
				Move move(position, y, -1, pawn, b[r + 1][c - 1]);
				moves.push_back(move);
			}

			if (r < 7 && c + 1 < boardSize && b[r + 1][c + 1].getColor() == PieceColor::WHITE) {
				y = Functions::convertToPosition(r + 1, c + 1);
				Move move(position, y, -1, pawn, b[r + 1][c + 1]);
				moves.push_back(move);
			}

			if (r == 4 && c + 1 == enPassantFile) {
				y = Functions::convertToPosition(r + 1, c + 1);
				Move move(position, y, -1, pawn, b[r + 1][c + 1]);
				moves.push_back(move);
			}

			if (r == 4 && c - 1 == enPassantFile) {
				y = Functions::convertToPosition(r + 1, c - 1);
				Move move(position, y, -1, pawn, b[r + 1][c - 1]);
				moves.push_back(move);
			}
		}
		return moves;
	}
}