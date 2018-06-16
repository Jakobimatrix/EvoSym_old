#include "Menue.h"

sf::Color Menue::MenueColor			= sf::Color(40	, 40	, 40);
sf::Color Menue::ButtonColor		= sf::Color(150	, 150	, 150);
sf::Color Menue::ButtonHoverColor	= sf::Color(200	, 200	, 200);
sf::Color Menue::TextColor			= sf::Color(5	, 5		, 5);

void Menue::defineMenueShape() {
	int n = 1;
	if (!this->colapsed) {
		n = this->Buttons.size();
	}
	this->MenueShape.setSize(sf::Vector2f(this->MenueShape.getSize().x + 2.0f*this->buttonMargin ,(float)(n*this->textsize*2) + this->buttonMargin*2.0f*(float)n));
}
void Menue::addButton(std::string& text) {
	int n = this->Buttons.size();
	sf::RectangleShape ButtonRect = sf::RectangleShape(sf::Vector2f(this->MenueShape.getSize().x - 2.0f * this->buttonMargin, (float)(this->textsize * 2)));
	ButtonRect.setPosition(sf::Vector2f(this->MenueShape.getPosition().x + this->buttonMargin, this->MenueShape.getPosition().y + (float)(n*this->textsize * 2) + this->buttonMargin*(float)n));
	Button B = Button(ButtonRect,this->ButtonColor);
	B.setText(text, this->textsize, this->TextColor);
	this->Buttons.push_back(B);
	this->defineMenueShape();
}

void Menue::display(sf::RenderWindow& w) {
	w.draw(this->MenueShape);
	if (this->colapsed) {
		w.draw(this->Buttons.begin()->shape);
		w.draw(this->Buttons.begin()->text);
	}
	else {
		for (std::vector<Button>::iterator it = this->Buttons.begin(); it != this->Buttons.end(); it++) {
			w.draw(it->shape);
			w.draw(it->text);
		}
	}
}
int Menue::click(sf::Vector2i& mousePosition) {
	int i = 0;
	if (this->colapsed) {
		if (this->Buttons.begin()->click(mousePosition, this->ButtonHoverColor)) {
			this->colapsed = false;
			this->defineMenueShape();
			return 0;
		}
	}
	else {
		for (std::vector<Button>::iterator it = this->Buttons.begin(); it != this->Buttons.end(); it++) {
			if (it->click(mousePosition)) {
				if (i == 0) {
					this->colapsed = true;
					this->defineMenueShape();
				}
				return i;
			}
			i++;
		}
	}
	return -1;
}

bool Menue::hover(sf::Vector2i& mousePosition) {
	bool hover = false;
	for (std::vector<Button>::iterator it = this->Buttons.begin(); it != this->Buttons.end(); it++) {
		if (it->hover(mousePosition, this->ButtonHoverColor)) {
			hover = true;
		}
	}
	return hover;
}