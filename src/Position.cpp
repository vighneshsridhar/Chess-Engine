#include <utility>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "Position.h"

namespace ChessGame {

	Position::Position() {
	}

	void Position::setupBoard(sf::RenderWindow& window, ChessBoard& chessBoard, std::vector<std::vector<sf::Sprite>>& spritesBoard, 
		std::vector<std::vector<sf::Sprite>>& pieceSprites) {
		sf::RenderWindow window2(sf::VideoMode({ 1125, 600 }), "Setup the Board Position.");
		int boardSize = 8;
		auto windowSize = window2.getSize();
		float squareSize = std::min(windowSize.x, windowSize.y) / 8.0f;
		std::vector<std::vector<ChessPiece>> b = chessBoard.getChessBoard();
		ChessPiece empty;
		ChessPiece newPiece;

		ChessGame::ChessPiece whitePawn(PieceType::PAWN, PieceColor::WHITE, -1, -1);
		ChessGame::ChessPiece whiteKnight(PieceType::KNIGHT, PieceColor::WHITE, -1, -1);
		ChessGame::ChessPiece whiteBishop(PieceType::BISHOP, PieceColor::WHITE, -1, -1);
		ChessGame::ChessPiece whiteRook(PieceType::ROOK, PieceColor::WHITE, -1, -1);
		ChessGame::ChessPiece whiteQueen(PieceType::QUEEN, PieceColor::WHITE, -1, -1);
		ChessGame::ChessPiece whiteKing(PieceType::KING, PieceColor::WHITE, -1, -1);

		ChessGame::ChessPiece blackPawn(PieceType::PAWN, PieceColor::BLACK, -1, -1);
		ChessGame::ChessPiece blackKnight(PieceType::KNIGHT, PieceColor::BLACK, -1, -1);
		ChessGame::ChessPiece blackBishop(PieceType::BISHOP, PieceColor::BLACK, -1, -1);
		ChessGame::ChessPiece blackRook(PieceType::ROOK, PieceColor::BLACK, -1, -1);
		ChessGame::ChessPiece blackQueen(PieceType::QUEEN, PieceColor::BLACK, -1, -1);
		ChessGame::ChessPiece blackKing(PieceType::KING, PieceColor::BLACK, -1, -1);
		std::vector<std::vector<ChessPiece>> newPieces = { {whitePawn, whiteKnight, whiteBishop, whiteRook, whiteQueen, whiteKing, empty},
			{blackPawn, blackKnight, blackBishop, blackRook, blackQueen, blackKing, empty} };
		sf::Sprite newPieceSprite = pieceSprites[0][6];
		int index1;
		int index2;

		sf::Vector2f mousePosition;
		int r = -1;
		int c = -1;
		int s = -1;
		int t = -1;

		sf::Vector2f dragOffset;
		float x;
		float y;

		sf::FloatRect spriteGlobalBounds;
		bool isDragging = false;
		bool isNewPiece = false;
		bool a = false;

		while (window2.isOpen()) {

			while (const std::optional event = window2.pollEvent()) {

				if (event->is<sf::Event::Closed>()) {
					window2.close();
				}

				if (event->is<sf::Event::MouseButtonPressed>()) {

					if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
						mousePosition = window2.mapPixelToCoords(sf::Mouse::getPosition(window2));
						r = int(mousePosition.y / squareSize);
						c = int(mousePosition.x / squareSize);

						if (!isNewPiece && r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
							isDragging = true;
							spriteGlobalBounds = spritesBoard[r][c].getGlobalBounds();
							dragOffset.x = spriteGlobalBounds.size.x / 2.0f;
							dragOffset.y = spriteGlobalBounds.size.y / 2.0f;
						}

						if (r == 1 && c == boardSize + 6) {
							isNewPiece = false;
							newPiece = newPieces[r - 1][c - 8];
						}

						else if (r == 2 && c == boardSize + 6) {
							isNewPiece = true;
							newPiece = newPieces[r - 1][c - 8];
							newPieceSprite = pieceSprites[r - 1][c - 8];
							index1 = r - 1;
							index2 = c - 8;
						}

						else if (r >= 1 && r <= 2 && c >= boardSize && c < boardSize + 6) {
							isNewPiece = true;
							newPiece = newPieces[r - 1][c - 8];
							newPieceSprite = pieceSprites[r - 1][c - 8];
							index1 = r - 1;
							index2 = c - 8;
							spriteGlobalBounds = newPieceSprite.getGlobalBounds();
							dragOffset.x = spriteGlobalBounds.size.x / 2.0f;
							dragOffset.y = spriteGlobalBounds.size.y / 2.0f;
						}

						if (isNewPiece && r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
							b[r][c] = newPiece;
							b[r][c].setCoordinates(r, c);
							spritesBoard[r][c] = newPieceSprite;
							newPiece = newPieces[index1][index2];
							newPieceSprite = pieceSprites[index1][index2];
							spriteGlobalBounds = newPieceSprite.getGlobalBounds();
							dragOffset.x = spriteGlobalBounds.size.x / 2.0f;
							dragOffset.y = spriteGlobalBounds.size.y / 2.0f;
							r = -1;
							c = -1;
						}
					}
				}

				if (event->is<sf::Event::MouseButtonReleased>()) {

					if (isDragging && r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
						isDragging = false;
						s = int(std::round(spritesBoard[r][c].getPosition().y / squareSize));
						t = int(std::round(spritesBoard[r][c].getPosition().x / squareSize));

						if (s < 0 || s >= boardSize || t < 0 || t >= boardSize || b[r][c].getPieceType() == PieceType::EMPTY) {
							continue;
						}
						b[s][t] = b[r][c];

						if (r != s || c != t) {
							b[r][c] = empty;
						}
						spritesBoard[s][t] = spritesBoard[r][c];
						b[s][t].setCoordinates(s, t);
						r = -1;
						c = -1;
					}
					isDragging = false;
				}
				mousePosition = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window2));

				if (isDragging) {
					x = mousePosition.x - dragOffset.x;
					y = mousePosition.y - dragOffset.y;

					if (r >= 0 && r < boardSize && c >= 0 && c < boardSize) {
						b[r][c].setPosition(sf::Vector2f(x, y));
					}
				}

				if (isNewPiece) {
					x = mousePosition.x - dragOffset.x;
					y = mousePosition.y - dragOffset.y;

					if (Functions::inBounds(x, y, 0.0f, 0.0f, windowSize.x, windowSize.y)) {
						newPiece.setPosition(sf::Vector2f(x, y));
					}
				}
				window2.clear();

				for (int r = 0; r < boardSize; r++) {

					for (int c = 0; c < boardSize + 8; c++) {

						if ((r != 1 && r != 2 && c >= boardSize && c < boardSize + 8)) {
							continue;
						}
						sf::RectangleShape square({ squareSize, squareSize });
						square.setPosition(sf::Vector2f(squareSize * c, squareSize * r));

						if ((r + c) % 2 == 0) {
							square.setFillColor(sf::Color(153, 204, 255));
						}

						else {
							square.setFillColor(sf::Color(0, 0, 255));
						}
						window2.draw(square);
					}
				}
				
				for (int s = 0; s < boardSize; s++) {

					for (int t = 0; t < boardSize; t++) {

						if (b[s][t].getPieceType() != PieceType::EMPTY && (s != r || t != c)) {
							sf::FloatRect bounds = spritesBoard[s][t].getLocalBounds();
							spritesBoard[s][t].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
							spritesBoard[s][t].setPosition(Functions::convertToPosition(s, t, squareSize));
							window2.draw(spritesBoard[s][t]);
						}

						else if (s == r && t == c && b[r][c].getPieceType() != PieceType::EMPTY) {
							sf::FloatRect bounds = spritesBoard[r][c].getLocalBounds();
							spritesBoard[r][c].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
							spritesBoard[r][c].setPosition(b[r][c].getPosition());
							window2.draw(spritesBoard[r][c]);
						}
					}
				}

				for (int i = 1; i <= 2; i++) {

					for (int j = boardSize; j < boardSize + 7; j++) {
						sf::FloatRect bounds = pieceSprites[i - 1][j - 8].getLocalBounds();
						pieceSprites[i - 1][j - 8].setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
						pieceSprites[i - 1][j - 8].setPosition(Functions::convertToPosition(i, j, squareSize));
						window2.draw(pieceSprites[i - 1][j - 8]);
					}
				}

				if (isNewPiece) {
					sf::FloatRect bounds = newPieceSprite.getLocalBounds();
					newPieceSprite.setScale(sf::Vector2f(squareSize / bounds.size.x, squareSize / bounds.size.y));
					newPieceSprite.setPosition(newPiece.getPosition());
					window2.draw(newPieceSprite);
				}
				window2.display();
			}
		}
		chessBoard.setChessBoard(b);
		return;
	}
}