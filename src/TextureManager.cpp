#include "TextureManager.h"
#include "ChessPiece.h"

namespace ChessGame {

	TextureManager::TextureManager() {
	}

    std::unordered_map<std::string, sf::Texture> TextureManager::getTextures() {
        std::unordered_map<std::string, sf::Texture> textures;
        int x = -1;
        int y = -1;

        ChessGame::ChessPiece whitePawn(PieceType::PAWN, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteKnight(PieceType::KNIGHT, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteBishop(PieceType::BISHOP, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteRook(PieceType::ROOK, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteQueen(PieceType::QUEEN, PieceColor::WHITE, x, y);
        ChessGame::ChessPiece whiteKing(PieceType::KING, PieceColor::WHITE, x, y);

        ChessGame::ChessPiece blackPawn(PieceType::PAWN, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackKnight(PieceType::KNIGHT, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackBishop(PieceType::BISHOP, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackRook(PieceType::ROOK, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackQueen(PieceType::QUEEN, PieceColor::BLACK, x, y);
        ChessGame::ChessPiece blackKing(PieceType::KING, PieceColor::BLACK, x, y);

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

        return textures;
    }
}