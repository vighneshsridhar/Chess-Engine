#ifndef ChessGame_ChessBoard_h
#define ChessGame_ChessBoard_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>
#include <vector>

#include "Move.h"
#include "ChessPiece.h"

namespace ChessGame {

	class ChessBoard {
	public:
		ChessBoard();
		bool getTurn() const;
		void changeTurn();
		std::vector<std::vector<ChessPiece>> getChessBoard();
		void setChessBoard(std::vector<std::vector<ChessPiece>>);
		std::vector<Move> getLegalMoves();
		std::vector<Move> getPieceMoves(ChessPiece piece);
		std::pair<int, int> getKingPosition();
		void setKingPosition(std::pair<int, int> coordinates);
		int getEnPassantFile();
		void setEnPassantFile(int file);
		bool isCheckOrCheckmate();

	private:
		int boardSize;
		bool whiteTurn;
		int enPassantFile;
		std::vector<std::vector<ChessPiece>> chessBoard;
		std::unordered_map<std::string, std::pair<int, int>> kingPosition;
	};

}

#endif