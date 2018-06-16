#ifndef _RESOURCES_
#define _RESOURCES_
#include <vector>
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

public:
	Resources(){
		this->reset();
	}
	void reset() {
		this->plants = 0.0;
		this->freshwater = 0.0;
	}

	void setMax_freshwater(double max_freshwater) {
		this->max_freshwater = max_freshwater;
	}
	void setRegeneration_freshwater(Season<double>& Regeneration_freshwater) {
		this->Regeneration_freshwater_1_divided_Tau = Regeneration_freshwater;
	}
	void RegenerateFreshWater(double deltaT,double(&TempSeasonIfluence)[_amountSeasons], int regionID) {
		if (this->max_freshwater <= 0.0) {
			this->freshwater = 0.0;
			return;
		}

		//calc Tau from Season
		double Tau = 0;
		for (int s = 0; s < _amountSeasons; s++) {
			if (TempSeasonIfluence[s] > 0.0) {
				Tau +=TempSeasonIfluence[s] * this->Regeneration_freshwater_1_divided_Tau.getValue(s);
			}
		}
		Tau = 1.0 / Tau;
		if (isinf(Tau)) {//Keine Änderung
			return;
		}

		double t = inverse_cosResurceGeneration(this->max_freshwater, Tau, this->freshwater); 

		this->freshwater = cosResurceGeneration(this->max_freshwater, Tau, t + deltaT);
	}

	void setMax_plants(double max_plants) {
		this->max_plants = max_plants;
	}
	void setRegeneration_plants(Season<double>& Regeneration_plants) {
		this->Regeneration_plants_1_divided_Tau = Regeneration_plants;
	}
	void RegeneratePlants(double deltaT,double(&TempSeasonIfluence)[_amountSeasons], int regionID, double temperature) {
		if (this->max_plants <= 0.0) {
			this->plants = 0.0;
			return;
		}

		//calc Tau from Season und temp
		double Tau_temp = 0.0;
		double Tau_region_season = 0.0;
		for (int s = 0; s < _amountSeasons; s++) {
			if (TempSeasonIfluence[s] > 0.0) {
				Tau_region_season = Tau_region_season + (TempSeasonIfluence[s] * this->Regeneration_plants_1_divided_Tau.getValue(s));
				//todo better:? spaart für jede Kachel this->Regeneration_plants_1_divided_Tau
				//Tau = Tau + (TempSeasonIfluence[s] * Region::getRegionBasedPlantGrowth(regionID).getValue(s));
			}
		}
		Tau_temp = one_divid_Tau_depend_Temp(temperature, Region::getOptimalTempForGrowth(regionID));
		
		double Tau = Region::desideTauDependancy(Tau_temp, Tau_region_season, regionID);
		
		//bis hier ist Tau eigendlich 1/Tau
		Tau = 1.0 / Tau;

		if (isinf(Tau)) {//Keine Änderung
			return;
		}	
		double t = inverse_cosResurceGeneration(this->max_plants, Tau, this->plants);

		this->plants = cosResurceGeneration(this->max_plants, Tau, t + deltaT);
	}

	double getMaxPlants() {
		return this->max_plants;
	}
	double getIsPlants() {
		return this->plants;
	}
	double getMaxFrehwater() {
		return this->max_freshwater;
	}
	double getIsFrehwater() {
		return this->freshwater;
	}
	Season<double> getPlantRegenFactor() {
		return this->Regeneration_plants_1_divided_Tau;
	}
	Season<double> getFreshWaterRegenFactor() {
		return this->Regeneration_freshwater_1_divided_Tau;
	}

	//todo void getSchittoverReferenz(Season<double>& referenz);




	//todo deadAnimals

};
#endif // !_RESOURCES_
