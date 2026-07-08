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

	std::vector<Move> Knight::getMoves(ChessBoard& chessBoard, ChessPiece knight) {
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = knight.getCoordinates();
		int s;
		int t;
		int knightDirs[8][2] = {{-1, 2}, {-1, -2}, {1, 2}, {1, -2}, {-2, 1}, {-2, -1}, {2, 1}, {2, -1}};

		for (const auto& d: knightDirs) {
			s = r + d[0];
			t = c + d[1];

			if (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto piece = chessBoard.pieceAt(s, t);

				if (piece.getColor() != knight.getColor()) {
					Move move(r, c, s, t, knight, chessBoard.pieceAt(s, t));
					moves.push_back(move);
				}
			}
		}

		return moves;
	}
}