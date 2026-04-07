#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

#include "King.h"
#include "Move.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	King::King(PieceColor color) : color(color) {
	};

	std::vector<Move*> King::getMoves(ChessBoard chessBoard, ChessPiece king) {
		sf::Vector2f position = king.getPosition();
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		std::vector<Move*> moves;
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

				if (b[s][t].getColor() != b[r][c].getColor()) {
					y = Functions::convertToPosition(s, t);
					Move* move = new Move(position, y, -1, king, b[s][t]);
					moves.push_back(move);
				}
			}
		}

		if (!king.pieceHasMoved()) {

			if (king.getColor() == PieceColor::WHITE) {

				if (!b[7][7].pieceHasMoved() && b[7][5].getPieceType() == PieceType::EMPTY && b[7][6].getPieceType() == PieceType::EMPTY) {
					y = Functions::convertToPosition(7, 6);
					Move* move = new Move(position, y, -1, king, b[7][6]);
					moves.push_back(move);
				}

				if (!b[7][0].pieceHasMoved() && b[7][1].getPieceType() == PieceType::EMPTY && b[7][2].getPieceType() == PieceType::EMPTY && b[7][3].getPieceType() == PieceType::EMPTY){
					y = Functions::convertToPosition(7, 2);
					Move* move = new Move(position, y, -1, king, b[7][2]);
					moves.push_back(move);
				}
			}

			if (king.getColor() == PieceColor::BLACK) {

				if (!b[0][7].pieceHasMoved() && b[0][5].getPieceType() == PieceType::EMPTY && b[0][6].getPieceType() == PieceType::EMPTY) {
					y = Functions::convertToPosition(0, 6);
					Move* move = new Move(position, y, -1, king, b[0][6]);
					moves.push_back(move);
				}

				if (!b[0][0].pieceHasMoved() && b[0][1].getPieceType() == PieceType::EMPTY && b[0][2].getPieceType() == PieceType::EMPTY && b[0][3].getPieceType() == PieceType::EMPTY) {
					y = Functions::convertToPosition(0, 2);
					Move* move = new Move(position, y, -1, king, b[0][2]);
					moves.push_back(move);
				}
			}
		}

		return moves;
	}
}