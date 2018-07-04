/**
* @file DeltaWorld.h
* @brief This inherits from SimulatedUnit. It is the smallest unit of the World.
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _DELTAWORLD_
#define _DELTAWORLD_

#include <math.h> /* floor */
#include <string>
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
	double* layerTemp; //array with the temperatures for all layers
	unsigned int amountLayers; //number of layers
	double lastLayerTemp; //temperature of the last layer
}Ground;

class DeltaWorld : public SimulatedUnit
{
private:
	//neigbours
	DeltaWorld* left;
	DeltaWorld* top_left;
	DeltaWorld* bottom_left;
	DeltaWorld* right;
	DeltaWorld* top_right;
	DeltaWorld* bottom_right;
	DeltaWorld* top;
	DeltaWorld* bottom;

	bool initilized;

	double InfluencedByTempZone[4];//the entrys always adds to 1 == 100%
	double latitude;

	double SeasonMultiplier[4];//-1 bis 1; //in welcher Season wir uns gerade befinden the positive numbers always add to 1 == 100%

	int regionID;//todo region pointer instead

	double height;
	double temperature; //[°C] 

	bool isFrozen;
	double iceThickness;

	Ground ground;

	double TempDropDueHeight;
	Resources resources;

	//drawing
	bool change_in_appearance;
	bool RecentFreezing;
	double RecentDeltaTemperature;
	int RecentSeason;
	bool show_info;
	//DrawDeltaWorld shape;

	GlobalSingleton* _G_;
	RegionGlobals* _RG_;

public:

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld(){
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();
		this->regionID = _RG_->getDefaultRegionID();
		this->initilized = false;
		this->iceThickness = 0.0;
		this->isFrozen = false;
		this->show_info = false;
		this->change_in_appearance = true;
		for (int i = 0; i < _AMOUNT_TEMPERATE_ZONES; i++) {
			this->InfluencedByTempZone[i] = 0;
		}
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld(Point2d& position, double latitude)
	{
		this->setPositionAndLatitude(position, latitude);
		this->show_info = false;
		this->change_in_appearance = true;
	}

	//latitude in grad
	void setPositionAndLatitude(const Point2d& position, double latitude) {
		this->iceThickness = 0.0;
		this->isFrozen = false;
		this->_G_ = &this->_G_->getInstance();
		this->_RG_ = &this->_RG_->getInstance();
		//numerical errors with latitude
		if (latitude > this->_G_->_BREITENGRAD.max) latitude = this->_G_->_BREITENGRAD.max;

		//parameter
		this->deltaT_Update = 1.0*_DAY_IN_S;//ein Tag
		this->_G_->announceDeltaTime(this->deltaT_Update);
		this->t_lastUpdate = 0.0;
		this->latitude = latitude;
		this->position = position;

		this->regionID = _RG_->getDefaultRegionID();
		this->initilized = false;

		TemperateZone::getAllTempZoneInfluence(this->InfluencedByTempZone, latitude);

		this->RecentDeltaTemperature = this->temperature;
		this->RecentSeason = this->getSeason();
		this->RecentFreezing = this->isFrozen;

		this->show_info = false;
		this->change_in_appearance = true;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void setRegion_and_height(int regionID, double h) {
		this->iceThickness = 0.0;
		this->isFrozen = false;
		this->regionID = regionID;
		this->height = h;
		this->TempDropDueHeight = 0;
		if (this->height > _BEGIN_HEIGHT_TEMP_DROP) {
			this->TempDropDueHeight = this->height * _TEMPERATURE_DROP_PER_METER;
		}
		this->temperature = 0.0;

		//resources
		
		this->resources.setMax_freshwater(this->_RG_->getRegion(this->regionID)->getMaxFreshWater());
		this->resources.setMax_plants(this->_RG_->getRegion(this->regionID)->getMaxPlants());
		this->resources.setRegeneration_freshwater(*this->_RG_->getRegion(this->regionID)->getTauFreshWater());
		this->resources.setRegeneration_plants(*this->_RG_->getRegion(this->regionID)->getTauPlants());
		this->resources.reset();

		this->change_in_appearance = true;

		//ground

		GroundProperties* ground_properties = this->_RG_->getRegion(this->regionID)->getGroundProperties(); //object exists somewhere else, dont delete!
		this->ground.amountLayers = (unsigned int)(ground_properties->groundDepth / ground_properties->groundLayerThickness); //calculate how many layers we will have
		this->ground.layerTemp = new double[this->ground.amountLayers]; //memory must be allocated

		//calculate the temperature of the last layer, assuming liniarity between min and max latitude
		double a = (ground_properties->groundLastLayerTemp[1] - ground_properties->groundLastLayerTemp[0]) / (this->_G_->_BREITENGRAD.getSpan());
		double b = ground_properties->groundLastLayerTemp[0] - a*this->_G_->_BREITENGRAD.min;
		this->ground.lastLayerTemp = a*this->latitude + b;

		//initiate all layers with the temp of the last layer
		for (unsigned int i = 0; i < this->ground.amountLayers; i++) {
			this->ground.layerTemp[i] = this->ground.lastLayerTemp;
		}
		this->initilized = true;		
	}
	~DeltaWorld(){	
		delete[] this->ground.layerTemp;
	}


	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	std::string DeltaWorld::getUnitName() { return std::to_string(this->position.x) + "X " + std::to_string(this->position.x) + "Y"; }

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void DeltaWorld::setNeigbours(DeltaWorld* left, DeltaWorld* right, DeltaWorld* top, DeltaWorld* bottom, DeltaWorld* bottom_right, DeltaWorld* bottom_left, DeltaWorld* top_left, DeltaWorld* top_right);

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	Resources* DeltaWorld::ShowResources() { return &this->resources; }

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool isInitialized() {
		if (this == nullptr) {
			return false;
		}
		return this->initilized;
	}

	/**
	* @function void getAllTempInfluence(double(&TempZonenfluence)[4])
	* @brief Writes in the given Array the Influences of the temperature.
	* @param[out] TempZonenfluence: Attay with the Temp. influences
	* @retval void:
	**/
	void getAllTempInfluence(double(&TempZonenfluence)[4]) {
		TempZonenfluence[0] = this->InfluencedByTempZone[0];
		TempZonenfluence[1] = this->InfluencedByTempZone[1];
		TempZonenfluence[2] = this->InfluencedByTempZone[2];
		TempZonenfluence[3] = this->InfluencedByTempZone[3];
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	double getTempZoneInfluence(int TempZoneId) {
		if (TempZoneId > -1 && TempZoneId < _AMOUNT_TEMPERATE_ZONES) {
			return this->InfluencedByTempZone[TempZoneId];
		}
		return 0.0;		
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	double getTemp() {
		return this->temperature;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	double getHeight() {
		return this->height;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	int getRegionId() {
		return this->regionID;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld* getLeftNeigbour() {
		return this->left;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld* getRightNeigbour() {
		return this->right;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld* getTopNeigbour() {
		return this->top;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld* getBottomNeigbour() {
		return this->bottom;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	DeltaWorld* getNeigbour(int i) {
		switch (i)
		{
		case 0:
			return this->left;
			break;
		case 1: 
			return this->right;
			break;
		case 2:
			return this->top;
			break;
		case 3:
			return this->bottom;
			break;
		case 4:
			return this->top_left;
			break;
		case 5:
			return this->top_right;
			break;
		case 6:
			return this->bottom_left;
			break;
		case 7:
			return this->bottom_right;
			break;
		default:
			return this->left;
			break;
		}
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	int getSeason() {
		int season = 0;
		double max = 0;
		for (int i = 0; i < _AMOUNT_SEASONS; i++) {
			if (this->SeasonMultiplier[i] > max) {
				max = this->SeasonMultiplier[i];
				season = i;
			}
		}
		return season;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void setSeason(double angle) {
		this->SeasonMultiplier[0] = sin(angle);
		this->SeasonMultiplier[1] = cos(angle);
		double vzSin = 1.0;
		double vzCos = 1.0;
		if (this->SeasonMultiplier[0] < 0) {
			vzSin = -1.0;
		}
		if (this->SeasonMultiplier[1] < 0) {
			vzCos = -1.0;
		}
		this->SeasonMultiplier[0] = vzSin * this->SeasonMultiplier[0] * this->SeasonMultiplier[0];
		this->SeasonMultiplier[1] = vzCos * this->SeasonMultiplier[1] * this->SeasonMultiplier[1];
		this->SeasonMultiplier[2] = -this->SeasonMultiplier[0];
		this->SeasonMultiplier[3] = -this->SeasonMultiplier[1];
		return;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	int getNumNeigboursWithSameRegion() {
		int num = 0;
		if (this->left != nullptr && this->left->isInitialized()) {
			if (this->left->getRegionId() == this->regionID) {
				num++;
			}
		}
		if (this->right != nullptr && this->right->isInitialized()) {
			if (this->right->getRegionId() == this->regionID) {
				num++;
			}
		}
		if (this->top != nullptr && this->top->isInitialized()) {
			if (this->top->getRegionId() == this->regionID) {
				num++;
			}
		}
		if (this->bottom != nullptr && this->bottom->isInitialized()) {
			if (this->bottom->getRegionId() == this->regionID) {
				num++;
			}
		}
		return num;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void changeRegionToFitNeigbours();

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void setRandRegion(double height, int notThisRegion = -1);

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool hadChangeInAppearance(int representation) {
		if (this->getAge_s() < 2 * this->_G_->_DeltaTime) {
			return true;
		}
		else if (this->change_in_appearance) {
			return true;
			this->change_in_appearance = false;
		}
		if(this->wasUpdated()){
			//1=region/2=height/3=temp/4=season/5=tempZone
			switch (representation)
			{
			case 1:
				return this->hasPassedFreezingPoint();
				break;
			case 3:
				return this->hadA_RecentChangeInTemp();
				break;
			case 4:
				return this->hadChangeInSeason();
				break;
			default:
				return false;
			}
		}
		return false;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool hasPassedFreezingPoint() {

		if (this->isFrozen != this->RecentFreezing) {
			this->RecentFreezing = this->isFrozen;
			return true;
		}
		return false;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool hadA_RecentChangeInTemp() {
		if (std::abs(this->RecentDeltaTemperature) > _VISUALIZED_TEMPERATURE_DIFFERENCE) {
			this->RecentDeltaTemperature = 0.0;
			return true;
		}
		return false;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool hadChangeInSeason() {
		if (this->getSeason() != this->RecentSeason) {
			this->RecentSeason = this->getSeason();
			return true;
		}
		return false;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool GroundFrozen() {
		return this->isFrozen;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void reset() {
		this->initilized = false;
		this->temperature = 0.0;
		this->t_lastUpdate = -1.0;
		this->t0 = 0.0;
		this->iceThickness = 0.0;
		this->isFrozen = false;
		this->resources.reset();
	}

	bool showInfo() {
		return this->show_info;
	}
	void setShowInfo(bool b) {
		this->show_info = b;
		this->change_in_appearance = true;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	std::string getInfoString();

	std::string getSeasonText() {
		int season = 0;
		double max = 0;
		for (int i = 0; i < _AMOUNT_SEASONS; i++) {
			if (this->SeasonMultiplier[i] > max) {
				max = this->SeasonMultiplier[i];
				season = i;
			}
		}
		switch (season) {
		case 0:
			return "spring";
		case 1:
			return "summer";
		case 2:
			return "autumn";
		case 3:
			return "winter";
		default:
			return "ERROR";
		}
	}

	double getIsPlant() {
		return this->resources.getIsPlants();
	}

private:
	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void update(double tnow);

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	void calcTemp(double dt);
	void calcResources(double dt);
	void calcIceThicknes(double dt);

	/**
	* @functionvoid update(double dt, double airTemp)
	* @brief Calculates the temperature for each layer in dependancy of the air temperature.
	* @param[in] dt: The time passed since the last update. This should not exceed 5 Days.
	* @param[in] airTemp: The curret temperature of the ait above the ground.
	**/
	void calcGroundTemp(double dt, double airTemp);

};


#endif