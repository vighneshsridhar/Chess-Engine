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

	std::vector<Move> King::getMoves(ChessBoard& chessBoard, ChessPiece& king) {
		std::vector<Move> moves;
		int boardSize = 8;
		float squareSize = 100.f;
		auto [r, c] = king.getCoordinates();

		PieceColor color = king.getColor();

		for (int s = r - 1; s <= r + 1; s++) {

			if (s < 0 || s >= boardSize) {
				continue;
			}
			for (int t = c - 1; t <= c + 1; t++) {

				if (t < 0 || t >= boardSize) {
					continue;
				}
				auto piece = chessBoard.pieceAt(s, t);

				if (piece.getColor() != color) {
					Move move(r, c, s, t, king, piece);
					moves.push_back(move);
				}
			}
		}

		if (!king.pieceHasMoved()) {

			if (color == PieceColor::WHITE) {
				auto rook = chessBoard.pieceAt(7, 7);
				auto square1 = chessBoard.pieceAt(7, 5);
				auto square2 = chessBoard.pieceAt(7, 6);

				if (!rook.pieceHasMoved() && square1.getPieceType() == PieceType::EMPTY && square2.getPieceType() == PieceType::EMPTY && !Bitboard::kingAttacked(chessBoard, 7, 4, color) &&
					!Bitboard::kingAttacked(chessBoard, 7, 5, color) && !Bitboard::kingAttacked(chessBoard, 7, 6, color)) {
					Move move(r, c, 7, 6, king, square2);
					moves.push_back(move);
				}
				rook = chessBoard.pieceAt(7, 0);
				square1 = chessBoard.pieceAt(7, 1);
				square2 = chessBoard.pieceAt(7, 2);
				auto square3 = chessBoard.pieceAt(7, 3);

				if (!rook.pieceHasMoved() && square1.getPieceType() == PieceType::EMPTY && square2.getPieceType() == PieceType::EMPTY && square3.getPieceType() == PieceType::EMPTY && 
					!Bitboard::kingAttacked(chessBoard, 7, 4, color) && !Bitboard::kingAttacked(chessBoard, 7, 3, color) && !Bitboard::kingAttacked(chessBoard, 7, 2, color)) {
					Move move(r, c, 7, 2, king, square2);
					moves.push_back(move);
				}
			}

			if (color == PieceColor::BLACK) {
				auto rook = chessBoard.pieceAt(0, 7);
				auto square1 = chessBoard.pieceAt(0, 5);
				auto square2 = chessBoard.pieceAt(0, 6);

				if (!rook.pieceHasMoved() && square1.getPieceType() == PieceType::EMPTY && square2.getPieceType() == PieceType::EMPTY && !Bitboard::kingAttacked(chessBoard, 0, 4, color) &&
					!Bitboard::kingAttacked(chessBoard, 0, 5, color) && !Bitboard::kingAttacked(chessBoard, 0, 6, color)) {
					Move move(r, c, 0, 6, king, square2);
					moves.push_back(move);
				}
				rook = chessBoard.pieceAt(0, 0);
				square1 = chessBoard.pieceAt(0, 1);
				square2 = chessBoard.pieceAt(0, 2);
				auto square3 = chessBoard.pieceAt(0, 3);

				if (!rook.pieceHasMoved() && square1.getPieceType() == PieceType::EMPTY && square2.getPieceType() == PieceType::EMPTY && square3.getPieceType() == PieceType::EMPTY &&
					!Bitboard::kingAttacked(chessBoard, 0, 4, color) && !Bitboard::kingAttacked(chessBoard, 0, 3, color) && !Bitboard::kingAttacked(chessBoard, 0, 2, color)) {
					Move move(r, c, 0, 2, king, square2);
					moves.push_back(move);
				}
			}
		}

		return moves;
	}
}