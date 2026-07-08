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

	std::vector<Move> Queen::getMoves(ChessBoard& chessBoard, ChessPiece& queen) {
		std::vector<Move> moves;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = queen.getCoordinates();
		int s;
		int t;
		int queenDirs[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

		for (auto& d : queenDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {

				if (chessBoard.pieceAt(s, t).getColor() == chessBoard.pieceAt(r, c).getColor()) {
					break;
				}
				Move move(r, c, s, t, queen, chessBoard.pieceAt(s, t));
				moves.push_back(move);

				if (chessBoard.pieceAt(s, t).getPieceType() != PieceType::EMPTY) {
					break;
				}
				s += d[0];
				t += d[1];
			}
		}

		return moves;
	}
}