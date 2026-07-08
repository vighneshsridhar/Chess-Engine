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

	std::vector<Move> Rook::getMoves(ChessBoard& chessBoard, ChessPiece& rook) {
		std::vector<Move> moves;
		sf::Vector2f y;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = rook.getCoordinates();
		int s = r;
		int t = c;
		int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

		for (auto& d : rookDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {

				if (chessBoard.pieceAt(s, t).getColor() == chessBoard.pieceAt(r, c).getColor()) {
					break;
				}
				Move move(r, c, s, t, rook, chessBoard.pieceAt(s, t));
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