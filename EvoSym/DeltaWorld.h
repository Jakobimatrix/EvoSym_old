/**
* @file DeltaWorld.h
* @brief: This inherits from SimulatedUnit. It is the smallest unit of the World.
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _DELTAWORLD_
#define _DELTAWORLD_

#include <math.h> /* floor */
#include <string>
#include <vector>
#include <SFML\Graphics.hpp>
#include "Clickable.h"
#include "globals.h"
#include "SimulatedUnit.h"
#include "Point2d.h"
#include "TemperateZone.h"
#include "Resources.h"
#include "Region.h"
#include "RegionGlobals.h"


typedef struct Ground {
	std::vector<double> layer_temperature; //array with the temperatures for all layers
	unsigned int amount_layers; //number of layers
	double last_layer_temperature; //temperature of the last layer
	~Ground(){
		std::vector<double>().swap(layer_temperature);//That will create an empty vector with no memory allocated and swap it with layerTemp, effectively deallocating the memory.
	}
}Ground;

class DeltaWorld : public SimulatedUnit
{
private:
	
	double mean_neigbour_temperature; //saves the last mean temperature of the neigbours

	bool initilized; 

	double temperature_zone_influence[4];//the entrys always adds to 1 == 100%
	double latitude;//between 0 and 90

	Region* region; //pointer to the region in the GlobalSigleton

	double height;
	double temperature; //[°C] 

	bool is_frozen;
	double ice_thickness; //[m]

	Ground ground;

	double temperature_drop_due_height;
	Resources resources;

	//drawing
	bool change_in_appearance; //if true, this delta world must be redrawn
	bool recent_freezing; 
	double recent_delta_temperature;
	int recent_season;
	bool show_info;
	//DrawDeltaWorld shape;

	GlobalSingleton* _G_;
	RegionGlobals* _RG_;

public:

	/**
	* @function DeltaWorld()
	* @brief: Default constructor
	**/
	DeltaWorld(){
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();
		this->region = _RG_->getUninitilized();
		this->initilized = false;
		this->ice_thickness = 0.0;
		this->is_frozen = false;
		this->show_info = false;
		this->change_in_appearance = true;
		for (int i = 0; i < _AMOUNT_TEMPERATE_ZONES; i++) {
			this->temperature_zone_influence[i] = 0;
		}
	}

	/**
	* @function DeltaWorld(Point2d& position, double latitude)
	* @brief: Constructor, sets position and latitude within the world.
	* @param[in] Point2d& position: The Position in meter within the world.
	* @param[in] double latitude:	The latitude within this world.
	**/
	DeltaWorld(Point2d& position, double latitude)
	{
		this->region = _RG_->getUninitilized();
		this->initSetPositionAndLatitude(position, latitude);
		this->show_info = false;
		this->change_in_appearance = true;
	}

	/**
	* @function initSetPositionAndLatitude(const Point2d& position, double latitude)
	* @brief: Part of the initialization: Sets the Position and the latitude and all things which depends on thouse parameters.
	* @param[in] Point2d& position: The Position in meter within the world.
	* @param[in] double latitude:	The latitude within this world.
	**/
	void initSetPositionAndLatitude(const Point2d& position, double latitude) {
		this->ice_thickness = 0.0;
		this->is_frozen = false;
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();
		this->region = _RG_->getUninitilized();
		//numerical errors with latitude
		if (latitude > this->_G_->_BREITENGRAD.max) latitude = this->_G_->_BREITENGRAD.max;

		//parameter
		this->delta_t_update = 1.0*_DAY_IN_S;//ein Tag
		this->_G_->announceDeltaTime(this->delta_t_update);
		this->t_last_update = 0.0;
		this->latitude = latitude;
		this->position = position;

		this->initilized = false;

		TemperateZone::getAllTempZoneInfluence(this->temperature_zone_influence, latitude);

		this->recent_delta_temperature = this->temperature;
		this->recent_season = this->_G_->dominantSeason;
		this->recent_freezing = this->is_frozen;

		this->show_info = false;
		this->change_in_appearance = true;
	}

	/**
	* @function InitSetRegionAndHeight(int regionID, double h)
	* @brief: Part of the initialization: Sets the Region and the height.
	* @param[in] int regionID: The Id of the region this delta world shall have.
	* @param[out] double height: The height of this delta world in meter.
	**/
	void InitSetRegionAndHeight(Region* region, double height) {
		this->ice_thickness = 0.0;
		this->is_frozen = false;
		this->region = region;
		this->height = height;
		this->temperature_drop_due_height = 0;
		if (this->height > _BEGIN_HEIGHT_TEMP_DROP) {
			this->temperature_drop_due_height = this->height * _TEMPERATURE_DROP_PER_METER;
		}
		this->temperature = 0.0;
		this->mean_neigbour_temperature = 0.0; 


		//resources
		
		this->resources.setMax_freshwater(this->region->getMaxFreshWater());
		this->resources.setMax_plants(this->region->getMaxPlants());
		this->resources.setRegeneration_freshwater(*this->region->getTauFreshWater());
		this->resources.setRegeneration_plants(*this->region->getTauPlants());
		this->resources.reset();

		this->change_in_appearance = true;

		//ground

		GroundProperties* ground_properties = this->region->getGroundProperties(); //object exists somewhere else, dont delete!
		this->ground.amount_layers = (unsigned int)(ground_properties->ground_depth / ground_properties->ground_layer_thickness); //calculate how many layers we will have
		this->ground.layer_temperature.reserve(this->ground.amount_layers); //memory must be allocated

		//calculate the temperature of the last layer, assuming liniarity between min and max latitude
		double a = (ground_properties->ground_last_layer_temp[1] - ground_properties->ground_last_layer_temp[0]) / (this->_G_->_BREITENGRAD.getSpan());
		double b = ground_properties->ground_last_layer_temp[0] - a*this->_G_->_BREITENGRAD.min;
		this->ground.last_layer_temperature = a*this->latitude + b;

		//initiate all layers with the temp of the last layer
		for (unsigned int i = 0; i < this->ground.amount_layers; i++) {
			this->ground.layer_temperature.emplace_back(this->ground.last_layer_temperature);
		}
		this->initilized = true;		
	}
	~DeltaWorld(){	

	}


	/**
	* @function getUnitName()
	* @brief: returns the position of the delta world as its name within a string.
	* @retval std::string: position of the delta world
	**/
	std::string getUnitName() { return std::to_string(this->position.x) + "X " + std::to_string(this->position.x) + "Y"; }

	/**
	* @function Resources* ShowResources() 
	* @brief: returns a pointer of this delta world resources.
	* @retval Resources*: This delta world resources.
	**/
	Resources* ShowResources() { return &this->resources; }

	/**
	* @function bool isInitialized()
	* @brief: Returns the state of initialization.
	* @retval bool:  True if initilazed.
	**/
	bool isInitialized() {
		return this->initilized;
	}

	/**
	* @function void getAllTempInfluence(double(&TempZonenfluence)[4])
	* @brief: Writes in the given Array the Influences of the temperature.
	* @param[out] TempZonenfluence: Array with the Temp. influences
	* @retval void:
	**/
	void getAllTempInfluence(double(&TempZonenfluence)[4]) {
		TempZonenfluence[0] = this->temperature_zone_influence[0];
		TempZonenfluence[1] = this->temperature_zone_influence[1];
		TempZonenfluence[2] = this->temperature_zone_influence[2];
		TempZonenfluence[3] = this->temperature_zone_influence[3];
	}

	/**
	* @function double getTempZoneInfluence(int TempZoneId)
	* @brief: Calculates how much the given temperature zone has influence on this delta world.
	* @param[in] int TempZoneId: 0: polar, 1: mediteran, 2: subtropica, 3: tropical
	* @retval double: A value between 0 and 1, depending how much the given temperature zone influences this region.
	**/
	double getTempZoneInfluence(int TempZoneId) {
		if (TempZoneId > -1 && TempZoneId < _AMOUNT_TEMPERATE_ZONES) {
			return this->temperature_zone_influence[TempZoneId];
		}
		return 0.0;		
	}

	/**
	* @function double getTemp()
	* @brief: Returns the current temperature in celsius.
	* @retval double: The current temperature.
	**/
	double getTemp() {
		return this->temperature;
	}

	/**
	* @function double getHeight()
	* @brief: Returns the height in meter.
	* @retval double: The height in meter.
	**/
	double getHeight() {
		return this->height;
	}

	/**
	* @function int getRegionId()
	* @brief: Returns the Id of the Region of this delta world.
	* @retval int: The id of this region.
	**/
	int getRegionId() {
		return this->region->getRegionId();
	}


	/**
	* @function getSeason()
	* @brief: calculate the curent dominat season.
	* @retval int: The dominant season id.
	**/
	int getSeason() {
		return this->_G_->dominantSeason;
	}

	/**
	* @function getSeason()
	* @brief: calculate the curent dominat season.
	* @retval int: The dominant season id.
	**/
	void setNeigboursTemperature(double meanTemp);

	/**
	* @function changeRegionToFitNeigbours(int regionNeigbour[], const int num_neigbours = 8)
	* @brief: changes the current region to fit the neigbours reagion.
	* @param[in] regionNeigbour[]: Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
	**/
	void changeRegionToFitNeigbours(int regionNeigbour[], const int num_neigbours = 8);

	/**
	* @function setRandRegion(double height, int regionNeigbour[], const int num_neigbours = 8, int notThisRegion = -1);
	* @brief
	* @param[in] height: the height of this region???
	* @param[in] regionNeigbour[]: Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
	* @param[in] notThisRegion: If a specific region shall not be choosen, notThisRegion must be the id of that region.
	**/
	void setRandRegion(double height, int regionNeigbour[], const int num_neigbours = 8, int notThisRegion = -1);

	/**
	* @function bool hadChangeInAppearance()
	* @brief: Returns true, if the region mast be redrawn.
	* @retval bool: Returns true, if the region mast be redrawn.
	**/
	bool hadChangeInAppearance(int representation);

	/**
	* @function bool hasPassedFreezingPoint()
	* @brief: Calculates if there was a change in the condition of the ground.
	* @retval bool: Returns true if there was a change of the ground condition.
	**/
	bool hasPassedFreezingPoint();

	/**
	* @function bool hadRecentChangeInTemp f()
	* @brief: Returns true, if the temperature has changed more than a value, so that the region has to be redrawn.
	* @retval bool: True, if the temperature change is noticable.
	**/
	bool hadRecentChangeInTemp();

	/**
	* @function bool hadChangeInSeason()
	* @brief: Returns true, if the Season changed.
	* @retval int: True, if the Season changed.
	**/
	bool hadChangeInSeason();

	/**
	* @function bool GroundFrozen()
	* @brief: Getter: Returns if this pice of Land is frozen.
	* @retval bool: True if the Ground temperature in below freezing point.
	**/
	bool GroundFrozen() {
		return this->is_frozen;
	}

	/**
	* @function reset()
	* @brief: resets temperature and resources as well as the age.
	**/
	void reset();

	/**
	* @function bool showInfo()
	* @brief: Getter: Returns if information of this delta world should be shown.
	* @retval bool: True if this delta worlds inf should be shown.
	**/
	bool showInfo() {
		return this->show_info;
	}

	/**
	* @function setShowInfo(bool b)
	* @brief: Setter: Sets if information of this delta world should be shown.
	* @param[in] bool b: Boolean if information of this delta world should be shown.
	**/
	void setShowInfo(bool b) {
		this->show_info = b;
		this->change_in_appearance = true;
	}

	/**
	* @function std::string getInfoString()
	* @brief: Returns a String with information about this delta World.
	* @retval std::string: Information about this delta World.
	**/
	std::string getInfoString();

	/**
	* @function std::string getSeasonText()
	* @brief: Returns a string with the current season
	* @retval std::string: A string with the current season
	**/
	std::string getSeasonText();

	/**
	* @function getIsPlants()
	* @brief: getter function; returns the current value of plants in Kg/m^2 this region holds.
	* @retval double: value of plants in Kg/m^2 this region holds.
	**/
	double getIsPlant() {
		return this->resources.getIsPlants();
	}

private:
	/**
	* @function update(double tnow)
	* @brief: Inherited update function which is called by SimulatedUnit.simulate(tnow).
	* Simulates everything within the lat timestep.
	**/
	void update(double tnow);

	/**
	* @function calcTemp(double dt)
	* @brief: Calculates the new temperature when dt time has passed.
	**/
	void calcTemp(double dt);

	/**
	* @function calcResources(double dt)
	* @brief: Calculates the resources when dt time has passed.
	**/
	void calcResources(double dt);

	/**
	* @function calcIceThicknes(double dt)
	* @brief: Calculates the Ice thicknes of the ground when dt time has passed.
	**/
	void calcIceThicknes(double dt);

	/**
	* @functionvoid update(double dt, double airTemp)
	* @brief: Calculates the temperature for each layer in dependancy of the air temperature.
	* @param[in] dt: The time passed since the last update. This should not exceed 5 Days.
	* @param[in] airTemp: The curret temperature of the ait above the ground.
	**/
	void calcGroundTemp(double dt, double airTemp);

};


#endif