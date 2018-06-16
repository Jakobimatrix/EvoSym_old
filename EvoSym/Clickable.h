#ifndef _CLICKABLE_
#define _CLICKABLE_
#include <math.h>
#include <chrono>
#include <SFML/Graphics.hpp>

static std::chrono::milliseconds minDiffBetweenClicks = std::chrono::milliseconds(100);

class ClickableRectangleShape
{
protected:
	std::chrono::time_point<std::chrono::system_clock> lastClick;
public:
	sf::RectangleShape shape;
	ClickableRectangleShape() {
		this->lastClick = std::chrono::system_clock::now();
	}
	ClickableRectangleShape(sf::RectangleShape& s) {
		this->lastClick = std::chrono::system_clock::now();
		this->shape = s;
	}
	~ClickableRectangleShape() {}
	bool Over(sf::Vector2i& mousePosition) {
		if (this->shape.getPosition().x < mousePosition.x && mousePosition.x < (this->shape.getPosition().x + this->shape.getSize().x)) {
			if (this->shape.getPosition().y < mousePosition.y && mousePosition.y < (this->shape.getPosition().y + this->shape.getSize().y)) {
				return true;
			}
		}
		return false;
	}
};

class ClickableCircleShape
{
public:
	sf::CircleShape shape;

	ClickableCircleShape() {}
	ClickableCircleShape(sf::CircleShape& shape) {
		this->shape = shape;
	}
	~ClickableCircleShape() {}
	bool Over(sf::Vector2i& mousePosition) {
		sf::Vector2f distance = sf::Vector2f((float)mousePosition.x, (float)mousePosition.y) - this->shape.getPosition();
		float radius = sqrt(distance.x*distance.x + distance.y* distance.y);
		if (radius < this->shape.getRadius()) {
			return true;
		}
		return false;
	}
};
#endif // !Clickable

