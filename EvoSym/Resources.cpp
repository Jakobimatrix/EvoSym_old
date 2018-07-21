#include "Resources.h"

void Resources::RegenerateFreshWater(double deltaT) {
	if (this->max_freshwater <= 0.0) {
		this->freshwater = 0.0;
		return;
	}

	//calc Tau from Season
	double Tau = 0;
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->_G_->SeasonMultiplier[s] > 0.0) {
			Tau += this->_G_->SeasonMultiplier[s] * this->Regeneration_freshwater_1_divided_Tau.getValue(s);
		}
	}
	Tau = 1.0 / Tau;
	if (isinf(Tau)) {//Keine Änderung
		return;
	}

	double t = inverse_cosResurceGeneration(this->max_freshwater, Tau, this->freshwater);

	this->freshwater = cosResurceGeneration(this->max_freshwater, Tau, t + deltaT);
}

void Resources::RegeneratePlants(double deltaT, int regionID, double temperature) {
	if (this->max_plants <= 0.0) {
		this->plants = 0.0;
		return;
	}

	//calc Tau from Season und temp
	double Tau_temp = 0.0;
	double Tau_region_season = 0.0;
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->_G_->SeasonMultiplier[s] > 0.0) {
			Tau_region_season = Tau_region_season + (this->_G_->SeasonMultiplier[s] * this->Regeneration_plants_1_divided_Tau.getValue(s));
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