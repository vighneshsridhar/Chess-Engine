#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Rook.h"
#include "Move.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	Rook::Rook(PieceColor color) : color(color) {
	};

	std::vector<Move> Rook::getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece rook) {
		sf::Vector2f position = rook.getPosition();
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s = r + 1;
		int t = c;

		while (s < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move = { position, y, -1, rook, chessBoard[s][t] };
			moves.push_back(move);

			if (chessBoard[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s++;
		}
		s = r - 1;
		t = c;

		while (s >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move = { position, y, -1, rook, chessBoard[s][t] };
			moves.push_back(move);

			if (chessBoard[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			s--;
		}
		s = r;
		t = c + 1;

		while (t < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move = { position, y, -1, rook, chessBoard[s][t] };
			moves.push_back(move);

			if (chessBoard[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			t++;
		}
		s = r;
		t = c - 1;

		while (t >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			Move move = { position, y, -1, rook, chessBoard[s][t] };
			moves.push_back(move);

			if (chessBoard[s][t].getPieceType() != PieceType::EMPTY) {
				break;
			}
			t--;
		}

		return moves;
	}
}