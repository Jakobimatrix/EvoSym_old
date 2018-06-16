#ifndef _BUTTON_
#define _BUTTON_

#include <chrono>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Clickable.h"
#include "globalDraw.h"

class Button : public ClickableRectangleShape {
public:
	GlobalDrawSingleton* G;
	sf::Text text;
	sf::Color ButtonColor;

	Button() {
		this->G = &this->G->getInstance();
		this->ButtonColor = sf::Color(200, 200, 200);
	}
	Button(sf::RectangleShape& Rshape, sf::Color& ButtonColor = sf::Color(200, 200, 200)):ClickableRectangleShape(Rshape)
	{
		this->G = &this->G->getInstance();
		this->shape = Rshape;
		this->ButtonColor = ButtonColor;
		this->shape.setFillColor(ButtonColor);
	}
	~Button(){}
	bool hover(sf::Vector2i& mousePosition, sf::Color& hooverColor = sf::Color(250, 250, 250)) {
		if (this->Over(mousePosition)) {
			this->shape.setFillColor(sf::Color(this->ButtonColor));
			return true;
		}
		else {
			this->shape.setFillColor(sf::Color(hooverColor));
			return false;
		}
	}
	bool click(sf::Vector2i& mousePosition, sf::Color& hooverColor = sf::Color(250, 250, 250)) {
		if (std::chrono::system_clock::now() - this->lastClick > minDiffBetweenClicks) {
			if (this->Over(mousePosition)) {
				this->lastClick = this->lastClick = std::chrono::system_clock::now();
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
	void setText(std::string& text, int charSize = 20, sf::Color& TextColor = sf::Color(0, 0, 0)) {
		this->text.setString(text);
		this->text.setPosition(this->shape.getPosition().x + this->shape.getSize().x *0.5f, this->shape.getPosition().y + this->shape.getSize().y * 0.5f);
		this->text.setFont(G->arial);
		this->text.setFillColor(TextColor);
		this->text.setCharacterSize(charSize);
		this->text.setOrigin(this->text.getLocalBounds().width * 0.5f, this->text.getLocalBounds().height);
	}
	void resize(float ratio) {
		this->text.setScale(sf::Vector2f(ratio, ratio));
		this->shape.setScale(sf::Vector2f(ratio, ratio));
	}
};
#endif // !_BUTTON_
