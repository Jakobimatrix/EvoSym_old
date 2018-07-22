#include"DeltaWorld.h"



void DeltaWorld::setNeigboursTemperature(double meanTemp){
	this->mean_neigbour_temperature = meanTemp;
}

void DeltaWorld::changeRegionToFitNeigbours(int regionNeigbour[], const int num_neigbours) {

	this->setRandRegion(this->height, regionNeigbour, num_neigbours, this->region->getRegionId());

}

void DeltaWorld::setRandRegion(double height, int regionNeigbour[], const int num_neigbours, int notThisRegion) {
	//region
	//gehe alle Nachbarn durch, errechne für jeden Nachbarn die Wahrscheinlichkeit für this DeltaWorld Region
	//Durch Höhe und Breitengrad wird die Auswahl begrenzt

	//TODO make a vector
	double PossibleRegion[_AMOUNT_REGIONS];//hier wird gespeichert, welche Regionen möglich sind
	double PossibleRegion_probability[_AMOUNT_REGIONS];//hier wird gespeichert, mit welcher warscheinlichkeit, welche Region diese DeltaWorld wird.
	//std::string report = "";
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		PossibleRegion[ir] = 0.0;
		PossibleRegion_probability[ir] = 0.0;
		if (this->_RG_->getRegion(ir)->occoursInHeight(height)) {//kommt die Region in dieser Höhe vor?
			//Ja
			//report += "Region Nr. " + std::to_string(ir) + " can occur in this height: " + std::to_string(height);
			for (int it = 0; it < _AMOUNT_TEMPERATE_ZONES; it++) {//wie wharscheinlich kommt die Region in der Tempertur Zohne vor?
				if (!IsNaN(this->getTempZoneInfluence(it))) {
					PossibleRegion[ir] += this->getTempZoneInfluence(it) * TrueFalseTo10(this->_RG_->getRegion(ir)->occoursInTempZone(it));				
				}
			}
			if (PossibleRegion[ir] > 0) {
				//report += " and has a chanse of " + std::to_string(PossibleRegion[ir]) + " to occur.\n";
			}
			else {
				//report += " but does not appear in the temperate zones.\n";
			}
		}
		else {
			//report += "Region Nr. " + std::to_string(ir) + " can NOT occur in this height: " + std::to_string(height) + ".\n";
		}
	}
	if (-1 < notThisRegion && notThisRegion < _AMOUNT_REGIONS) {//schließe die Region aus, die Übergeben wurde.
		PossibleRegion[notThisRegion] = 0.0;
		//report += "Region Nr. " + std::to_string(notThisRegion) + " has been ruled out.\n";
	}
	int count = 0;
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		if (PossibleRegion[notThisRegion] != 0.0) {
			count++;
		}
	}
	if (count == 0) {
		//report += "It appears that here no Region is allowed?";
	}
	//Die Region muss sich mit den Regionen der Nachbar DeltaWorlds vertragen

	for (int i = 0; i < num_neigbours; i++) {//8 Nachbarn
		if (!(regionNeigbour[i] < 0)) {
			for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
				PossibleRegion_probability[ir] = PossibleRegion[ir] * (double)(this->_RG_->getRegion(regionNeigbour[i])->getRegionChanseFaktor(ir));
				//P(Region_i) = P(Region_i|Klimazohne) * P(Region_i|NachbarRegion)
				//if (PossibleRegion_probability[ir] == 0) {
				//	report += "Region Nr. " + std::to_string(ir) + " has been ruled out it can not appear next to region "+std::to_string(regionNeigbour[in])+".\n";
				//}
			}			
		}
	}

	double TotalChanse = 0;
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		TotalChanse += PossibleRegion_probability[ir];
	}
	if (TotalChanse == 0) {//falls etwas schief gelaufen ist und für jede Region P(Region_i) = 0 gilt, alle auf possible region zurück
		//std::cout << "Warning: One Region had to be random; jede Region P(Region_i) = 0:" << std::endl;
		//std::cout << report << std::endl;
		if (count != 0) {
			for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
				PossibleRegion_probability[ir] = PossibleRegion[ir];
			}
			for (int i = 0; i < num_neigbours; i++) {//every valide neigbour with region x doubles the chanse that this ´region, will be the neigbours region
				if (!(regionNeigbour[i] < 0)) {
					PossibleRegion_probability[regionNeigbour[i]] += 1;
				}
			}
		}
		else {
			count = 0;
			for (int i = 0; i < num_neigbours; i++) {//every valide neigbour with region x doubles the chanse that this ´region, will be the neigbours region
				if (!(regionNeigbour[i] < 0)) {
					PossibleRegion_probability[regionNeigbour[i]] += 1;
					count++;
				}
			}
			if (count == 0) {
				std::cout << "Warning: One Region had to be default:" << std::endl;
				bool polar = false;
				if (this->temperature_zone_influence[0] > 0.5) {
					polar = true;
				}
				this->InitSetRegionAndHeight(this->_RG_->getDefaultRegion(height,polar),height);
				return;
			}
			else {
				//std::cout << "Warning: One Region had to be random; no valide neigbours:" << std::endl;
			}
		}
	}

	double RandRegion = uniform_double_dist(0.0, TotalChanse);
	TotalChanse = 0;
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		TotalChanse += PossibleRegion_probability[ir];
		if (TotalChanse > RandRegion) {
			this->InitSetRegionAndHeight(this->_RG_->getRegion(ir), height);
			return;
		}
	}
	std::cout << "ERROR: Deltla Worls was not initialized!" << std::endl;
	std::getchar();
	
	return;
}

void DeltaWorld::calcTemp(double dt) {
	
	double tempAlt = this->temperature;
	//dependencys: height, t neigbours, season, region, temperateZone

	double T_TempZone_mean	= 0;//Mittlere Temperatur in der TempZohne  -> Abhängig von der Season
	//this->TempDropDueHeight //Pro 100m Fällt die Durchschnittstemperatur um 1 °C
	//this->meanTempNeigbour Die Temperatur ist von den Temperaturen der Nachbarregionen abhängig
	double T_Region_Offset	= 0; //+- Varianz_tagesrauschen: Jede Region kann Währme besser/schlechter speichern  -> Abhängig von der Season
	double T_TempZone_Offset = 0; //+- Varianz_Jahresrauschen: In jeder TempZohne Treten temperaturschwankungen anders auf  -> Abhängig von der Season
	//this->temperature //ist Temperatur
	
	//Season
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->_G_->SeasonMultiplier[s] > 0) {
			double T_TempZone_mean_2 = 0;
			double T_Region_mean_2 = 0;
			double T_TempZone_offset_Seasondependant = 0;
			//TempZohne
			for (int TempZone = 0; TempZone < _AMOUNT_TEMPERATE_ZONES; TempZone++) {

				if (this->temperature_zone_influence[TempZone] > 0) {
				T_TempZone_mean_2 += this->_RG_->getTemperatureZone(TempZone)->getSeasonDependentTemp()->getValue(s) * this->temperature_zone_influence[TempZone];
									//mean temp.in TempZone in Season		//influence of TempZone 
				T_TempZone_offset_Seasondependant += this->_G_->CurrentYearTempOffset[s] * this->temperature_zone_influence[TempZone];

				T_Region_mean_2 += this->region->getSeasonDependantTempVariation(s, TempZone) * this->temperature_zone_influence[TempZone];
				}
			}

			T_TempZone_mean += T_TempZone_mean_2 * this->_G_->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Temp aller TempZonen
			T_TempZone_Offset += T_TempZone_offset_Seasondependant * this->_G_->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Varianz aller TempZonen

							
			//Region
			T_Region_Offset += T_Region_mean_2 * this->_G_->SeasonMultiplier[s];
		}
	}

	if (!this->initilized) {//dann nur von Temperaturzohne abhängig
		this->temperature = T_TempZone_mean + this->temperature_drop_due_height + T_Region_Offset + T_TempZone_Offset;
																					 
		this->recent_delta_temperature += this->temperature - tempAlt;//for drawing purpose
		return;
	}
	

	//calc new Temp
	this->temperature = (this->temperature - this->temperature_drop_due_height )*_TEMP_INFLUENCE_IS_TEMP + this->mean_neigbour_temperature*_TEMP_INFLUENCE_NEIGHBOURS + T_TempZone_mean*_TEMP_INFLUENCE_TEMPERATE_ZONE + this->ground.layer_temperature[0]* _TEMP_INFLUENCE_GROUND;
	this->temperature += this->temperature_drop_due_height; //beachte Höhe

	this->temperature = this->temperature + T_Region_Offset + T_TempZone_Offset; //beachte Varianzen in TemperaturZohne(Jährlich anders) und Region

	//for drawing purpose
	this->recent_delta_temperature += this->temperature - tempAlt;
}
void DeltaWorld::calcIceThicknes(double dt) {

	double freezingTemp = _WATER_FREEZING_TEMPERATURE[0];
	if (this->region->getRegionId() == 0) {//ocean
		freezingTemp = _WATER_FREEZING_TEMPERATURE[1];
	}
	if (this->ground.layer_temperature[0] < 0) {
		this->is_frozen = true;
		for (unsigned int FrozenLayer = 1; FrozenLayer < this->ground.amount_layers; FrozenLayer++) {
			if (this->ground.layer_temperature[FrozenLayer] > freezingTemp) { //this layer is not frozen anymore
				//linear interpolation between this layer and the last
				double a = (this->ground.layer_temperature[FrozenLayer] - this->ground.layer_temperature[FrozenLayer-1]) / this->region->getGroundProperties()->ground_layer_thickness;
				double b = this->ground.layer_temperature[FrozenLayer] - a*this->region->getGroundProperties()->ground_layer_thickness*(FrozenLayer+1);
				this->ice_thickness = -b / a;
				break;
			}
		}
	}
	else {
		this->is_frozen = false;
		this->ice_thickness = 0.0;
	}
}

void DeltaWorld::calcGroundTemp(double dt, double airTemp)
{
	int last_layer = this->ground.amount_layers - 1;
	double pt_T = PT1_T_Discrete(dt, this->region->getGroundProperties()->ground_pt1_t); //get the discret t constant of a dynamic pt1 behaviour

	for (unsigned int i = 0; i < this->ground.amount_layers; i++) {
		if (i == 0) { //erster Layer
			this->ground.layer_temperature[i] = pt_T * (this->region->getGroundProperties()->ground_above_layer_factor*airTemp + this->region->getGroundProperties()->ground_below_layer_factor*this->ground.layer_temperature[i + 1] - this->ground.layer_temperature[i]) + this->ground.layer_temperature[i];
		}
		else if (i != last_layer) {
			this->ground.layer_temperature[i] = pt_T * (this->region->getGroundProperties()->ground_above_layer_factor*this->ground.layer_temperature[i - 1] + this->region->getGroundProperties()->ground_below_layer_factor*this->ground.layer_temperature[i + 1] - this->ground.layer_temperature[i]) + this->ground.layer_temperature[i];
		}
		else { //letzterLayer
			this->ground.layer_temperature[i] = pt_T * (this->region->getGroundProperties()->ground_above_layer_factor*this->ground.layer_temperature[i - 1] + this->region->getGroundProperties()->ground_below_layer_factor*this->ground.last_layer_temperature - this->ground.layer_temperature[i]) + this->ground.layer_temperature[i];
		}
	}
}

void DeltaWorld::update(double tnow) {
	double dt = (tnow - this->t_last_update);
	this->calcTemp(dt);
	if (this->initilized) {
		this->calcResources(dt);
		this->calcIceThicknes(dt);
		this->calcGroundTemp(dt, this->temperature);
	}	
}
void DeltaWorld::calcResources(double dt) {

	this->resources.RegeneratePlants(dt,this->region->getRegionId(),this->temperature);
	this->resources.RegenerateFreshWater(dt);
}

std::string DeltaWorld::getInfoString() {
	std::string info = this->getUnitName() + "\n\n";
	//geology
	info += "Geology:\n";
	info += "Region: \t" + Region::getRegionName(this->region->getRegionId()) + "\n";
	info += "Height: \t" + std::to_string(this->height) + " m\n";
	info += "Temperature: \t" + std::to_string(this->temperature) + " °C\n";
	info += "Mean neigbour Temperature: \t" + std::to_string(this->mean_neigbour_temperature) + " °C\n";
	for (unsigned int i = 0; i < this->ground.amount_layers; i++) {
		info += "Temperatur Ground Layer " + std::to_string((i+1)*this->region->getGroundProperties()->ground_layer_thickness) + "cm: \t" + std::to_string(this->ground.layer_temperature[i]) + " °C\n";
	}
	info += "Temperatur Ground constLayer: \t" + std::to_string(this->ground.last_layer_temperature) + " °C\n";
	info += "Season: \t" + this->getSeasonText() + "\n";
	TemperateZone T;//todo safe pointer as attribute!!
	for (int i = 0; i < _AMOUNT_TEMPERATE_ZONES; i++) {
		if (this->temperature_zone_influence[i] > 0) {
			info += std::to_string(this->temperature_zone_influence[i]*100) + "% " + _RG_->getTemperatureZone(i)->getZoneName()+"\n";
		}
	}
	info += "Latitude: \t" + std::to_string(this->latitude) + " °\n\n";
	//resources
	info += "Resources:\n";
	info += "Plants: \t" + std::to_string(this->resources.getIsPlants()) + " Kg/m^2\n";
	info += "Plants max: \t" + std::to_string(this->resources.getMaxPlants()) + " Kg/m^2\n";
	
	//info += "Plants Tau: \t" + std::to_string(1.0 / S.spring) + " " + std::to_string(1.0 / S.summer) + " " + std::to_string(1.0 / S.autumn) + " " + std::to_string(1.0 / S.winter) + " 1/s\n";
	info += "Fresh water: \t" + std::to_string(this->resources.getIsFrehwater()) + " l/m^2\n";
	info += "Fresh water max: \t" + std::to_string(this->resources.getMaxFrehwater()) + " l/m^2\n";
	
	//info += "Fresh water Tau: \t" + std::to_string(1.0 / S.spring) + " " + std::to_string(1.0 / S.summer) + " " + std::to_string(1.0 / S.autumn) + " " + std::to_string(1.0 / S.winter) + " 1/s\n";

	//if (this->GroundFrozen()) {
		info += "Ice thickness: \t" + std::to_string(this->ice_thickness) + "m\n";
	//}


	return info;
}

bool DeltaWorld::hadChangeInAppearance(int representation) {
	if (this->getAge_s() < 2 * this->_G_->_DeltaTime) {
		return true;
	}
	else if (this->change_in_appearance) {
		return true;
		this->change_in_appearance = false;
	}
	if (this->wasUpdated()) {
		//1=region/2=height/3=temp/4=season/5=tempZone
		switch (representation)
		{
		case 1:
			return this->hasPassedFreezingPoint();
			break;
		case 3:
			return this->hadRecentChangeInTemp();
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

void DeltaWorld::reset() {
	this->initilized = false;
	this->temperature = 0.0;
	this->t_last_update = -1.0;
	this->t0 = 0.0;
	this->ice_thickness = 0.0;
	this->is_frozen = false;
	this->resources.reset();
}

bool DeltaWorld::hasPassedFreezingPoint() {

	if (this->is_frozen != this->recent_freezing) {
		this->recent_freezing = this->is_frozen;
		return true;
	}
	return false;
}

bool DeltaWorld::hadRecentChangeInTemp() {
	if (std::abs(this->recent_delta_temperature) > _VISUALIZED_TEMPERATURE_DIFFERENCE) {
		this->recent_delta_temperature = 0.0;
		return true;
	}
	return false;
}

bool DeltaWorld::hadChangeInSeason() {
	if (this->_G_->dominantSeason != this->recent_season) {
		this->recent_season = this->_G_->dominantSeason;
		return true;
	}
	return false;
}

std::string DeltaWorld::getSeasonText() {
	switch (this->_G_->dominantSeason) {
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