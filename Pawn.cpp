#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "Pawn.h"
#include "ChessBoard.h"
#include "Functions.h"


namespace ChessGame {

	Pawn::Pawn(std::string color): color(color) {
	};

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> Pawn::getMoves(std::vector<std::vector<ChessGame::ChessPiece>> chessBoard, ChessPiece pawn) {
		sf::Vector2f position = pawn.getPosition();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		
		if (pawn.getColor() == "WHITE") {
			
			if (chessBoard[r - 1][c].getPieceType() == "EMPTY_SQUARE") {
				y = Functions::convertToPosition(r - 1, c);
				moves.push_back(std::make_pair(position, y));

				if (r == 6 && chessBoard[r - 2][c].getPieceType() == "EMPTY_SQUARE") {
					y = Functions::convertToPosition(r - 2, c);
					moves.push_back(std::make_pair(position, y));
				}
			}

			if (c - 1 >= 0 && chessBoard[r - 1][c - 1].getColor() == "BLACK") {
				y = Functions::convertToPosition(r - 1, c - 1);
				moves.push_back(std::make_pair(position, y));
			}

			if (r - 1 >= 0 && c + 1 < boardSize && chessBoard[r - 1][c + 1].getColor() == "BLACK") {
				y = Functions::convertToPosition(r - 1, c + 1);
				moves.push_back(std::make_pair(position, y));
			}
		}

		if (pawn.getColor() == "BLACK") {

			if (chessBoard[r + 1][c].getPieceType() == "EMPTY_SQUARE") {
				y = Functions::convertToPosition(r + 1, c);
				moves.push_back(std::make_pair(position, y));

				if (r == 1 && chessBoard[r + 2][c].getPieceType() == "EMPTY_SQUARE") {
					y = Functions::convertToPosition(r + 2, c);
					moves.push_back(std::make_pair(position, y));
				}
			}

			if (c - 1 >= 0 && chessBoard[r + 1][c - 1].getColor() == "WHITE") {
				y = Functions::convertToPosition(r + 1, c - 1);
				moves.push_back(std::make_pair(position, y));
			}

			if (c + 1 < boardSize && chessBoard[r + 1][c + 1].getColor() == "WHITE") {
				y = Functions::convertToPosition(r + 1, c + 1);
				moves.push_back(std::make_pair(position, y));
			}
		}
		return moves;
	}
}