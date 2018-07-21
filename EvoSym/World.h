/**
* @file World.h
* @brief This Class creates a x*x wide 2D World with plants and birds and rocks and things.
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

struct xyMap {
	unsigned int x;
	unsigned int y;
};
class World
{
public:
	//int *ary = new int[sizeX*sizeY];
	//// ary[i][j] is then rewritten as
	//ary[i*sizeY + j]
	std::vector<DeltaWorld> WorldParts;
	std::vector<Animal> Animals;
	std::vector<xyMap> at2xy_LOOKUPTABLE; //to find teh x y coordnate if just iterating through the WorldParts Vector.
private:

	GlobalSingleton* _G_;
	RegionGlobals* _RG_;
	double time;

	bool init;
	bool loadFromImage;
	sf::Image WorldHeightMap;

	double offset_x = 0;
	double offset_y = 0;

public:

	/**
	* @function World(std::string imgSRC = "")
	* @brief: Constructor. Sets up a new World depending on the GLOBAL Variables.
	* @param[in] std::string imgSRC: A path to a black white image as a relief map. If empty then the world is created with a perlian pseudo randoom pattern.
	**/
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

	/**
	* @function ~World()
	* @brief: Destructor, deletes the vectors.
	**/
	~World()
	{
		std::vector<DeltaWorld>().swap(WorldParts);
		std::vector<xyMap>().swap(at2xy_LOOKUPTABLE);
	}

	/**
	* @function Update()
	* @brief: Simulates everything for the next time step. Timestap duration depends on the smallest timestap announced to the GlobalSigleton.
	**/
	void World::Update();

	/**
	* @function GetTimeReadable(bool y = true, bool d = true, bool h = false, bool m = false, bool s = false);
	* @brief: Writes a String of the simulated time, depending on the input.
	* @param[in] bool y: Set true if years shall be displayed
	* @param[in] bool d: Set true if days shall be isplayed
	* @param[in] bool h: Set true if hours shall be displayed
	* @param[in] bool m: Set true if minutes shall be displayed
	* @param[in] bool s: Set true if seconds shall be displayed
	* @retval std::string: a String of the simulated time.
	**/
	std::string GetTimeReadable(bool y = true, bool d = true, bool h = false, bool m = false, bool s = false);

	/**
	* @function reset()
	* @brief: Resets every deltaWorld within the world. If no Picture path for a relief map was set, the new world will be a new randoom one.
	**/
	void reset();

	/**
	* @function getIsReady();
	* @brief: If everything within the world was successfully initiated, this is true.
	* @retval bool: True, if the world is ready for simulaton.
	**/
	bool getIsReady();
private:

	/**
	* @function bool createWorld()
	* @brief: Creates a new world by setting all the delta Worlds.
	* @retval bool: True if everything was successfull.
	**/
	bool createWorld();	

	/**
	* @function createSetHeightPredefined()
	* @brief: Set all Delta Worlds height according to the given relief map. Randoom Regions according to setted height and latitude will be set.
	**/
	void createSetHeightPredefined();

	/**
	* @function createSetHeightRand()
	* @brief: Set all Delta Worlds height using perlian noise. Randoom Regions according to setted height and latitude will be set.
	**/
	void createSetHeightRand();

	/**
	* @function getNeigbourRegionIdXY(int neigbourRegionId[], int x, int y, const int num_neigbours = 8);
	* @brief: Returns the region ids of all 8 Neigbours of a given Delta World by its x y position in an virtual 2d vector.
	* @param[in] int x: x position of the Region.
	* @param[in] int y: y position of the Region.
	* @param[out] int neigbourRegionId[]: The 8 neigbours region ids. If x y was at the border of the world, the not existing neigbours region ids will be -1.
	**/
	void getNeigbourRegionIdXY(int neigbourRegionId[], int x, int y, const int num_neigbours = 8);

	/**
	* @function getNeigbourRegionIdAT(int neigbourRegionId[], int at, const int num_neigbours = 8);
	* @brief: Returns the region ids ofall 8 Neigbours of a given Delta World by its position within the vector of this->DeltaWorld.
	* @param[in] at: Position in vector.
	* @param[out] int neigbourRegionId[]: The 8 neigbours region ids. If x y was at the border of the world, the not existing neigbours region ids will be -1.
	**/
	void getNeigbourRegionIdAT(int neigbourRegionId[], int at, const int num_neigbours = 8);

	/**
	* @function getNeigbourTempXY(double temp, double neigbourTemp[], int x, int y, const int num_neigbours = 8);
	* @brief: Returns temperature of all 8 Neigbours of a given Delta World by its x y position in an virtual 2d vector.
	* @param[in] int x: x position of the Region.
	* @param[in] int y: y position of the Region.
	* @param[out] int neigbourTemp[]: The 8 neigbours temperature. If x y was at the border of the world, the not existing neigbours temperature will be the mean temperature of the existing neigbours.
	**/
	void getNeigbourTempXY(double temp, double neigbourTemp[], int x, int y, const int num_neigbours = 8);

	/**
	* @function getNeigbourMeanTempXY(double temp, int x, int y);
	* @brief: Returns mean temperature of all 8 Neigbours of a given Delta World by its x y position in an virtual 2d vector.
	* @param[in] int x: x position of the Region.
	* @param[in] int y: y position of the Region.
	* @retval double: The mean temperature
	**/
	double getNeigbourMeanTempXY(double temp, int x, int y);

	/**
	* @function int getNumNeigboursSameRegion(int at, int regionId);
	* @brief: Returns the number of same Regions considering the neigbours of a given Delta World by its position within the vector of this->DeltaWorld
	* @param[in] at: Position in vector.
	* @retval int: num of same regions
	**/
	int getNumNeigboursSameRegion(int at, int regionId);
};


#endif