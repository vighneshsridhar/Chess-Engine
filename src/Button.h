#ifndef ChessGame_Button_h
#define ChessGame_Button_h


#include <SFML/Graphics.hpp>

#include "Functions.h"


namespace ChessGame {

	class Button {
	public:
		Button(float width, float height, float x, float y, bool mode);
		sf::RectangleShape getButton();
		sf::Vector2f getPosition();
		void setPosition(float x, float y);
		void setFillColor(float mx, float my);
		sf::Vector2f getSize();
		bool getMode();
		void setMode(bool m);
		bool mouseOverButton(float mx, float my) const;

	private:
		sf::RectangleShape button;
		sf::Vector2f position;
		sf::Vector2f size;
		sf::Color colorNormal;
		sf::Color colorHover;
		sf::Color colorPressed;
		bool mode;
	};
}

#endif