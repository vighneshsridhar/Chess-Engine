#include <SFML/Graphics.hpp>
#include "ChessPiece.h"
#include <string>
#include <utility>
#include <iostream>
#include <filesystem> // Requires C++17 or later
#include <windows.h>

namespace ChessGame {

	ChessPiece::ChessPiece(std::string pieceType, std::string color, sf::Vector2f position): pieceType(pieceType), color(color), position(position) {
		hasMoved = false;
	};

	std::string ChessPiece::getPieceType() {
		return pieceType;
	}

	void ChessPiece::setPieceType(std::string newPieceType) {
		pieceType = newPieceType;
		return;
	}

	std::string ChessPiece::getColor() {
		return color;
	}

	sf::Texture ChessPiece::getTexture() {
		sf::Texture texture;
		std::string path;

		if (pieceType == "EMPTY_SQUARE") {
			path = "Assets/WhitePawn.png";
		}

		if (pieceType == "PAWN") {

			if (color == "WHITE") {
				path = "Assets/WhitePawn.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackPawn.png";
			}
		}

		if (pieceType == "KNIGHT") {

			if (color == "WHITE") {
				path = "Assets/WhiteKnight.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackKnight.png";
			}
		}

		if (pieceType == "BISHOP") {

			if (color == "WHITE") {
				path = "Assets/WhiteBishop.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackBishop.png";
			}
		}

		if (pieceType == "ROOK") {

			if (color == "WHITE") {
				path = "Assets/WhiteRook.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackRook.png";
			}
		}

		if (pieceType == "QUEEN") {

			if (color == "WHITE") {
				path = "Assets/WhiteQueen.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackQueen.png";
			}
		}

		if (pieceType == "KING") {

			if (color == "WHITE") {
				path = "Assets/WhiteKing.png";
			}

			if (color == "BLACK") {
				path = "Assets/BlackKing.png";
			}
		}

		if (!texture.loadFromFile(path)) {
			OutputDebugStringA("Failed to load texture\n");
		}

		return texture;
	}

	sf::Vector2f ChessPiece::getPosition() {
		return position;
	}

	void ChessPiece::setPosition(sf::Vector2f p) {
		position = p;
		return;
	}
	
	bool ChessPiece::pieceHasMoved() {
		return hasMoved;
	}

	void ChessPiece::setPieceHasMoved() {
		hasMoved = true;
	}
}