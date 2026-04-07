#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "Knight.h"
#include "Move.h"
#include "ChessPiece.h"
#include "Functions.h"

namespace ChessGame {

	Knight::Knight(PieceColor color) : color(color) {
	};

	std::vector<Move*> Knight::getMoves(ChessBoard chessBoard, ChessPiece knight) {
		sf::Vector2f position = knight.getPosition();
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		std::vector<Move*> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = Functions::convertToSquare(position);
		int s;
		int t;
		std::vector<std::vector<int>> squares = { {r - 1, c + 2}, {r - 1, c - 2}, {r + 1, c + 2}, {r + 1, c - 2}, {r - 2, c + 1}, {r - 2, c - 1}, {r + 2, c + 1}, {r + 2, c - 1} };

		for (const auto& square : squares) {
			s = square[0];
			t = square[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize && b[s][t].getColor() != b[r][c].getColor()) {
				y = Functions::convertToPosition(s, t);
				Move* move = new Move(position, y, -1, knight, b[s][t]);
				moves.push_back(move);
			}
		}

		return moves;
	}
}