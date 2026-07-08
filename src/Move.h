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
		Move(int r1, int c1, int r2, int c2, ChessPiece piece, ChessPiece capturedPiece);
		/*struct MoveNode {
			Move* move;
			std::vector<MoveNode*> children;
			MoveNode* parent;
			size_t moveNumber;
			std::vector<std::vector<ChessPiece>> b;
			std::vector<Move> legalMoves;
			std::string checkSymbol;
		}; */
		std::pair<int, int> getInitialSquare() const;
		std::pair<int, int> getEndSquare() const;
		ChessPiece getAttacker() const;
		ChessPiece getCapturedPiece() const;
		bool isCheck() const;
		void setCheck(bool check);
		bool isCapture();
		std::pair<bool, bool> isCastling() const;
		bool isEnPassant() const;
		ChessPiece getPromotionPiece() const;
		void setPromotionPiece(ChessPiece p);
		int getOrderingScore(std::vector<std::vector<Move>> killerMoves, int depth);
		int pieceValues[7];
		int getCaptureScore();
		bool operator == (const Move& m) const;

	private:
		int r1;
		int c1;
		int r2;
		int c2;
		ChessPiece piece;
		ChessPiece capturedPiece;
		ChessPiece promotionPiece;
		bool check;
	};

	struct MoveNode {
		Move* move;
		std::vector<MoveNode*> children;
		MoveNode* parent;
		size_t moveNumber;
		std::vector<std::vector<ChessPiece>> b;
		std::vector<Move> legalMoves;
		std::string checkSymbol;
	};
}

#endif