#ifndef ChessGame_ChessBoard_h
#define ChessGame_ChessBoard_h

#include <SFML/Graphics.hpp>
#include <string>
#include <utility>

#include "ChessPiece.h"

namespace ChessGame {

	class ChessBoard {
	public:
		ChessBoard();
		bool getTurn();
		void changeTurn();
		std::vector<std::vector<ChessPiece>> getChessBoard();
		void setChessBoard(std::vector<std::vector<ChessPiece>>);
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getLegalMoves();
		std::vector<std::pair<sf::Vector2f, sf::Vector2f>> getPieceMoves(ChessPiece piece);
		std::pair<int, int> getKingPosition();
		void setKingPosition(std::pair<int, int> coordinates);
		bool isCheckmate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves);
		bool isStalemate(std::vector<std::pair<sf::Vector2f, sf::Vector2f>> legalMoves);

	private:
		int boardSize;
		bool whiteTurn;
		std::vector<std::vector<ChessPiece>> chessBoard;
		std::unordered_map<std::string, std::pair<int, int>> kingPosition;
	};

}

#endif