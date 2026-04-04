#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <bitset>

#include "PGN.h"
#include "Move.h"
#include "Functions.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "Bitboard.h"


namespace ChessGame {

	PGN::PGN() {
		moveNumber = 1;
	};

	std::string PGN::convertSquareToPGN(int r, int c) const {
		std::vector<char> files = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
		std::vector<char> ranks = { '8', '7', '6', '5', '4', '3', '2', '1' };
		char file = files[c];
		char rank = ranks[r];
		std::string square;
		square += file;
		square += rank;

		return square;
	}

	std::string PGN::convertMoveToPGN(Move move, bool isCapture, ChessBoard b) const {
		std::vector<std::vector<ChessPiece>> chessBoard = b.getChessBoard();
		auto [r1, c1] = Functions::convertToSquare(move.getInitialSquare());
		auto [r2, c2] = Functions::convertToSquare(move.getEndSquare());

		ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, move.getInitialSquare());
		ChessPiece piece = chessBoard[r1][c1];
		PieceType type = piece.getPieceType();
		PieceColor color = piece.getColor();
		std::string ans;

		if (type == PieceType::KING && c2 - c1 == 2) {
			ans = "O-O";

			return ans;
		}

		if (type == PieceType::KING && c2 - c1 == -2) {
			ans = "O-O-O";

			return ans;
		}

		std::string square1 = convertSquareToPGN(r1, c1);
		std::string square2 = convertSquareToPGN(r2, c2);
		std::unordered_map<PieceType, std::string> pieceToLetter;
		pieceToLetter[PieceType::PAWN] = square1[0];
		pieceToLetter[PieceType::KNIGHT] = "N";
		pieceToLetter[PieceType::BISHOP] = "B";
		pieceToLetter[PieceType::ROOK] = "R";
		pieceToLetter[PieceType::QUEEN] = "Q";
		pieceToLetter[PieceType::KING] = "K";

		std::string disambiguateFile = "";
		std::string disambiguateRank = "";
		std::string capture = "";

		if (isCapture) {
			capture = "x";
		}

		if (type == PieceType::PAWN) {

			if (capture.length() > 0) {
				ans = pieceToLetter[type] + capture + square2;
			}

			else {
				ans = pieceToLetter[type] + square2[1];
			}

			return ans;
		}
		std::vector<Move> moves = b.getPieceMoves(chessBoard[r1][c1]);
		int boardSize = 8;
		std::vector<ChessPiece> samePieces;

		for (int r3 = 0; r3 < boardSize; r3++) {

			for (int c3 = 0; c3 < boardSize; c3++) {
				
				if (r1 == r3 && c1 == c3) {
					continue;
				}
				ChessPiece piece2 = chessBoard[r3][c3];

				if (piece2 == piece) {
					std::vector<Move> moves2 = b.getPieceMoves(piece2);

					for (const auto& move2 : moves2) {
						
						if (move2.getEndSquare() == move.getEndSquare() && c1 == c3) {
							disambiguateRank = convertSquareToPGN(r1, c1)[1];
						}
					}
				}
			}
		}

		for (int r3 = 0; r3 < boardSize; r3++) {

			for (int c3 = 0; c3 < boardSize; c3++) {

				if (r1 == r3 && c1 == c3) {
					continue;
				}
				ChessPiece piece2 = chessBoard[r3][c3];

				if (piece2 == piece) {
					std::vector<Move> moves2 = b.getPieceMoves(piece2);

					for (const auto& move2 : moves2) {

						if (move2.getEndSquare() == move.getEndSquare() && (r1 == r3 || disambiguateRank.length() == 0)) {
							disambiguateFile = convertSquareToPGN(r1, c1)[0];
						}
					}
				}
			}
		}
		ans = pieceToLetter[type] + disambiguateFile + disambiguateRank + capture + square2;

		return ans;
	}

	void PGN::printMove(std::string ans, ChessBoard b, size_t numLegalMoves) const {
		if (!b.getTurn()) {
			ans = std::to_string(moveNumber) + ". " + ans;
		}

		else {
			ans = " " + ans;
		}
		std::string statement;
		bool w = b.isCheckOrCheckmate();

		if (w && numLegalMoves == 0) {
			ans += "#";
			statement = b.getTurn() ? "\n \nCheckmate! Black wins!" : "\n \nCheckmate! White wins!";
			std::cout << ans << statement << std::endl;
			return;
		}

		else if (w) {
			ans += "+";
		}
		
		else if (numLegalMoves == 0) {
			statement = "\n \nStalemate!";
			std::cout << ans << statement << std::endl;
			return;
		}

		if (b.getTurn()) {
			ans += "\n";
		}
		std::cout << ans;
		return;
	}

	void PGN::incrementMove() {
		moveNumber++;
	}

	void PGN::decrementMove() {
		if (moveNumber > 1) {
			moveNumber--;
		}
	}
}