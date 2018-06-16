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
	double GradientRot;
	double GradientBlau;
	double RotOffset;
	double BlauOffset;
	double GradientGruen_neg;
	double GradientGruen_pos;

	//HeighFarbGradient
	double minHeightRGB;
	double maxHeightRGB;
	double GradientMinMaxHeight;
	double HeightOffset;

	//Resources Color
	double resource_green_gradient;
	double resource_green_offset;

	//Region specifische Background
	sf::Texture BackgroundOcean;
	sf::Texture BackgroundOceanFrozen;
	sf::Texture BackgroundLake;
	sf::Texture BackgroundLakeFrozen;
	sf::Texture BackgroundMeadow;
	sf::Texture BackgroundMeadowFrozen;
	sf::Texture BackgroundForest;
	sf::Texture BackgroundForestFrozen;
	sf::Texture BackgroundSteppe;
	sf::Texture BackgroundDesert;
	sf::Texture BackgroundMoor;
	sf::Texture BackgroundMoorFrozen;
	sf::Texture BackgroundTropicalForest;
	sf::Texture BackgroundIceDesert;
	sf::Texture BackgroundHeighlands;
	sf::Texture BackgroundHeighlandsFrozen;
	sf::Texture BackgroundMountains;
	sf::Texture BackgroundMountainsFrozen;
	sf::Texture BackgroundBlack;
	sf::Texture BackgroundTransparent;

	//Region specific color
	sf::Color ocean;
	sf::Color oceanf;
	sf::Color lake;
	sf::Color lakef;
	sf::Color meadow;
	sf::Color meadowf;
	sf::Color forest;
	sf::Color forestf;
	sf::Color steppe;
	sf::Color desert;
	sf::Color moor;
	sf::Color moorf;
	sf::Color tropicalForest;
	sf::Color iceDesert;
	sf::Color heighlands;
	sf::Color heighlandsf;
	sf::Color mountains;
	sf::Color mountainsf;


private:
	// Private Constructor
	GlobalDrawSingleton() {
		this->arial.loadFromFile("arial.ttf");

		//tempFarbGradient
		this->toggleTempColor(80.0, -80.0);
		//resourcesFarbGradient
		this->toggleResourcePlantColor(10.0, 0.0);
		//HeighFarbGradient
		this->minHeightRGB = 0.0;
		this->maxHeightRGB = 255.0;
		this->GradientMinMaxHeight = -(this->maxHeightRGB - this->minHeightRGB) / (float)(_MAX_TERRANE_HEIGHT - _MIN_TERRANE_HEIGHT);
		this->HeightOffset = this->maxHeightRGB - this->GradientMinMaxHeight*(float)_MAX_TERRANE_HEIGHT;
		//regionSpecifische color
		this->BackgroundOcean.setSmooth(true);
		this->BackgroundOcean.loadFromFile("bilder/backgrounds/ocean.jpg");
		this->BackgroundOceanFrozen.loadFromFile("bilder/backgrounds/oceanF.jpg");
		this->BackgroundLake.loadFromFile("bilder/backgrounds/lake.jpg");
		this->BackgroundLakeFrozen.loadFromFile("bilder/backgrounds/lakeF.jpg");
		this->BackgroundMeadow.loadFromFile("bilder/backgrounds/meadow.jpg");
		this->BackgroundMeadowFrozen.loadFromFile("bilder/backgrounds/meadowF.jpg");
		this->BackgroundForest.loadFromFile("bilder/backgrounds/forest.jpg");
		this->BackgroundForestFrozen.loadFromFile("bilder/backgrounds/forestF.jpg");
		this->BackgroundSteppe.loadFromFile("bilder/backgrounds/steppe.jpg");
		this->BackgroundDesert.loadFromFile("bilder/backgrounds/desert.jpg");
		this->BackgroundMoor.loadFromFile("bilder/backgrounds/moor.jpg");
		this->BackgroundMoorFrozen.loadFromFile("bilder/backgrounds/moorF.jpg");
		this->BackgroundTropicalForest.loadFromFile("bilder/backgrounds/tropicalforest.jpg");
		this->BackgroundIceDesert.loadFromFile("bilder/backgrounds/icedesert.jpg");
		this->BackgroundHeighlands.loadFromFile("bilder/backgrounds/highlands.jpg");
		this->BackgroundHeighlandsFrozen.loadFromFile("bilder/backgrounds/highlandsF.jpg");
		this->BackgroundMountains.loadFromFile("bilder/backgrounds/mountain.jpg");
		this->BackgroundMountainsFrozen.loadFromFile("bilder/backgrounds/mountainF.jpg");
		this->BackgroundBlack.loadFromFile("bilder/backgrounds/black.jpg");
		this->BackgroundTransparent.loadFromFile("bilder/backgrounds/transparent.png");

		this->ocean = sf::Color(23, 50, 79);
		this->oceanf = sf::Color(173, 203, 237);
		this->lake = sf::Color(0, 125, 178);
		this->lakef = sf::Color(197, 234, 249);
		this->meadow = sf::Color(0, 230, 76);
		this->meadowf = sf::Color(181, 242, 201);
		this->forest = sf::Color(0, 112, 37);
		this->forestf = sf::Color(132, 163, 142);
		this->steppe = sf::Color(209, 159, 41);
		this->desert = sf::Color(219, 219, 0);
		this->moor = sf::Color(88, 119, 70);
		this->moorf = sf::Color(229, 255, 214);
		this->tropicalForest = sf::Color(77, 137, 42);
		this->iceDesert = sf::Color(250, 250, 250);
		this->heighlands = sf::Color(158, 250, 167);
		this->heighlandsf = sf::Color(177, 191, 179);
		this->mountains = sf::Color(181, 181, 181);
		this->mountainsf = sf::Color(244, 242, 242);

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
		this->GradientRot = 255.0 / (max - min);
		this->GradientBlau = 255.0/ (min - max);
		this->RotOffset = 255.0 - this->GradientRot*max;
		this->BlauOffset = this->RotOffset;
		this->GradientGruen_neg = -this->GradientBlau;
		this->GradientGruen_pos = -this->GradientRot;
	}
	void toggleResourcePlantColor(double max, double min) {
		this->resource_green_gradient = 255.0 / (max - min);
		this->resource_green_offset = 255.0 - this->resource_green_gradient*max;
	}
};

#endif // !_GlobalDraw_
