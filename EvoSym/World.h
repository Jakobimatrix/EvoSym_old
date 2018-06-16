/**
* @file
* @brief
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _WORLD_
#define _WORLD_
#include <boost/foreach.hpp>

#include <math.h> /* floor */
#include <vector>
#include <iostream>
#include <algorithm>
#include "DeltaWorld.h"
#include "Region.h"
#include "structs.h"
#include "Animal.h"
#include "Point2d.h"
#include "globals.h"
#include "RegionGlobals.h"
#include "functions.h"
#include "noises/simplexnoise1234.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>


class World
{
public:
	//int *ary = new int[sizeX*sizeY];
	//// ary[i][j] is then rewritten as
	//ary[i*sizeY + j]
	DeltaWorld* WorldParts[_Amount_Delta_Worlds];
	std::vector<Animal> Animals;
private:

	GlobalSingleton* _G_;
	RegionGlobals* _RG_;
	double time;

	bool init;
	bool loadFromImage;
	sf::Image WorldHeightMap;	

	TemperateZone Polar;
	TemperateZone Moderate;
	TemperateZone Subtropical;
	TemperateZone Tropical;

public:
	World(std::string imgSRC = "")
	{
		if (imgSRC != "" && WorldHeightMap.loadFromFile(imgSRC)) {
			this->loadFromImage = true;
		}
		else {
			this->loadFromImage = false;
		}
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();

		init = false;
		this->reset();
	}
	~World()
	{
		for (int i = 0; i < _Amount_Delta_Worlds; i++) {
			delete this->WorldParts[i];
		}
	}
	void World::Update();
	std::string GetTimeReadable(bool y = true, bool d = true, bool h = false, bool m = false, bool s = false);
	void reset() {
		this->time = 0;
		this->Animals.clear();
		if (!init) {
			for (int i = 0; i < _Amount_Delta_Worlds; i++) {
				this->WorldParts[i] = new DeltaWorld();
			}
		}
		else {
			for (int i = 0; i < _Amount_Delta_Worlds; i++) {
				this->WorldParts[i]->reset();
			}
		}
		this->createWorld();
		init = true;
	}
private:
	void World::setTemperateZone();
	void World::createWorld();	

	void createSetHeightPredefined();
	void createSetHeightRand();
};


#endif