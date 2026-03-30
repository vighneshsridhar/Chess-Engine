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
		ChessPiece(PieceType pieceType, PieceColor color, sf::Vector2f position);
		PieceType getPieceType();
		void setPieceType(PieceType newPieceType);
		PieceColor getColor();
		sf::Texture getTexture();
		sf::Vector2f getPosition() const;
		void setPosition(sf::Vector2f);
		bool pieceHasMoved();
		void setPieceHasMoved();

	private:
		PieceType pieceType;
		PieceColor color;
		sf::Vector2f position;
		bool hasMoved;
	};

}

#endif