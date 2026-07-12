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
		int boardSize = 8;
		auto [r, c] = rook.getCoordinates();
		int s = r;
		int t = c;
		int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

		for (auto& d : rookDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {

				if (chessBoard.pieceAt(s, t).getColor() == rook.getColor()) {
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

	std::vector<Move> Rook::getCaptures(ChessBoard& chessBoard, ChessPiece& rook) {
		std::vector<Move> captures;
		int boardSize = 8;
		auto [r, c] = rook.getCoordinates();
		int s = r;
		int t = c;
		int rookDirs[4][2] = { {-1, 0}, {1,0}, {0, -1}, {0, 1} };

		for (auto& d : rookDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {

				if (chessBoard.pieceAt(s, t).getColor() == rook.getColor()) {
					break;
				}
				Move move(r, c, s, t, rook, chessBoard.pieceAt(s, t));

				if (chessBoard.pieceAt(s, t).getPieceType() != PieceType::EMPTY) {
					captures.push_back(move);
					break;
				}
				s += d[0];
				t += d[1];
			}
		}

		return captures;
	}
}