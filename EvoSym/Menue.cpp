#include "Menue.h"

sf::Color Menue::menue_color			= sf::Color(40	, 40	, 40);
sf::Color Menue::button_color		= sf::Color(150	, 150	, 150);
sf::Color Menue::button_hover_color	= sf::Color(200	, 200	, 200);
sf::Color Menue::text_color			= sf::Color(5	, 5		, 5);

void Menue::defineMenueShape() {
	int n = 1;
	if (!this->colapsed) {
		n = this->buttons.size();
	}
	this->menue_shape.setSize(sf::Vector2f(this->menue_shape.getSize().x + 2.0f*this->button_margin ,(float)(n*this->textsize*2) + this->button_margin*2.0f*(float)n));
}
void Menue::addButton(std::string& text) {
	int n = this->buttons.size();
	sf::RectangleShape ButtonRect = sf::RectangleShape(sf::Vector2f(this->menue_shape.getSize().x - 2.0f * this->button_margin, (float)(this->textsize * 2)));
	ButtonRect.setPosition(sf::Vector2f(this->menue_shape.getPosition().x + this->button_margin, this->menue_shape.getPosition().y + (float)(n*this->textsize * 2) + this->button_margin*(float)n));
	Button B = Button(ButtonRect,this->button_color);
	B.setText(text, this->textsize, this->text_color);
	this->buttons.push_back(B);
	this->defineMenueShape();
}

void Menue::display(sf::RenderWindow& w) {
	w.draw(this->menue_shape);
	if (this->colapsed) {
		w.draw(this->buttons.begin()->shape);
		w.draw(this->buttons.begin()->text);
	}
	else {
		for (std::vector<Button>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++) {
			w.draw(it->shape);
			w.draw(it->text);
		}
	}
}
int Menue::click(sf::Vector2i& mousePosition) {
	int i = 0;
	if (this->colapsed) {
		if (this->buttons.begin()->click(mousePosition, this->button_hover_color)) {
			this->colapsed = false;
			this->defineMenueShape();
			return 0;
		}
	}
	else {
		for (std::vector<Button>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++) {
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
	for (std::vector<Button>::iterator it = this->buttons.begin(); it != this->buttons.end(); it++) {
		if (it->hover(mousePosition, this->button_hover_color)) {
			hover = true;
		}
	}
	return hover;
}