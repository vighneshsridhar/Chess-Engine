#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Queen.h"
#include "Move.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	Queen::Queen(PieceColor color) : color(color) {
	};

	std::vector<Move> Queen::getMoves(ChessBoard chessBoard, ChessPiece queen) {
		sf::Vector2f position = queen.getPosition();
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s = r + 1;
		int t = c;

		while (s < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s++;
		}
		s = r - 1;
		t = c;

		while (s >= 0 && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s--;
		}
		s = r;
		t = c + 1;

		while (t < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			t++;
		}
		s = r;
		t = c - 1;

		while (t >= 0 && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			t--;
		}
		s = r + 1;
		t = c + 1;

		while (s < boardSize && t < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s++;
			t++;
		}
		s = r + 1;
		t = c - 1;

		while (s < boardSize && t >= 0 && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s++;
			t--;
		}
		s = r - 1;
		t = c + 1;

		while (s >= 0 && t < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);

			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s--;
			t++;
		}
		s = r - 1;
		t = c - 1;

		while (s >= 0 && t >= 0 && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, queen, b[s][t]);
			moves.push_back(move);
			
			if (b[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s--;
			t--;
		}

		return moves;
	}
}