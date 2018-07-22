#ifndef _GlobalDraw_
#define _GlobalDraw_
#include <SFML\Graphics.hpp>
#include "globals.h"

//https://stackoverflow.com/questions/86582/singleton-how-should-it-be-used
/*
GlobalDrawSingleton* G;
G = &G->getInstance();
*/
class GlobalDrawSingleton
{
public:
	sf::Font arial;

	//tempFarbGradient
	double graient_rot;
	double gradient_blau;
	double rot_offset;
	double blau_offset;
	double gradient_green_neg;
	double gradient_green_pos;

	//HeighFarbGradient
	double min_height_rgb;
	double max_height_rgb;
	double gradient_min_max_height;
	double height_offset;

	//Resources Color
	double resource_green_gradient;
	double resource_green_offset;

	//Region specific color
	sf::Color color_ocean;
	sf::Color color_oceanf;
	sf::Color color_lake;
	sf::Color color_lakef;
	sf::Color color_meadow;
	sf::Color color_meadowf;
	sf::Color color_forest;
	sf::Color color_forestf;
	sf::Color color_steppe;
	sf::Color color_desert;
	sf::Color color_moor;
	sf::Color color_moorf;
	sf::Color color_tropical_forest;
	sf::Color color_ice_desert;
	sf::Color color_mountains;
	sf::Color color_mountainsf;


private:
	// Private Constructor
	GlobalDrawSingleton() {
		this->arial.loadFromFile("arial.ttf");

		//tempFarbGradient
		this->toggleTempColor(80.0, -80.0);
		//resourcesFarbGradient
		this->toggleResourcePlantColor(10.0, 0.0);
		//HeighFarbGradient
		this->min_height_rgb = 0.0;
		this->max_height_rgb = 255.0;
		this->gradient_min_max_height = -(this->max_height_rgb - this->min_height_rgb) / (float)(_MAX_TERRANE_HEIGHT - _MIN_TERRANE_HEIGHT);
		this->height_offset = this->max_height_rgb - this->gradient_min_max_height*(float)_MAX_TERRANE_HEIGHT;

		this->color_ocean = sf::Color(23, 50, 79);
		this->color_oceanf = sf::Color(173, 203, 237);
		this->color_lake = sf::Color(0, 125, 178);
		this->color_lakef = sf::Color(197, 234, 249);
		this->color_meadow = sf::Color(0, 230, 76);
		this->color_meadowf = sf::Color(181, 242, 201);
		this->color_forest = sf::Color(0, 112, 37);
		this->color_forestf = sf::Color(132, 163, 142);
		this->color_steppe = sf::Color(209, 159, 41);
		this->color_desert = sf::Color(219, 219, 0);
		this->color_moor = sf::Color(88, 119, 70);
		this->color_moorf = sf::Color(229, 255, 214);
		this->color_tropical_forest = sf::Color(77, 137, 42);
		this->color_ice_desert = sf::Color(250, 250, 250);
		this->color_mountains = sf::Color(181, 181, 181);
		this->color_mountainsf = sf::Color(244, 242, 242);

	}
	// Stop the compiler generating methods of copy the object
	GlobalDrawSingleton(GlobalDrawSingleton const& copy);            // Not Implemented
	GlobalDrawSingleton& operator=(GlobalDrawSingleton const& copy); // Not Implemented

public:
	static GlobalDrawSingleton& getInstance()
	{
		// The only instance
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static GlobalDrawSingleton instance;
		return instance;
	}

	void toggleTempColor(double max, double min) {
		this->graient_rot = 255.0 / (max - min);
		this->gradient_blau = 255.0/ (min - max);
		this->rot_offset = 255.0 - this->graient_rot*max;
		this->blau_offset = this->rot_offset;
		this->gradient_green_neg = -this->gradient_blau;
		this->gradient_green_pos = -this->graient_rot;
	}
	void toggleResourcePlantColor(double max, double min) {
		this->resource_green_gradient = 255.0 / (max - min);
		this->resource_green_offset = 255.0 - this->resource_green_gradient*max;
	}
};

#endif // !_GlobalDraw_
