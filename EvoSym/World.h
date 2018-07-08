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
	std::vector<DeltaWorld> WorldParts;
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
		this->WorldParts.reserve(_AMOUNT_DELTA_WORLDS);
		if (imgSRC != "" && WorldHeightMap.loadFromFile(imgSRC)) {
			this->loadFromImage = true;
		}
		else {
			this->loadFromImage = false;
		}
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();

		this->init = false;
		this->reset();
	}
	~World()
	{
		std::vector<DeltaWorld>().swap(WorldParts);
	}
	void World::Update();
	std::string GetTimeReadable(bool y = true, bool d = true, bool h = false, bool m = false, bool s = false);
	void reset() {
		this->time = 0;
		this->Animals.clear();
		if (!this->init) {
			for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
				this->WorldParts.push_back(DeltaWorld());
			}
		}
		else {
			for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
				this->WorldParts.at(i).reset();
			}
		}
		this->init = this->createWorld();
	}
	bool getIsReady();
private:
	void World::setTemperateZone();
	bool World::createWorld();	

	void createSetHeightPredefined();
	void createSetHeightRand();

	void getNeigbourRegionId(int neigbourRegionId[],  int x, int y, const int num_neigbours = 8);
	void getNeigbourRegionId(int neigbourRegionId[], int at, const int num_neigbours = 8);
};


#endif