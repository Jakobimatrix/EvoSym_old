/**
* @file Button.h
* @brief: This class contains the Button class which can be used to create Buttons with SFML. It inherits from Clickabe.h and is used in Menue.h
* @date 10.07.2017
* @author Jakob Wandel
* @version 1.0
**/
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
	GlobalDrawSingleton* _G_DRAW_; //global drawing information
	sf::Text text; //the text on the button
	sf::Color button_color; //the colour ot the button

	/**
	* @function Button()
	* @brief: Default constructor. Sets a default color but no position or shape.
	**/
	Button() {
		this->_G_DRAW_ = &this->_G_DRAW_->getInstance();
		this->button_color = sf::Color(200, 200, 200);
	}

	/**
	* @function Button(sf::RectangleShape& Rshape, sf::Color& ButtonColor = sf::Color(200, 200, 200)):ClickableRectangleShape(Rshape)
	* @brief: Constructor: Sets colour, shape and position.
	* @param[in] sf::RectangleShape& Rshape: A sfml::Rectangle containing position and size of the Button.
	* @param[in] sf::Color& ButtonColor: The color for the button.
	**/
	Button(sf::RectangleShape& Rshape, sf::Color& ButtonColor = sf::Color(200, 200, 200)):ClickableRectangleShape(Rshape)
	{
		this->_G_DRAW_ = &this->_G_DRAW_->getInstance();
		//this->shape = Rshape;
		this->button_color = ButtonColor;
		this->shape.setFillColor(ButtonColor);
	}

	/**
	* @function hover(sf::Vector2i& mousePosition, sf::Color& hooverColor = sf::Color(250, 250, 250))
	* @brief: Calculates if a mouse hovers over the button.
	* @param[in] sf::Vector2i& mousePosition: The mouse position.
	* @param[in] sf::Color& hooverColor: The color the button shall have, if its hovered.
	* @retval bool: returns true if the given mouseposition is over the button.
	**/
	bool hover(sf::Vector2i& mousePosition, sf::Color& hooverColor = sf::Color(250, 250, 250)) {
		if (this->Over(mousePosition)) {
			this->shape.setFillColor(sf::Color(this->button_color));
			return true;
		}
		else {
			this->shape.setFillColor(sf::Color(hooverColor));
			return false;
		}
	}

	/**
	* @function click(sf::Vector2i& mousePosition, sf::Color& hooverColor = sf::Color(250, 250, 250))
	* @brief: Calculates if a mouse has clicked this button. (using "debouncing")
	* @param[in] sf::Vector2i& mousePosition: The mouse position.
	* @param[in] name: sf::Color& hooverColor the color the button shall have, if its clicked.
	* @retval bool: returns true if the given mouseposition is over the button and this function wasnt called within minDiffBetweenClicks twice ore more.
	**/
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

	/**
	* @function setText(std::string& text, int charSize = 20, sf::Color& TextColor = sf::Color(0, 0, 0)) {
	* @brief: Sets the Text for the button.
	* @param[in] std::string& text: The text.
	* @param[in] int charSize: The text size.
	* @param[in] sf::Color& TextColor: The text color.
	**/
	void setText(std::string& text, int charSize = 20, sf::Color& TextColor = sf::Color(0, 0, 0)) {
		this->text.setString(text);
		this->text.setPosition(this->shape.getPosition().x + this->shape.getSize().x *0.5f, this->shape.getPosition().y + this->shape.getSize().y * 0.5f);
		this->text.setFont(_G_DRAW_->arial);
		this->text.setFillColor(TextColor);
		this->text.setCharacterSize(charSize);
		this->text.setOrigin(this->text.getLocalBounds().width * 0.5f, this->text.getLocalBounds().height);
	}

	/**
	* @function resize(float ratio)
	* @brief: resizes the Button.
	* @param[in] float ratio: if smaller 1 it gets smaller, if bigger 1 it gets bigger.
	**/
	void resize(float ratio) {
		this->text.setScale(sf::Vector2f(ratio, ratio));
		this->shape.setScale(sf::Vector2f(ratio, ratio));
	}
};
#endif // !_BUTTON_
