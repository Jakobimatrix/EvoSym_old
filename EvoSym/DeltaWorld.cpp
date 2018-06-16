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
	double PossibleRegion[_amountRegions];//hier wird gespeichert, mit welcher warscheinlichkeit, welche Region diese DeltaWorld wird. 

	for (int ir = 0; ir < _amountRegions; ir++) {
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
	if (-1 < notThisRegion && notThisRegion < _amountRegions) {
		PossibleRegion[notThisRegion] = 0.0;
	}

	//Die Region muss sich mit den Regionen der Nachbar DeltaWorlds vertragen

	for (int in = 0; in < 7; in++) {
		DeltaWorld* D = this->getNeigbour(in);
		if (D) {
			if (D->isInitialized()) {
				for (int ir = 0; ir < _amountRegions; ir++) {
					PossibleRegion[ir] = PossibleRegion[ir] * (double)(this->_RG_->getRegion(D->getRegionId())->getRegionChanseFaktor(ir));
					//P(Region_i) = P(Region_i|Klimazohne) * P(Region_i|NachbarRegion)
				}
			}
		}
	}

	double TotalChanse = 0;
	for (int ir = 0; ir < _amountRegions; ir++) {
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
		for (int ir = 0; ir < _amountRegions; ir++) {
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
	double dependency_neigbour = _multiplier_neigbours;		//"nachbar abhängigkeit"
	double dependency_T_TempZone_mean = _multiplier_temperateZone;//"temp Zohne"
	double dependency_T_ist = _ist_temp;			//"temp speicher"
	if (!this->initilized) {
		double dependency_neigbour = 0.0;
		double dependency_T_TempZone_mean = 1;
		double dependency_T_ist = 0.0;
	}
	//alles zusammen muss 1 ergeben!!

	//Season
	for (int s = 0; s < _amountSeasons; s++) {
		if (this->SeasonMultiplier[s] > 0) {
			TemperateZone T;
			double T_TempZone_mean_2 = 0;
			double T_TempZone_offset_Seasondependant = 0;
			//TempZohne
			for (int TempZone = 0; TempZone < _amountTemperateZones; TempZone++) {
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
	this->temperature = this->temperature*dependency_T_ist + T_mean_neigbours*dependency_neigbour + T_TempZone_mean*dependency_T_TempZone_mean;	
	this->temperature += this->TempDropDueHeight; //beachte Höhe

	this->temperature = this->temperature + T_Region_Offset + T_TempZone_Offset; //beachte Varianzen in TemperaturZohne und Region

	//for drawing purpose
	this->RecentDeltaTemperature += this->temperature - tempAlt;
}
void DeltaWorld::calcIceThicknes(double dt) {

	Region *R = this->_RG_->getRegion(this->regionID);

	//tau nehme linear zu mit dem Breitengrad: große Tau->langsameres EisWachstum
	//zusätlich schmiltzt dickeres Eis langsamer als dünnens Eis.
	//double Tau = R->getTauIceGrwoth() + this->latitude*_ice_tau_per_degree_latitude + this->iceThickness*_ice_tau_thickness_factor;
	//todo relativeIceThickness more global
	double Tau;
	if (this->temperature > 0) {//Tau ist negativ und repräsentiert den schmeltzprozess
		double relativeIceThickness = this->iceThickness + 0.8;
		relativeIceThickness = std::pow(relativeIceThickness, 5.0);//je dicker das eis, desto langsamer Taut es

		Tau = R->getTauIceGrwoth();//es Taut je nach Region schneller oder langsamer;
		//Tau = relativeIceThickness*_ice_tau_thickness_factor;
		Tau = Tau / (_ice_tau_per_grad_celsius_factor*std::pow(this->temperature*_ice_temperature_gradient, _ice_melt_temp_expo));//Tau soll kleiner werden je größer die Temperatur ist->schneller auftauen
		//Tau = Tau / (this->latitude * _ice_tau_per_degree_latitude + 1); //Je steiler die Sonne steht, desto schneller Taut es
		Tau = -Tau;
	}
	else {//Tau ist positiv und repräsentiert den zuwachs an von Eis
		int waterType = 0;
		if (this->regionID == 0) {
			waterType = 1;
		}
		if (temperature < _water_freez_temp[waterType]) {

			Tau = R->getTauIceGrwoth();//es friert je nach Region schneller oder langsamer zu;
			Tau = Tau / std::abs(_ice_tau_per_grad_celsius_factor*std::pow(this->temperature*_ice_temperature_gradient, _ice_freeze_temp_expo));  //je niedriger die Temp, desto schneller friert es
		}
		else {
			return;
		}
	}

	double max_ice_thickness = R->getMaxIceThickness();
	double t = inverse_cosResurceGeneration(max_ice_thickness, Tau, this->iceThickness);

	this->iceThickness = cosResurceGeneration(max_ice_thickness, Tau, t + dt);

	if (iceThickness > 0) {
		this->isFrozen = true;
	}
	else {
		this->isFrozen = false;
	}
}

void DeltaWorld::update(double tnow) {
	double dt = (tnow - this->t_lastUpdate);
	this->calcTemp(dt);
	this->calcResources(dt);
	this->calcIceThicknes(dt);
	
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
	info += "Season: \t" + this->getSeasonText() + "\n";
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
