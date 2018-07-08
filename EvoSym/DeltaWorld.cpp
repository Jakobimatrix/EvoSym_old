#include"DeltaWorld.h"



void DeltaWorld::setNeigboursTemperature(double temps[], const int num_neigbours){
	for (int i = 0; i < num_neigbours; i++) {
		this->TempNeigbours[i] = temps[i];
	}
}

void DeltaWorld::changeRegionToFitNeigbours(int regionNeigbour[], const int num_neigbours) {

	this->setRandRegion(this->height, regionNeigbour, num_neigbours, this->regionID);

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
		std::cout << "Warning: One Region had to be random:" << std::endl;
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
				if (this->InfluencedByTempZone[0] > 0.5) {
					polar = true;
				}
				this->setRegion_and_height(this->_RG_->getDefaultRegionID(height,polar),height);
				return;
			}
			else {
				std::cout << "Warning: One Region had to be random:" << std::endl;
			}
		}
	}

	double RandRegion = uniform_double_dist(0.0, TotalChanse);
	TotalChanse = 0;
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		TotalChanse += PossibleRegion_probability[ir];
		if (TotalChanse > RandRegion) {
			this->setRegion_and_height(ir, height);
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
	double T_mean_neigbours = 0;//Die Temperatur ist von den Temperaturen der Nachbarregionen abhängig
	double T_Region_Offset	= 0; //+- Varianz_tagesrauschen: Jede Region kann Währme besser/schlechter speichern  -> Abhängig von der Season
	double T_TempZone_Offset = 0; //+- Varianz_Jahresrauschen: In jeder TempZohne Treten temperaturschwankungen anders auf  -> Abhängig von der Season
	//this->temperature //ist Temperatur

	//Season
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->SeasonMultiplier[s] > 0) {
			TemperateZone T;//todo safe pointer as attribute!!
			double T_TempZone_mean_2 = 0;
			double T_TempZone_offset_Seasondependant = 0;
			//TempZohne
			for (int TempZone = 0; TempZone < _AMOUNT_TEMPERATE_ZONES; TempZone++) {
				T = TemperateZone(TempZone);
				if (this->InfluencedByTempZone[TempZone] > 0) {
				T_TempZone_mean_2 += T.getSeasonDependentTemp()->getValue(s) * this->InfluencedByTempZone[TempZone];
									//mean temp.in TempZone in Season		//influence of TempZone 
				T_TempZone_offset_Seasondependant += this->_G_->CurrentYearTempOffset[s] * this->InfluencedByTempZone[TempZone];
				}
			}

			T_TempZone_mean += T_TempZone_mean_2 * this->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Temp aller TempZonen
			T_TempZone_Offset += T_TempZone_offset_Seasondependant * this->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Varianz aller TempZonen

							
			//Region
			T_Region_Offset += this->_RG_->getRegion(this->regionID)->getSeasonDependantTempVariation(s) * this->SeasonMultiplier[s];
		}
	}

	if (!this->initilized) {//dann nur von Temperaturzohne abhängig
		this->temperature = T_TempZone_mean + this->TempDropDueHeight + T_Region_Offset + T_TempZone_Offset;
																					 
		this->RecentDeltaTemperature += this->temperature - tempAlt;//for drawing purpose
		return;
	}


	//Berechne MeanTemp nachbar
	for (int i = 0; i < 8; i++) {
		T_mean_neigbours += this->TempNeigbours[i];
	}
	T_mean_neigbours = T_mean_neigbours / 8;
	

	//calc new Temp
	this->temperature = (this->temperature - this->TempDropDueHeight )*_TEMP_INFLUENCE_IS_TEMP + T_mean_neigbours*_TEMP_INFLUENCE_NEIGHBOURS + T_TempZone_mean*_TEMP_INFLUENCE_TEMPERATE_ZONE + this->ground.layerTemp[0]* _TEMP_INFLUENCE_GROUND;
	this->temperature += this->TempDropDueHeight; //beachte Höhe

	this->temperature = this->temperature + T_Region_Offset + T_TempZone_Offset; //beachte Varianzen in TemperaturZohne und Region

	//for drawing purpose
	this->RecentDeltaTemperature += this->temperature - tempAlt;
}
void DeltaWorld::calcIceThicknes(double dt) {

	Region *R = this->_RG_->getRegion(this->regionID);
	double freezingTemp = _WATER_FREEZING_TEMPERATURE[0];
	if (this->regionID == 0) {//ocean
		freezingTemp = _WATER_FREEZING_TEMPERATURE[1];
	}
	if (this->ground.layerTemp[0] < 0) {
		this->isFrozen = true;
		for (unsigned int FrozenLayer = 1; FrozenLayer < this->ground.amountLayers; FrozenLayer++) {
			if (this->ground.layerTemp[FrozenLayer] > freezingTemp) { //this layer is not frozen anymore
				//linear interpolation between this layer and the last
				double a = (this->ground.layerTemp[FrozenLayer] - this->ground.layerTemp[FrozenLayer-1]) / R->getGroundProperties()->groundLayerThickness;
				double b = this->ground.layerTemp[FrozenLayer] - a*R->getGroundProperties()->groundLayerThickness*(FrozenLayer +1);
				this->iceThickness = -b / a;
				break;
			}
		}
	}
	else {
		this->isFrozen = false;
		this->iceThickness = 0.0;
	}
}

void DeltaWorld::calcGroundTemp(double dt, double airTemp)
{
	int last_layer = this->ground.amountLayers - 1;
	double pt_T = PT1_T_Discrete(dt, this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundPt1T); //get the discret t constant of a dynamic pt1 behaviour

	for (unsigned int i = 0; i < this->ground.amountLayers; i++) {
		if (i == 0) { //erster Layer
			this->ground.layerTemp[i] = pt_T * (this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundaboveLayerFacator*airTemp + this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundbelowLayerFacator*this->ground.layerTemp[i + 1] - this->ground.layerTemp[i]) + this->ground.layerTemp[i];
		}
		else if (i != last_layer) {
			this->ground.layerTemp[i] = pt_T * (this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundaboveLayerFacator*this->ground.layerTemp[i - 1] + this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundbelowLayerFacator*this->ground.layerTemp[i + 1] - this->ground.layerTemp[i]) + this->ground.layerTemp[i];
		}
		else { //letzterLayer
			this->ground.layerTemp[i] = pt_T * (this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundaboveLayerFacator*this->ground.layerTemp[i - 1] + this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundbelowLayerFacator*this->ground.lastLayerTemp - this->ground.layerTemp[i]) + this->ground.layerTemp[i];
		}
	}
}

void DeltaWorld::update(double tnow) {
	double dt = (tnow - this->t_lastUpdate);
	this->calcTemp(dt);
	if (this->initilized) {
		this->calcResources(dt);
		this->calcIceThicknes(dt);
		this->calcGroundTemp(dt, this->temperature);
	}
	
}
void DeltaWorld::calcResources(double dt) {

	this->resources.RegeneratePlants(dt,this->SeasonMultiplier,this->regionID,this->temperature);
	this->resources.RegenerateFreshWater(dt, this->SeasonMultiplier, this->regionID);
}

std::string DeltaWorld::getInfoString() {
	std::string info = this->getUnitName() + "\n\n";
	//geology
	info += "Geology:\n";
	info += "Region: \t" + Region::getRegionName(this->regionID) + "\n";
	info += "Height: \t" + std::to_string(this->height) + " m\n";
	info += "Temperatur: \t" + std::to_string(this->temperature) + " °C\n";
	for (unsigned int i = 0; i < this->ground.amountLayers; i++) {
		info += "Temperatur Ground Layer " + std::to_string((i+1)*this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundLayerThickness) + "cm: \t" + std::to_string(this->ground.layerTemp[i]) + " °C\n";
	}
	info += "Temperatur Ground constLayer: \t" + std::to_string(this->ground.lastLayerTemp) + " °C\n";
	info += "Season: \t" + this->getSeasonText() + "\n";
	TemperateZone T;//todo safe pointer as attribute!!
	for (int i = 0; i < _AMOUNT_SEASONS; i++) {
		if (this->InfluencedByTempZone[i] > 0) {
			T = TemperateZone(i);
			info += std::to_string(this->InfluencedByTempZone[i]*100) + "% " + T.getZoneName()+"\n";
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
		info += "Ice thickness: \t" + std::to_string(this->iceThickness) + "m\n";
	//}


	return info;
}
