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
		pgn = "";
		moveNumber = 0;
	};

	std::string PGN::convertSquareToPGN(int r, int c) const {
		std::vector<char> files = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h' };
		std::vector<char> ranks = { '8', '7', '6', '5', '4', '3', '2', '1' };
		char file = files[c];
		char rank = ranks[r];
		std::string square;
		square += file;
		square += rank;

		return square;
	}

	std::string PGN::convertMoveToPGN(Move::MoveNode* n, int moveNumber, ChessBoard chessBoard) const {
		Move* move = n->move;
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		auto [r1, c1] = Functions::convertToSquare(move->getInitialSquare());
		auto [r2, c2] = Functions::convertToSquare(move->getEndSquare());

		ChessPiece empty(PieceType::EMPTY, PieceColor::NONE, move->getInitialSquare());
		ChessPiece piece = b[r1][c1];
		PieceType type = piece.getPieceType();
		PieceColor color = piece.getColor();
		bool moveRecorded = false;
		std::string s;
		std::string ans;

		if (type == PieceType::KING && c2 - c1 == 2) {
			s = "O-O";
			moveRecorded = true;
		}

		if (type == PieceType::KING && c2 - c1 == -2) {
			s = "O-O-O";
			moveRecorded = true;
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

		if (move->getCapturedPiece().getPieceType() != PieceType::EMPTY || move->isEnPassant()) {
			capture = "x";
		}

		if (type == PieceType::PAWN) {

			if (capture.length() > 0) {
				s = pieceToLetter[type] + capture + square2;
			}

			else {
				s = pieceToLetter[type] + square2[1];
			}

			if (r2 == 0 || r2 == 7) {
				ChessPiece p = n->promotionPiece;
				s += "=" + pieceToLetter[p.getPieceType()];
			}

			if (chessBoard.getTurn()) {
				ans += std::to_string(moveNumber) + ".";
			}
			ans += s;
			return ans;
		}
		std::vector<Move*> moves = chessBoard.getPieceMoves(b[r1][c1]);
		int boardSize = 8;

		for (int r3 = 0; r3 < boardSize; r3++) {

			for (int c3 = 0; c3 < boardSize; c3++) {

				if (r1 == r3 && c1 == c3) {
					continue;
				}
				ChessPiece piece2 = b[r3][c3];

				if (piece2 == piece) {
					std::vector<Move*> moves2 = chessBoard.getPieceMoves(piece2);

					for (const auto& move2 : moves2) {

						if (move2->getEndSquare() == move->getEndSquare() && c1 == c3) {
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
				ChessPiece piece2 = b[r3][c3];

				if (piece2 == piece) {
					std::vector<Move*> moves2 = chessBoard.getPieceMoves(piece2);

					for (const auto& move2 : moves2) {

						if (move2->getEndSquare() == move->getEndSquare() && (r1 == r3 || disambiguateRank.length() == 0)) {
							disambiguateFile = convertSquareToPGN(r1, c1)[0];
						}
					}
				}
			}
		}

		if (!moveRecorded) {
			s = pieceToLetter[type] + disambiguateFile + disambiguateRank + capture + square2;
		}

		if (chessBoard.getTurn()) {
			ans = std::to_string(moveNumber) + ".";
		}
		ans += s;
		return ans;
	}

	std::string PGN::generatePGN(Move::MoveNode* root, ChessBoard chessBoard, int index) {
		std::string pgn_ans = "";
		std::string move_pgn;
		int moveNumber = root->moveNumber;
		bool priorityLine = false;

		if (root->move) {
			move_pgn = convertMoveToPGN(root, moveNumber, chessBoard) + root->check;
			pgn_ans = move_pgn + " ";
		}
		std::vector<Move::MoveNode*> siblings;

		if (root->parent) {
			siblings = root->parent->children;
		}

		if (index + 1 < siblings.size()) {
			move_pgn = generatePGN(siblings[index + 1], chessBoard, index + 1);
			pgn_ans += "\n";

			if (!chessBoard.getTurn()) {
				pgn_ans += "..." + std::to_string(moveNumber) + ".";
			}
			pgn_ans += move_pgn;
		}

		if (index == 0 && siblings.size() > 1) {
			pgn_ans += "\n";
			priorityLine = true;
		}

		if (root->children.size() > 0) {
			Move::MoveNode* n = root->children[0];
			chessBoard.setChessBoard(n->b);
			chessBoard.changeTurn();

			if (priorityLine && !chessBoard.getTurn()) {
				pgn_ans += "..." + std::to_string(moveNumber) + ".";
			}
			pgn_ans += generatePGN(n, chessBoard, 0);
		}

		return pgn_ans;
	}

	std::string PGN::checkOrCheckmate(ChessBoard chessBoard, size_t numLegalMoves) {
		bool w = chessBoard.isCheckOrCheckmate();
		std::string c;

		if (w && numLegalMoves == 0) {
			c = "#";
		}

		else if (w) {
			c = "+";
		}

		else if (numLegalMoves == 0) {
			return "";
		}

		return c;
	}
}