#ifndef ChessGame_ChessBoard_h
#define ChessGame_ChessBoard_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

#include "ChessPiece.h"
#include "Move.h"

namespace ChessGame {

	class ChessBoard {
	public:
		ChessBoard();
		bool whiteTurn() const;
		void changeTurn();
		std::vector<std::vector<ChessPiece>> getChessBoard();
		void setChessBoard(std::vector<std::vector<ChessPiece>> newChessBoard);
		ChessPiece& pieceAt(int r, int c);
		void setPieceAt(ChessPiece piece, int r, int c);
		std::vector<Move> getLegalMoves();
		std::vector<Move> getCaptureMoves();
		std::vector<Move> getPieceMoves(ChessPiece& piece);
		std::vector<Move> getPieceCaptures(ChessPiece& piece);
		void push(Move& move);
		void unmakeMove(Move& move);
		std::pair<int, int> getKingPosition();
		void setKingPosition(std::pair<int, int> coordinates);
		int getEnPassantFile();
		void setEnPassantFile(int file);
		bool isCheckOrCheckmate();

	private:
		int boardSize;
		bool wTurn;
		int enPassantFile;
		std::vector<std::vector<ChessPiece>> b;
		std::unordered_map<std::string, std::pair<int, int>> kingPosition;
	};

}

#endif