/**
* @file Menue.h
* @brief: This class contains the Menue class which can be used to create a drop down Menue. It uses the Button class.
* @date 10.07.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _MENUE_
#define _MENUE_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <vector>
#include <string>
#include "Button.h"


class Menue
{
	std::vector<Button> buttons;
	sf::RectangleShape menue_shape;
	int textsize;
	float button_margin;
	static sf::Color menue_color; 
	static sf::Color button_color;
	static sf::Color button_hover_color;
	static sf::Color text_color;
	bool colapsed;


private:
	void defineMenueShape();

public:

	/**
	* @function Menue()
	* @brief: Default constructor. Sets a default color but no position or shape.
	**/
	Menue() {
		this->menue_shape.setSize(sf::Vector2f(0.0f, 0.0f));
		this->button_margin = 0.0f;
		this->menue_shape.setFillColor(this->menue_color);
		this->colapsed = true;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] float width: Width of the menue.
	* @param[in] float posX: Top corner x position.
	* @param[in] float posY: Top corner y position.
	* @param[in] std::string& menueName: Text displayed on the menue button.
	* @param[in] int textsize: Text size of the menue buttons.
	* @param[in] float buttonMargin: Margin between the menue buttons.
	* @retval int:
	**/
	Menue(float width, float posX, float posY, std::string& menueName, int textsize = 10, float buttonMargin = 0.0f) {
		this->textsize = textsize;
		this->menue_shape.setSize(sf::Vector2f(width, 0.0f));
		this->menue_shape.setPosition(sf::Vector2f(posX, posY));
		this->menue_shape.setFillColor(this->menue_color);
		this->button_margin = buttonMargin;
		this->colapsed = true;
		this->addButton(menueName);
		this->defineMenueShape();
	}
	~Menue() {
	}

	/**
	* @function addButton(std::string& text)
	* @brief: Adds a button to the menue. The order whit which the buttons are added, is the order within the drop down menue.
	* @param[in] std::string& text: The Text for the new button.
	**/
	void addButton(std::string& text); 

	/**
	* @function display(sf::RenderWindow& w)
	* @brief: Adds the menue to the render Window to be dawn.
	* @param[in] sf::RenderWindow& w: SFML::window in which the menue shall be displayed.
	**/
	void display(sf::RenderWindow& w);

	/**
	* @function int click(sf::Vector2i& mousePosition);
	* @brief: returns the button number which was clicked.
	* @param[in] sf::Vector2i& mousePosition: The mouse position.
	* @retval int: returns the button number which was clicked or -1 if nothing as clicked.
	**/
	int click(sf::Vector2i& mousePosition);

	/**
	* @function bool hover(sf::Vector2i& mousePosition);
	* @brief: Calculates if a mouse hovers over the menue.
	* @param[in] sf::Vector2i& mousePosition: The mouse position.
	* @retval int: returns true, if the mouse hovers over the menue.
	**/
	bool hover(sf::Vector2i& mousePosition);
	
};


#endif // ! _MENUE_
