#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "King.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	King::King(std::string color) : color(color) {
	};

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> King::getMoves(std::vector<std::vector<ChessPiece>> chessBoard, ChessPiece king) {
		sf::Vector2f position = king.getPosition();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);

		for (int s = r - 1; s <= r + 1; s++) {

			if (s < 0 || s >= boardSize) {
				continue;
			}
			for (int t = c - 1; t <= c + 1; t++) {

				if (t < 0 || t >= boardSize) {
					continue;
				}

				if (chessBoard[s][t].getColor() != chessBoard[r][c].getColor()) {
					y = Functions::convertToPosition(s, t);
					moves.push_back(std::make_pair(position, y));
				}
			}
		}

		if (!king.pieceHasMoved()) {

			if (king.getColor() == "WHITE") {

				if (!chessBoard[7][7].pieceHasMoved() && chessBoard[7][5].getPieceType() == "EMPTY_SQUARE" && chessBoard[7][6].getPieceType() == "EMPTY_SQUARE") {
					y = Functions::convertToPosition(7, 6);
					moves.push_back(std::make_pair(position, y));
				}

				if (!chessBoard[7][0].pieceHasMoved() && chessBoard[7][1].getPieceType() == "EMPTY_SQUARE" && chessBoard[7][2].getPieceType() == "EMPTY_SQUARE" && chessBoard[7][3].getPieceType() == "EMPTY_SQUARE"){
					y = Functions::convertToPosition(7, 2);
					moves.push_back(std::make_pair(position, y));
				}
			}

			if (king.getColor() == "BLACK") {

				if (!chessBoard[0][7].pieceHasMoved() && chessBoard[0][5].getPieceType() == "EMPTY_SQUARE" && chessBoard[0][6].getPieceType() == "EMPTY_SQUARE") {
					y = Functions::convertToPosition(0, 6);
					moves.push_back(std::make_pair(position, y));
				}

				if (!chessBoard[0][0].pieceHasMoved() && chessBoard[0][1].getPieceType() == "EMPTY_SQUARE" && chessBoard[0][2].getPieceType() == "EMPTY_SQUARE" && chessBoard[0][3].getPieceType() == "EMPTY_SQUARE") {
					y = Functions::convertToPosition(0, 2);
					moves.push_back(std::make_pair(position, y));
				}
			}
		}

		return moves;
	}
}