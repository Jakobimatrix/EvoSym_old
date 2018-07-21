/**
* @file Resources.h
* @brief: Resources (plants, water and dead animals) for each DealtaWorld are managed here.
* @date 10.07.2018
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _RESOURCES_
#define _RESOURCES_
#include <vector>
#include "globals.h"
#include "structs.h"
#include "Skills.h"
#include "Region.h"


class Resources {
private:
	Season<double> Regeneration_freshwater_1_divided_Tau;//Tau of signoide
	double max_freshwater;//Liter per m^2
	double freshwater;//ist
	Season<double> Regeneration_plants_1_divided_Tau;//Tau of signoide
	double max_plants;//Kg per m^2
	double plants;//ist

	GlobalSingleton* _G_;

public:
	/**
	* @function Resources()()
	* @brief: Default constructor, prepares class for usage.
	**/
	Resources(){
		this->_G_ = &this->_G_->getInstance();
		this->reset();
	}

	/**
	* @function void reset()
	* @brief: resets the class to its initial state

	**/
	void reset() {
		this->plants = 0.0;
		this->freshwater = 0.0;
	}

	/**
	* @function setMax_freshwater(double max_freshwater)
	* @brief: setter function; Sets the maximum amount of water this resources can hold/generate.
	* @param[in] double max_freshwater: maximum of water which can be generted, in l/m^2.
	**/
	void setMax_freshwater(double max_freshwater) {
		this->max_freshwater = max_freshwater;
	}

	/**
	* @function setRegeneration_freshwater(Season<double>& Regeneration_freshwater)
	* @brief: setter function; Sets the regeneration rate for fresh water for this resource.
	* @param[in] Season<double>& Regeneration_freshwater: For each Season one regeneration rate.
	**/
	void setRegeneration_freshwater(Season<double>& Regeneration_freshwater) {
		this->Regeneration_freshwater_1_divided_Tau = Regeneration_freshwater;
	}

	/**
	* @function RegenerateFreshWater(double deltaT)
	* @brief: Calculates the new is value of freshwater this resource holds after deltaT has passed.
	* @param[in] double deltaT: Time in seconds passed between this and last function call.
	**/
	void RegenerateFreshWater(double deltaT);

	/**
	* @function setMax_plants(double max_plants) 
	* @brief: setter function; Set the max. amount of plants this resource gan hold/generate.
	* @param[in] double max_plants: Max amount of plants this resource can hold in Kg/m^2
	**/
	void setMax_plants(double max_plants) {
		this->max_plants = max_plants;
	}

	/**
	* @function setRegeneration_plants(Season<double>& Regeneration_plants)
	* @brief: setter function; Sets the regeneration rate for plants for this resource.
	* @param[in] Season<double>& Regeneration_plants: For each Season one regeneration rate.
	**/
	void setRegeneration_plants(Season<double>& Regeneration_plants) {
		this->Regeneration_plants_1_divided_Tau = Regeneration_plants;
	}

	/**
	* @function RegeneratePlants(double deltaT, int regionID, double temperature)
	* @brief: Calculates the new is value of plants this resource holds after deltaT has passed,
	* depending on the temperature and the region.
	* @param[in] double deltaT: Time in seconds passed between this and last function call.
	* @param[in] int regionID: The Id of the region this resource is in.
	* @param[in] double temperature: Current temperature in celsius.
	**/
	void RegeneratePlants(double deltaT, int regionID, double temperature);

	/**
	* @function getMaxPlants()
	* @brief: getter function; returns the value of max_plants in Kg/m^2 this region can hold.
	* @retval double: value of max_plants in Kg/m^2 this region can hold.
	**/
	double getMaxPlants() {
		return this->max_plants;
	}

	/**
	* @function getIsPlants()
	* @brief: getter function; returns the current value of plants in Kg/m^2 this resource holds.
	* @retval double: value of plants in Kg/m^2 this region holds.
	**/
	double getIsPlants() {
		return this->plants;
	}

	/**
	* @function getMaxFrehwater()
	* @brief: getter function; returns the value of max_freshwater in l/m^2 this region can hold.
	* @retval double: value of max_freshwater in Kg/m^2 this region can hold
	**/
	double getMaxFrehwater() {
		return this->max_freshwater;
	}

	/**
	* @function getIsFrehwater()
	* @brief: getter function; returns the current value of watr in l/m^2 this resource holds.
	* @retval double: value of water in l/m^2 this region holds.
	**/
	double getIsFrehwater() {
		return this->freshwater;
	}


	//todo void getSchittoverReferenz(Season<double>& referenz);
	//todo deadAnimals

};
#endif // !_RESOURCES_
