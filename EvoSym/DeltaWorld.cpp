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
	double PossibleRegion[_AMOUNT_REGIONS];//hier wird gespeichert, mit welcher warscheinlichkeit, welche Region diese DeltaWorld wird. 

	for (int ir = 0; ir < _AMOUNT_REGIONS; ir++) {
		PossibleRegion[ir] = 0.0;
		if (this->_RG_->getRegion(ir)->occoursInHeight(height)) {//kommt die Region in dieser Höhe vor?
			//Ja
			for (int it = 0; it < 4; it++) {
				PossibleRegion[ir] += this->getTempZoneInfluence(it) * TrueFalseTo10(this->_RG_->getRegion(ir)->occoursInTempZone(it));
			}
		}
		else {
			PossibleRegion[ir] = 0.0;//Zohne kommt nicht in der Höhe vor
		}
	}
	if (-1 < notThisRegion && notThisRegion < _AMOUNT_REGIONS) {
		PossibleRegion[notThisRegion] = 0.0;
	}

	//Die Region muss sich mit den Regionen der Nachbar DeltaWorlds vertragen

	for (int in = 0; in < 7; in++) {
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
	}
}

void DeltaWorld::calcTemp(double dt) {
	
	double tempAlt = this->temperature;
	//dependencys: height, t neigbours, season, region, temperateZone

	double T_TempZone_mean = 0;//Mittlere Temperatur in der TempZohne  -> Abhängig von der Season
	//this->TempDropDueHeight //Pro 100m Fällt die Durchschnittstemperatur um 1 °C
	double T_mean_neigbours = 0;//Die Temperatur ist von den Temperaturen der Nachbarregionen abhängig
	double T_Region_Offset = 0; //+- Varianz_tagesrauschen: Jede Region kann Währme besser/schlechter speichern  -> Abhängig von der Season
	double T_TempZone_Offset = 0; //+- Varianz_Jahresrauschen: In jeder TempZohne Treten temperaturschwankungen anders auf  -> Abhängig von der Season
	//this->temperature //ist Temperatur

	
	//je nachdem wie viel Zeit vergangen ist, müssen die istwerte (t ist und mean nachbar) weniger gewertet werden

	/* Dynamische Abhängigkeit, aber mehr rechenzeit
	//Formel: -e^(-x/T) + 1 ->nach T sekunden ist das Ergebnis 0.6, 3T~=0,95
	//nach ~3Wochen sollte die Temp nicht mehr von der ist Temp abhängen -> T = 1 Wochen = 60*60*24*7 
	//1/1209600 = 
	double timeFactorSoll = -exp(-t_dif * _1_divided_time_dependency_for_temp_seconds_) + 1;
	double timeFactorist = (1 - timeFactorSoll)*0.5;
	//wie stark die verschiedenen Abhängigkeiten gewichtet sind zusammen = 1 !!
	double dependency_neigbour = timeFactorist;
	double dependency_T_TempZone_mean = timeFactorSoll;
	double dependency_T_ist = timeFactorist;
	*/
	//constante Abhängigkeit 
	double dependency_neigbour = _TEMP_INFLUENCE_NEIGHBOURS;		//"nachbar abhängigkeit"
	double dependency_T_TempZone_mean = _TEMP_INFLUENCE_TEMPERATE_ZONE;//"temp Zohne"
	double dependency_T_ist = _TEMP_INFLUENCE_IS_TEMP;			//"temp speicher air"
	double dependency_T_ground = _TEMP_INFLUENCE_GROUND; //"temp speicher ground"
	if (!this->initilized) {
		double dependency_neigbour = 0.0;
		double dependency_T_TempZone_mean = 1.0;
		double dependency_T_ist = 0.0;
		double dependency_T_ground = 0.0;
	}
	//alles zusammen muss 1 ergeben!!

	//Season
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->SeasonMultiplier[s] > 0) {
			TemperateZone T;//todo safe pointer as attribute!!
			double T_TempZone_mean_2 = 0;
			double T_TempZone_offset_Seasondependant = 0;
			//TempZohne
			for (int TempZone = 0; TempZone < _AMOUNT_TEMPERATE_ZONES; TempZone++) {
				T = TemperateZone(TempZone);
				T_TempZone_mean_2 += T.getSeasonDependentTemp()->getValue(s) * this->InfluencedByTempZone[TempZone];
									//mean temp.in TempZone in Season		//influence of TempZone 
				T_TempZone_offset_Seasondependant += this->_G_->CurrentYearTempOffset[s] * this->InfluencedByTempZone[TempZone];
			}

			T_TempZone_mean += T_TempZone_mean_2 * this->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Temp aller TempZonen
			T_TempZone_Offset += T_TempZone_offset_Seasondependant * this->SeasonMultiplier[s]; //einfluss momentane Season[0-1] auf mittlere Varianz aller TempZonen

							
			//Region
			T_Region_Offset += this->_RG_->getRegion(this->regionID)->getSeasonDependantTempVariation(s) * this->SeasonMultiplier[s];
		}
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
	this->temperature = (this->temperature - this->TempDropDueHeight )*dependency_T_ist + T_mean_neigbours*dependency_neigbour + T_TempZone_mean*dependency_T_TempZone_mean + this->ground.layerTemp[0]*dependency_T_ground;
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
	double pt_T = 1.0 / (this->_RG_->getRegion(this->regionID)->getGroundProperties()->groundPt1T / dt + 1); //see wikipedia pt1 for time discrete form

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
	this->calcResources(dt);
	this->calcIceThicknes(dt);
	this->calcGroundTemp(dt, this->temperature);	
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
	for (int i = 0; i < this->ground.amountLayers; i++) {
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
