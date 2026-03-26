#pragma once
#ifndef ChessGame_ChessPiece_h
#define ChessGame_ChessPiece_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

namespace ChessGame {

	class ChessPiece {
	public: 
		ChessPiece(std::string pieceType, std::string color, sf::Vector2f position);
		std::string getPieceType();
		void setPieceType(std::string newPieceType);
		std::string getColor();
		sf::Texture getTexture();
		sf::Vector2f getPosition();
		void setPosition(sf::Vector2f);
		bool pieceHasMoved();
		void setPieceHasMoved();

	private:
		std::string pieceType;
		std::string color;
		sf::Vector2f position;
		bool hasMoved;
	};

}

#endif