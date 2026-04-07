#ifndef ChessGame_Move_h
#define ChessGame_Move_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

#include "ChessPiece.h"

namespace ChessGame {

	class Move {
	public:
		Move(sf::Vector2f initialSquare, sf::Vector2f endSquare, int moveNumber, ChessPiece piece, ChessPiece capturedPiece);
		struct MoveNode {
			Move* move;
			std::vector<MoveNode*> children;
			MoveNode* parent;
			size_t moveNumber;
			std::vector<std::vector<ChessPiece>> b;
			std::string check;
		};
		sf::Vector2f getInitialSquare() const;
		sf::Vector2f getEndSquare() const;
		ChessPiece getCapturedPiece() const;
		bool isEnPassant() const;
		bool operator == (const Move m) const;

	private:
		sf::Vector2f initialSquare;
		sf::Vector2f endSquare;
		int moveNumber;
		ChessPiece piece;
		ChessPiece capturedPiece;
	};

}

#endif