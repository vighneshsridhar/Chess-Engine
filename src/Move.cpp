#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>

#include "Move.h"

namespace ChessGame {

	Move::Move(sf::Vector2f initialSquare, sf::Vector2f endSquare, int moveNumber, ChessPiece piece, ChessPiece capturedPiece) : initialSquare(initialSquare), 
		endSquare(endSquare), moveNumber(moveNumber), piece(piece), capturedPiece(capturedPiece) {
	};

	sf::Vector2f Move::getInitialSquare() const {
		return initialSquare;
	}

	sf::Vector2f Move::getEndSquare() const {
		return endSquare;
	}

	ChessPiece Move::getCapturedPiece() const {
		return capturedPiece;
	}

	bool Move::operator == (const Move & m) const {
		return initialSquare == m.getInitialSquare() && endSquare == m.getEndSquare();
	}
}