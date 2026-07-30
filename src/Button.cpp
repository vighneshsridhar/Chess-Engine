#include <SFML/Graphics.hpp>

#include "Button.h"

namespace ChessGame {

	Button::Button(float width, float height, float x, float y, bool mode) : mode(mode) {
		button.setSize(sf::Vector2f(width, height));
		button.setPosition(sf::Vector2f(x, y));
		position = button.getPosition();
		size = button.getSize();
		colorNormal = sf::Color(211, 211, 211);
		colorHover = sf::Color(230, 230, 230);
		colorPressed = sf::Color(160, 160, 160);
		button.setFillColor(colorNormal);
	}

	sf::RectangleShape Button::getButton() {
		return button;
	}

	sf::Vector2f Button::getPosition() {
		return button.getPosition();
	}
	
	void Button::setPosition(float x, float y) {
		button.setPosition(sf::Vector2f(x, y));
	}

	void Button::setFillColor(float mx, float my) {
		if (mode) {
			button.setFillColor(colorPressed);
		}

		else if (mouseOverButton(mx, my)) {
			button.setFillColor(colorHover);
		}

		else {
			button.setFillColor(colorNormal);
		}
		return;
	}

	sf::Vector2f Button::getSize() {
		return button.getSize();
	}

	bool Button::getMode() {
		return mode;
	}

	void Button::setMode(bool m) {
		mode = m;
		return;
	}

	bool Button::mouseOverButton(float mx, float my) const {
		return Functions::inBounds(mx, my, position.x, position.y, position.x + size.x, position.y + size.y);
	}
}