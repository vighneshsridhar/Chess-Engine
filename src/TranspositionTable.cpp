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
		hashTable.resize(64, std::vector<unsigned long long>(13, 0));

		for (int s = 0; s < 64; s++) {
			
			for (int t = 0; t < 12; t++) {
				hashTable[s][t] = dist(gen);
			}
		}

		for (int u = 0; u < 5; u++) {
			hashTable[u][12] = dist(gen);
		}
	};

	unsigned long long TranspositionTable::updateHash(Move move, unsigned long long h) {
		int s1 = move.getInitialSquare().first * 8 + move.getInitialSquare().second;
		int s2 = move.getEndSquare().first * 8 + move.getEndSquare().second;
		ChessPiece piece = move.getAttacker();
		ChessPiece capturedPiece = move.getCapturedPiece();
		int t1 = static_cast<int>(piece.getPieceType()) - 1;
		int t2 = static_cast<int>(capturedPiece.getPieceType()) - 1;
		
		if (piece.getColor() == PieceColor::BLACK) {
			t1 += 6;
		}

		if (capturedPiece.getColor() == PieceColor::BLACK) {
			t2 += 6;
		}
		/*if (s1 == 42 && s2 == 57) {
			std::cout << "h before = " << h << std::endl;
		}

		if (s1 == 57 && s2 == 42) {
			std::cout << "h before = " << h << std::endl;
		} */
		h ^= hashTable[s1][t1];
		h ^= hashTable[s2][t1];

		/*if (s1 == 42 && s2 == 57) {
			std::cout << "knight to b1 h = " << h << std::endl;
		}

		if (s1 == 57 && s2 == 42) {
			std::cout << "knight to c3 h = " << h << std::endl;
		} */

		if (capturedPiece.getPieceType() != PieceType::EMPTY) {
			h ^= hashTable[s2][t2];
		}
		auto [kingSide, queenSide] = move.isCastling();
		
		if (piece.getColor() == PieceColor::BLACK) {
			h ^= hashTable[0][12];

			if (kingSide) {
				h ^= hashTable[3][12];
			}

			if (queenSide) {
				h ^= hashTable[4][12];
			}
		}

		if (piece.getColor() == PieceColor::WHITE) {

			if (kingSide) {
				h ^= hashTable[1][12];
			}

			if (queenSide) {
				h ^= hashTable[2][12];
			}
		}

		return h;
	}

	unsigned long long TranspositionTable::getInitialH() {
		unsigned long long h = 0;

		for (int s = 0; s < 64; s++) {

			for (int t = 0; t < 12; t++) {
				h ^= hashTable[s][t];
			}
		}

		for (int u = 0; u < 5; u++) {
			h ^= hashTable[u][12];
		}

		return h;
	}
}