#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

#include "Move.h"
#include "Functions.h"

namespace ChessGame {
	
	Move::Move() {
		check = false;
		r1 = -1;
		c1 = -1;
		r2 = -1;
		c2 = -1;
	};

	Move::Move(int r1, int c1, int r2, int c2, ChessPiece piece, ChessPiece capturedPiece) : r1(r1), c1(c1), r2(r2), c2(c2), piece(piece), capturedPiece(capturedPiece) {
		ChessPiece empty;
		promotionPiece = empty;
		pieceValues = {
			{PieceType::PAWN, 100},
			{PieceType::KNIGHT, 320},
			{PieceType::BISHOP, 330},
			{PieceType::ROOK, 500},
			{PieceType::QUEEN, 900},
			{PieceType::KING, 0}
		};
		check = false;
	};

	std::pair<int, int> Move::getInitialSquare() const {
		return std::make_pair(r1, c1);
	}

	std::pair<int, int> Move::getEndSquare() const {
		return std::make_pair(r2, c2);
	}

	ChessPiece Move::getAttacker() const {
		return piece;
	}

	ChessPiece Move::getCapturedPiece() const {
		return capturedPiece;
	}

	bool Move::isCheck() const {
		return check;
	}

	void Move::setCheck(bool c) {
		check = c;
	}

	bool Move::isCapture() {
		return capturedPiece.getPieceType() != PieceType::EMPTY;
	}

	std::pair<bool, bool> Move::isCastling() const {
		bool kingSide = false;
		bool queenSide = false;

		if (c2 - c1 == 2) {
			kingSide = true;
		}

		if (c2 - c1 == -2) {
			queenSide = true;
		}

		return std::make_pair(kingSide, queenSide);
	}

	bool Move::isEnPassant() const {
		if (piece.getPieceType() == PieceType::PAWN && std::abs(c2 - c1) > 0 && capturedPiece.getCoordinates().first == r1) {
			return true;
		}
		return false;
	}

	ChessPiece Move::getPromotionPiece() const {
		return promotionPiece;
	}

	void Move::setPromotionPiece(ChessPiece p) {
		promotionPiece = p;
	}

	int Move::getOrderingScore(std::vector<std::vector<Move>> killerMoves, int depth) {
		int score = 0;

		if (isCapture()) {
			score += 10000 + pieceValues[capturedPiece.getPieceType()] - pieceValues[piece.getPieceType()];
		}

		if (promotionPiece.getPieceType() != PieceType::EMPTY) {
			score += 9000;
		}

		if (isCheck()) {
			score += 5000;
		}

		if (*this == killerMoves[depth][0] || *this == killerMoves[depth][1]) {
			score += 9000;
		}

		return score;
	}

	bool Move::operator == (const Move& m) const {
		return r1 == m.r1 && c1 == m.c1 && r2 == m.r2 && c2 == m.c2 && promotionPiece.getPieceType() == m.promotionPiece.getPieceType();
	}
}