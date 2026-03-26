#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Rook.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	Rook::Rook(std::string color) : color(color) {
	};

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Rook::getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece rook) {
		sf::Vector2f position = rook.getPosition();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s = r + 1;
		int t = c;

		while (s < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s++;
		}
		s = r - 1;
		t = c;

		while (s >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s--;
		}
		s = r;
		t = c + 1;

		while (t < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			t++;
		}
		s = r;
		t = c - 1;

		while (t >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			t--;
		}

		return moves;
	}
}