#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Bishop.h"
#include "Move.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Functions.h"


namespace ChessGame {

	Bishop::Bishop(PieceColor color) : color(color) {
	};

	std::vector<Move> Bishop::getMoves(ChessBoard chessBoard, ChessPiece bishop) {
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		sf::Vector2f position = bishop.getPosition();
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s = r + 1;
		int t = c + 1;

		while (s < boardSize && t < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move(position, y, -1, bishop, b[s][t]);
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
			Move move(position, y, -1, bishop, b[s][t]);
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
			Move move(position, y, -1, bishop, b[s][t]);
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
			Move move(position, y, -1, bishop, b[s][t]);
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