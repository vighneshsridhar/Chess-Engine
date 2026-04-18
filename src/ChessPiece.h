#pragma once
#ifndef ChessGame_ChessPiece_h
#define ChessGame_ChessPiece_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>


namespace ChessGame {

	enum class PieceType {
		EMPTY,
		PAWN,
		KNIGHT,
		BISHOP,
		ROOK,
		QUEEN,
		KING
	};

	enum class PieceColor {
		WHITE,
		BLACK,
		NONE
	};

	class ChessPiece {
	public: 
		ChessPiece();
		ChessPiece(PieceType pieceType, PieceColor color, sf::Vector2f position);
		PieceType getPieceType() const;
		void setPieceType(PieceType newPieceType);
		PieceColor getColor();
		void setColor(PieceColor color);
		sf::Texture getTexture() const;
		sf::Vector2f getPosition() const;
		void setPosition(sf::Vector2f newPosition);
		bool pieceHasMoved() const;
		void setPieceHasMoved();
		bool operator == (ChessPiece& piece) const;

	private:
		PieceType pieceType;
		PieceColor color;
		sf::Vector2f position;
		bool hasMoved;
	};

}

#endif