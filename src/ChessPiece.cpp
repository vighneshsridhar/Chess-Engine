#include <SFML/Graphics.hpp>
#include "ChessPiece.h"
#include <string>
#include <utility>
#include <iostream>
#include <filesystem> // Requires C++17 or later
#include <windows.h>


namespace ChessGame {

	ChessPiece::ChessPiece() {
		hasMoved = false;
		pieceType = PieceType::EMPTY;
		color = PieceColor::NONE;
		coordinates = std::make_pair(0, 0);
	};

	ChessPiece::ChessPiece(PieceType pieceType, PieceColor color, int r, int c): pieceType(pieceType), color(color) {
		hasMoved = false;
		coordinates = std::make_pair(r, c);
	};

	PieceType ChessPiece::getPieceType() const {
		return pieceType;
	}

	void ChessPiece::setPieceType(PieceType newPieceType) {
		pieceType = newPieceType;
		return;
	}

	PieceColor ChessPiece::getColor() {
		return color;
	}

	void ChessPiece::setColor(PieceColor newColor) {
		color = newColor;
	}

	sf::Texture ChessPiece::getTexture() const {
		sf::Texture texture;
		std::string path;

		if (pieceType == PieceType::EMPTY) {
			path = "Assets/WhitePawn.png";
		}

		if (pieceType == PieceType::PAWN) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhitePawn.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackPawn.png";
			}
		}

		if (pieceType == PieceType::KNIGHT) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhiteKnight.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackKnight.png";
			}
		}

		if (pieceType == PieceType::BISHOP) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhiteBishop.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackBishop.png";
			}
		}

		if (pieceType == PieceType::ROOK) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhiteRook.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackRook.png";
			}
		}

		if (pieceType == PieceType::QUEEN) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhiteQueen.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackQueen.png";
			}
		}

		if (pieceType == PieceType::KING) {

			if (color == PieceColor::WHITE) {
				path = "Assets/WhiteKing.png";
			}

			if (color == PieceColor::BLACK) {
				path = "Assets/BlackKing.png";
			}
		}

		if (!texture.loadFromFile(path)) {
			OutputDebugStringA("Failed to load texture\n");
		}

		return texture;
	}

	sf::Vector2f ChessPiece::getPosition() const {
		return position;
	}

	void ChessPiece::setPosition(sf::Vector2f p) {
		position = p;
		return;
	}

	std::pair<int, int> ChessPiece::getCoordinates() const {
		return coordinates;
	}

	void ChessPiece::setCoordinates(int r, int c) {
		coordinates = std::make_pair(r, c);
	}
	
	bool ChessPiece::pieceHasMoved() const {
		return hasMoved;
	}

	void ChessPiece::setPieceHasMoved() {
		hasMoved = true;
	}

	bool ChessPiece::operator == (ChessPiece& piece) const {
		return pieceType == piece.getPieceType() && color == piece.getColor();
	}
}