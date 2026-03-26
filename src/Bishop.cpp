#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Bishop.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	Bishop::Bishop(std::string color) : color(color) {
	};

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Bishop::getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece bishop) {
		sf::Vector2f position = bishop.getPosition();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s = r + 1;
		int t = c + 1;

		while (s < boardSize && t < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s++;
			t++;
		}
		s = r + 1;
		t = c - 1;

		while (s < boardSize && t >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s++;
			t--;
		}
		s = r - 1;
		t = c + 1;

		while (s >= 0 && t < boardSize && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s--;
			t++;
		}
		s = r - 1;
		t = c - 1;

		while (s >= 0 && t >= 0 && chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
			y = Functions::convertToPosition(s, t);
			moves.push_back(std::make_pair(position, y));

			if (chessBoard[s][t].getPieceType() != "EMPTY_SQUARE") {
				break;
			}
			s--;
			t--;
		}

		return moves;
	}
}