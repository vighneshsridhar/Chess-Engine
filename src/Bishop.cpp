#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

#include "Bishop.h"
#include "Move.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Functions.h"


namespace ChessGame {

	Bishop::Bishop(PieceColor color) : color(color) {
	};

	std::vector<Move> Bishop::getMoves(ChessBoard& chessBoard, ChessPiece& bishop) {
		auto [r, c] = bishop.getCoordinates();
		std::vector<Move> moves;
		int boardSize = 8;
		int bishopDirs[4][2] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
		int s;
		int t;

		for (auto& d : bishopDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto piece = chessBoard.pieceAt(s, t);

				if (piece.getColor() == bishop.getColor()) {
					break;
				}
				Move move(r, c, s, t, bishop, chessBoard.pieceAt(s, t));
				moves.push_back(move);

				if (piece.getPieceType() != PieceType::EMPTY) {
					break;
				}
				s += d[0];
				t += d[1];
			}
		}

		return moves;
	}

	std::vector<Move> Bishop::getCaptures(ChessBoard& chessBoard, ChessPiece& bishop) {
		auto [r, c] = bishop.getCoordinates();
		std::vector<Move> captures;
		int boardSize = 8;
		
		int bishopDirs[4][2] = { {-1, -1}, {-1, 1}, {1, -1}, {1, 1} };
		int s;
		int t;

		for (auto& d : bishopDirs) {
			s = r + d[0];
			t = c + d[1];

			while (s >= 0 && s < boardSize && t >= 0 && t < boardSize) {
				auto piece = chessBoard.pieceAt(s, t);

				if (piece.getColor() == bishop.getColor()) {
					break;
				}
				Move move(r, c, s, t, bishop, chessBoard.pieceAt(s, t));

				if (piece.getPieceType() != PieceType::EMPTY) {
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