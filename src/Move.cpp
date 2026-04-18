#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>
#include <cmath>

#include "Move.h"
#include "Functions.h"

namespace ChessGame {
	
	Move::Move() {
		moveNumber = 0;
		check = false;
		score = 0;
		initialSquare = sf::Vector2f(0, 0);
		endSquare = sf::Vector2f(0, 0);
	};

	Move::Move(sf::Vector2f initialSquare, sf::Vector2f endSquare, int moveNumber, ChessPiece piece, ChessPiece capturedPiece) : initialSquare(initialSquare), 
		endSquare(endSquare), moveNumber(moveNumber), piece(piece), capturedPiece(capturedPiece) {
		ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, sf::Vector2(20.f, 20.f));
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
		score = 0;
	};

	sf::Vector2f Move::getInitialSquare() const {
		return initialSquare;
	}

	sf::Vector2f Move::getEndSquare() const {
		return endSquare;
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
		auto [r1, c1] = Functions::convertToSquare(initialSquare);
		auto [r2, c2] = Functions::convertToSquare(endSquare);
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
		if (piece.getPieceType() == PieceType::PAWN && std::abs(endSquare.x - initialSquare.x) > 0 && capturedPiece.getPosition().y == initialSquare.y) {
			return true;
		}
		return false;
	}

	ChessPiece Move::getPromotionPiece() {
		return promotionPiece;
	}

	void Move::setPromotionPiece(ChessPiece p) {
		promotionPiece = p;
	}

	int Move::getOrderingScore(std::vector<std::vector<Move>> killerMoves, int depth) {
		int score = 0;

		if (isCapture()) {
			score += 100 + pieceValues[capturedPiece.getPieceType()] - pieceValues[piece.getPieceType()];
		}

		if (promotionPiece.getPieceType() != PieceType::EMPTY) {
			score += 90;
		}

		if (isCheck()) {
			score += 50;
		}

		if (*this == killerMoves[depth - 1][0] || *this == killerMoves[depth - 1][1]) {
			score += 90;
		}

		return score;
	}

	bool Move::operator == (const Move m) const {
		return initialSquare == m.getInitialSquare() && endSquare == m.getEndSquare();
	}
}