#include <utility>
#include <iostream>
#include <vector>
#include <random>

#include "TranspositionTable.h"
#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"


namespace ChessGame {

	TranspositionTable::TranspositionTable() {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<unsigned long long> dist(0, 0xFFFFFFFFFFFFFFFFULL);
		boardSize = 8;

		castlingRights.push({ true, true, true, true });
		enPassantFiles.push(-1);

		hashTable.resize(64, std::vector<unsigned long long>(13, 0));

		for (int s = 0; s < 64; s++) {
			
			for (int t = 0; t < 12; t++) {
				hashTable[s][t] = dist(gen);
			}
		}

		for (int u = 0; u < 13; u++) {
			hashTable[u][12] = dist(gen);
		}
	};

	unsigned long long TranspositionTable::updateHash(Move& move, bool push, ChessBoard& chessBoard, unsigned long long h) {
		if (!push) {
			castlingRights.pop();
			enPassantFiles.pop();
		}
		auto [r1, c1] = move.getInitialSquare();
		auto [r2, c2] = move.getEndSquare();
		int s1 = r1 * 8 + c1;
		int s2 = r2 * 8 + c2;
		const ChessPiece& piece = move.getAttacker();
		const ChessPiece& capturedPiece = move.getCapturedPiece();
		int t1 = static_cast<int>(piece.getPieceType()) - 1;
		int t2 = static_cast<int>(capturedPiece.getPieceType()) - 1;
		
		if (piece.getColor() == PieceColor::BLACK) {
			t1 += 6;
		}

		if (capturedPiece.getColor() == PieceColor::BLACK) {
			t2 += 6;
		}
		h ^= hashTable[s1][t1];
		h ^= hashTable[s2][t1];

		if (capturedPiece.getPieceType() != PieceType::EMPTY) {
			s2 = capturedPiece.getCoordinates().first * 8 + capturedPiece.getCoordinates().second;
			h ^= hashTable[s2][t2];
		}
		h ^= hashTable[0][12];
		
		auto [kingSide, queenSide] = move.isCastling();
		t1 = piece.getColor() == PieceColor::WHITE ? 3 : 9;

		if (kingSide) {
			s1 = r1 * 8 + 7;
			s2 = r1 * 8 + 5;
			h ^= hashTable[s1][t1];
			h ^= hashTable[s2][t1];
		}

		if (queenSide) {
			s1 = r1 * 8;
			s2 = r1 * 8 + 3;
			h ^= hashTable[s1][t1];
			h ^= hashTable[s2][t1];
		}
		auto [whiteKingSide, whiteQueenSide] = chessBoard.castlingRights(PieceColor::WHITE);
		auto [blackKingSide, blackQueenSide] = chessBoard.castlingRights(PieceColor::BLACK);
		std::vector<bool> a = { whiteKingSide, whiteQueenSide, blackKingSide, blackQueenSide };
		std::vector<bool> b = castlingRights.top();

		for (int i = 1; i <= 4; i++) {
			
			if (b[i - 1]) {
				h ^= hashTable[i][12];
			}

			if (a[i - 1]) {
				h ^= hashTable[i][12];
			}
		}
		int enPassantFile = chessBoard.getEnPassantFile();
		int prevEnPassantFile = enPassantFiles.top();

		if (enPassantFile != -1) {
			h ^= hashTable[enPassantFile + 5][12];
		}

		if (prevEnPassantFile != -1) {
			h ^= hashTable[prevEnPassantFile + 5][12];
		}

		if (push) {
			castlingRights.push(a);
			enPassantFiles.push(enPassantFile);
		}

		return h;
	}

	unsigned long long TranspositionTable::getInitialH(ChessBoard& chessBoard) {
		unsigned long long h = 0;

		for (int r = 0; r < boardSize; r++){

			for (int c = 0; c < boardSize; c++) {
				ChessPiece& piece = chessBoard.pieceAt(r, c);

				if (piece.getPieceType() == PieceType::EMPTY) {
					continue;
				}
				auto s = r * 8 + c;
				auto t = static_cast<int>(piece.getPieceType()) - 1;

				if (piece.getColor() == PieceColor::BLACK) {
					t += 6;
				}
				h ^= hashTable[s][t];
			}
		}
		h = 0;
		count[h] += 1;

		return h;
	}

	TranspositionTable::TTEntry& TranspositionTable::getTT(unsigned long long h) {
		if (tt.find(h) != tt.end()) {
			return tt[h];
		}
		Move move;
		TTEntry entry = { 0, std::numeric_limits<int>::min(), TTFlag::EXACT_EVAL, move };

		return entry;
	}

	void TranspositionTable::updateTT(unsigned long long h, TranspositionTable::TTEntry& entry) {
		tt[h] = entry;
		return;
	}

	void TranspositionTable::incrementCount(unsigned long long h) {
		count[h] += 1;
		return;
	}

	void TranspositionTable::decrementCount(unsigned long long h) {
		count[h] -= 1;
		return;
	}

	int TranspositionTable::getPositionCount(unsigned long long h) {
		if (count.find(h) != count.end()) {
			return count[h];
		}

		return 0;
	}
}