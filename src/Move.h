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
		Move();
		Move(sf::Vector2f initialSquare, sf::Vector2f endSquare, int moveNumber, ChessPiece piece, ChessPiece capturedPiece);
		struct MoveNode {
			Move* move;
			std::vector<MoveNode*> children;
			MoveNode* parent;
			size_t moveNumber;
			std::vector<std::vector<ChessPiece>> b;
			std::string checkSymbol;
		};
		sf::Vector2f getInitialSquare() const;
		sf::Vector2f getEndSquare() const;
		ChessPiece getAttacker() const;
		ChessPiece getCapturedPiece() const;
		bool isCheck() const;
		void setCheck(bool check);
		bool isCapture();
		std::pair<bool, bool> isCastling() const;
		bool isEnPassant() const;
		ChessPiece getPromotionPiece();
		void setPromotionPiece(ChessPiece p);
		int getOrderingScore(std::vector<std::vector<Move>> killerMoves, int depth);
		bool operator == (const Move m) const;

	private:
		sf::Vector2f initialSquare;
		sf::Vector2f endSquare;
		int moveNumber;
		ChessPiece piece;
		ChessPiece capturedPiece;
		ChessPiece promotionPiece;
		bool check;
		int score;
		std::unordered_map<PieceType, int> pieceValues;
	};

}

#endif