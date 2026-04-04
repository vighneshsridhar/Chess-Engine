#include "TextureManager.h"
#include "ChessPiece.h"

namespace ChessGame {

	TextureManager::TextureManager() {
        float x = 20.f;
        float y = 20.f;
        ChessGame::ChessPiece whitePawn(PieceType::PAWN, PieceColor::WHITE, sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteKnight(PieceType::KNIGHT, PieceColor::WHITE, sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteBishop(PieceType::BISHOP, PieceColor::WHITE, sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteRook(PieceType::ROOK, PieceColor::WHITE, sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteQueen(PieceType::QUEEN, PieceColor::WHITE, sf::Vector2f(x, y));
        ChessGame::ChessPiece whiteKing(PieceType::KING, PieceColor::WHITE, sf::Vector2f(x, y));

        ChessGame::ChessPiece blackPawn(PieceType::PAWN, PieceColor::BLACK, sf::Vector2f(x, y));
        ChessGame::ChessPiece blackKnight(PieceType::KNIGHT, PieceColor::BLACK, sf::Vector2f(x, y));
        ChessGame::ChessPiece blackBishop(PieceType::BISHOP, PieceColor::BLACK, sf::Vector2f(x, y));
        ChessGame::ChessPiece blackRook(PieceType::ROOK, PieceColor::BLACK, sf::Vector2f(x, y));
        ChessGame::ChessPiece blackQueen(PieceType::QUEEN, PieceColor::BLACK, sf::Vector2f(x, y));
        ChessGame::ChessPiece blackKing(PieceType::KING, PieceColor::BLACK, sf::Vector2f(x, y));

        textures["WHITE_PAWN"] = whitePawn.getTexture();
        textures["WHITE_KNIGHT"] = whiteKnight.getTexture();
        textures["WHITE_BISHOP"] = whiteBishop.getTexture();
        textures["WHITE_ROOK"] = whiteRook.getTexture();
        textures["WHITE_QUEEN"] = whiteQueen.getTexture();
        textures["WHITE_KING"] = whiteKing.getTexture();

        textures["BLACK_PAWN"] = blackPawn.getTexture();
        textures["BLACK_KNIGHT"] = blackKnight.getTexture();
        textures["BLACK_BISHOP"] = blackBishop.getTexture();
        textures["BLACK_ROOK"] = blackRook.getTexture();
        textures["BLACK_QUEEN"] = blackQueen.getTexture();
        textures["BLACK_KING"] = blackKing.getTexture();
	}

    std::unordered_map<std::string, sf::Texture> TextureManager::getTextures() {
        return textures;
    }
}