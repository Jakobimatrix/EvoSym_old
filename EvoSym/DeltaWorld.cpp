#include"DeltaWorld.h"



//left right top bottom
void DeltaWorld::setNeigbours(DeltaWorld* left, DeltaWorld* right, DeltaWorld* top, DeltaWorld* bottom, DeltaWorld* bottom_right, DeltaWorld* bottom_left, DeltaWorld* top_left, DeltaWorld* top_right){
    this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;

	this->bottom_left = bottom_left;
	this->bottom_right = bottom_right;
	this->top_left = top_left;
	this->top_right = top_right;
}

void DeltaWorld::changeRegionToFitNeigbours() {

	this->setRandRegion(this->height,this->regionID);

}

void DeltaWorld::setRandRegion(double height, int notThisRegion) {
	//region
	//gehe alle Nachbarn durch, errechne für jeden Nachbarn die Wahrscheinlichkeit für this DeltaWorld Region
	//Durch Höhe und Breitengrad wird die Auswahl begrenzt

	//TODO make a vector
	double PossibleRegion[_AMOUNT_REGIONS];//hier wird gespeichert, mit welcher warscheinlichkeit, welche Region diese DeltaWorld wird. 

	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		PossibleRegion[ir] = 0.0;
		if (this->_RG_->getRegion(ir)->occoursInHeight(height)) {//kommt die Region in dieser Höhe vor?
			//Ja
			for (int it = 0; it < _AMOUNT_TEMPERATE_ZONES; it++) {//wie wharscheinlich kommt die Region in der Tempertur Zohne vor?
				if (!IsNaN(this->getTempZoneInfluence(it))) {
					PossibleRegion[ir] += this->getTempZoneInfluence(it) * TrueFalseTo10(this->_RG_->getRegion(ir)->occoursInTempZone(it));
				}
			}
		}
	}
	if (-1 < notThisRegion && notThisRegion < _AMOUNT_REGIONS) {//schließe die Region aus, die Übergeben wurde.
		PossibleRegion[notThisRegion] = 0.0;
	}

	//Die Region muss sich mit den Regionen der Nachbar DeltaWorlds vertragen

	for (int in = 0; in < 8; in++) {//8 Nachbarn
		DeltaWorld* D = this->getNeigbour(in);
		if (D) {
			if (D->isInitialized()) {
				for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
					PossibleRegion[ir] = PossibleRegion[ir] * (double)(this->_RG_->getRegion(D->getRegionId())->getRegionChanseFaktor(ir));
					//P(Region_i) = P(Region_i|Klimazohne) * P(Region_i|NachbarRegion)
				}
			}
		}
	}

	double TotalChanse = 0;
	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		TotalChanse += PossibleRegion[ir];
	}
	if (TotalChanse == 0) {//falls etwas schief gelaufen ist und für jede Region P(Region_i) = 0 gilt
		bool polar = false;
		if (this->InfluencedByTempZone[0] > 0.0) {
			if (this->InfluencedByTempZone[1] > 0.0) {
				polar = randTrueFalse();
			}
			else {
				polar = true;
			}
		}
		this->setRegion_and_height(this->_RG_->getDefaultRegionID(height, polar), height);
		std::cout << "Warning: One Region had to be set up as default:"<<std::endl;
		//TODO more information about this deltaWorld->height,tempZohne,neigbours,etc... all possibilityies
		return;
	}
	else {
		double RandRegion = uniform_double_dist(0.0, TotalChanse);
		TotalChanse = 0;
		for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
			TotalChanse += PossibleRegion[ir];
			if (TotalChanse > RandRegion) {
				this->setRegion_and_height(ir, height);
				return;
			}
		}
		std::cout << "Warning: Deltla Worls was not initialized!" << std::endl;
		std::getchar();
	}
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
	int i = 0;

	///!!!die Forschleife ist langsamer, als das darunter
	//for (int ii = 0; ii < 4; ii++) {
	//	DeltaWorld* neigbour = this->getNeigbour(ii);
	//	if (neigbour != NULL && neigbour->isInitialized()) {
	//		i++;
	//		T_mean_neigbours += neigbour->getTemp();
	//	}
	//}
	if (this->left != NULL && this->left->isInitialized()) {
		i++;
		T_mean_neigbours += this->left->getTemp();
	}
	if (this->right != NULL && this->right->isInitialized()) {
		i++;
		T_mean_neigbours += this->right->getTemp();
	}
	if (this->top != NULL && this->top->isInitialized()) {
		i++;
		T_mean_neigbours += this->top->getTemp();
	}
	if (this->bottom != NULL && this->bottom->isInitialized()) {
		i++;
		T_mean_neigbours += this->bottom->getTemp();
	}
	if (this->bottom_left != NULL && this->bottom_left->isInitialized()) {
		i++;
		T_mean_neigbours += this->bottom_left->getTemp();
	}
	if (this->bottom_right != NULL && this->bottom_right->isInitialized()) {
		i++;
		T_mean_neigbours += this->bottom_right->getTemp();
	}
	if (this->top_left != NULL && this->top_left->isInitialized()) {
		i++;
		T_mean_neigbours += this->top_left->getTemp();
	}
	if (this->top_right != NULL && this->top_right->isInitialized()) {
		i++;
		T_mean_neigbours += this->top_right->getTemp();
	}

	if (i == 0) {//keiner der Nachbarn ist initiert
		T_mean_neigbours = this->temperature;
	}
	else {
		T_mean_neigbours = T_mean_neigbours / (double)i;
	}

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
	if (this->temperature < freezingTemp) {
		this->isFrozen = true;
		for (unsigned int FrozenLayer = 0; FrozenLayer < this->ground.amountLayers; FrozenLayer++) {
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
