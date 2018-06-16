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
	std::vector<Button> Buttons;
	sf::RectangleShape MenueShape;
	int textsize;
	float buttonMargin;
	static sf::Color MenueColor; 
	static sf::Color ButtonColor;
	static sf::Color ButtonHoverColor;
	static sf::Color TextColor;
	bool colapsed;


private:
	void defineMenueShape();

public:

	Menue() {
		this->MenueShape.setSize(sf::Vector2f(0.0f, 0.0f));
		this->buttonMargin = 0.0f;
		this->MenueShape.setFillColor(this->MenueColor);
		this->colapsed = true;
	}
	Menue(float width, float posX, float posY,std::string& menueName, int textsize = 10, float buttonMargin = 0.0f) {
		this->textsize = textsize;
		this->MenueShape.setSize(sf::Vector2f(width, 0.0f));
		this->MenueShape.setPosition(sf::Vector2f(posX, posY));
		this->MenueShape.setFillColor(this->MenueColor);
		this->buttonMargin = buttonMargin;
		this->colapsed = true;
		this->addButton(menueName);
		this->defineMenueShape();
	}
	~Menue() {
	}

	void addButton(std::string& text); 

	void display(sf::RenderWindow& w);

	int click(sf::Vector2i& mousePosition);
	bool hover(sf::Vector2i& mousePosition);
	
};


#endif // ! _MENUE_
