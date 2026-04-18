#pragma once
#ifndef ChessGame_TextureManager_h
#define ChessGame_TextureManager_h

#include "ChessBoard.h"
#include "ChessPiece.h"
#include "Functions.h"
#include "PromotionClicker.h"

namespace ChessGame {

	class TextureManager {
	public:
		TextureManager();
		static std::unordered_map<std::string, sf::Texture> getTextures();
	};

}

#endif
