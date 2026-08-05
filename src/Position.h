#ifndef ChessGame_Position_h
#define ChessGame_Position_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Functions.h"

namespace ChessGame {

	class Position {
	public:
		Position();
		static void setupBoard(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, 
			std::vector<std::vector<sf::Sprite>>& pieceSprites);
	};
}

#endif