/**
* @file DeltaWorldInformation.h
* @brief 
* @date 10.11.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _DELTAWORLDINFO_
#define _DELTAWORLDINFO_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>

class DeltaWorldInformation {
private:
	sf::Text info;
	sf::RectangleShape infoBox;
public:
	DeltaWorldInformation() {
	this->infoBox.setFillColor(sf::Color(0, 0, 0));
	this->info.setCharacterSize(10);
	};

	sf::RectangleShape& draw() {
		return this->infoBox;
	}

	void detInfoText(std::string text) {
		this->infoBox.
	}

};

#endif
