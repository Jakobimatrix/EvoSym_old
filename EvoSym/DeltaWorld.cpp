#include"DeltaWorld.h"

void DeltaWorld::setNeigboursTemperature(double meanTemp){
	this->mean_neigbour_temperature = meanTemp;
}

bool DeltaWorld::changeRegionToFitNeigbours(int(&regionNeigbour)[_AMOUNT_NEIGHBOURS]) {

	return this->setRandRegion(this->height, regionNeigbour, this->region->getRegionId());

}

bool DeltaWorld::setRandRegion(double height, int(&regionNeigbour)[_AMOUNT_NEIGHBOURS], int notThisRegion) {

	double region_probability[_AMOUNT_REGIONS]; //saves the probabilitiy of the region i
	double total_probability = 0;
	//1. get the probability for the region from the temperated zone and the height
	for (int i = 0; i < _AMOUNT_REGIONS; i++) {
		region_probability[i] = 0;
		if (this->_RG_->getRegion(i)->occoursInHeight(height)) {
			for (int tz = 0; tz < _AMOUNT_TEMPERATE_ZONES; tz++) {
				region_probability[i] += temperature_zone_influence[tz] * this->_RG_->getRegion(i)->occoursInTempZone(tz);
				total_probability += region_probability[i];
			}
		}
	}
	//error handling
	if (total_probability == 0) {
		std::string info = "Warning: Bad region parameters! \n";
		info += "Coordinates: " + getUnitName() + "\n"
			+ "Height: \t" + std::to_string(this->height) + " m\n";
		for (int i = 0; i < _AMOUNT_TEMPERATE_ZONES; i++) {
			if (this->temperature_zone_influence[i] > 0) {
				info += std::to_string(this->temperature_zone_influence[i] * 100) + "% " + _RG_->getTemperatureZone(i)->getZoneName() + "\n";
			}
		}
		std::cout << info;
		bool polar = false;
		if (this->temperature_zone_influence[0] > 0.5) {
			polar = true;
		}
		this->initSetRegionAndHeight(this->_RG_->getDefaultRegion(height, polar), height);
		return false;
	}
	total_probability = 0;

	//2. consider neigbours
	std::string info2 = "";
	for (int n = 0; n < _AMOUNT_NEIGHBOURS; n++) {
		if (!(regionNeigbour[n] < 0)) { // the neigbour has a region 
			for (int r = 0; r < _AMOUNT_REGIONS; r++) {
				region_probability[r] = region_probability[r] *
					this->_RG_->getRegion(regionNeigbour[n])->getRegionChanseFaktor(r);
				if ((this->_RG_->getRegion(regionNeigbour[n])->getRegionChanseFaktor(r)) == 0.0) {
					info2 += "Region " + std::to_string(regionNeigbour[n]) + " mag Region " + std::to_string(r) + " nicht\n";
				}
			}
		}
	}

	//calc the summof all probabilities
	total_probability = 0;
	for (int r = 0; r < _AMOUNT_REGIONS; r++) {
		total_probability += region_probability[r];
	}

	//check if the probability for every region is zero
	if (total_probability == 0) {
		//std::cout << "Warning: Region set to random after considering neigbours.\n" + info2;
		bool polar = false;
		if (this->temperature_zone_influence[0] > 0.5) {
			polar = true;
		}
		this->initSetRegionAndHeight(this->_RG_->getDefaultRegion(height, polar), height);
		return false;
	}

	// rule out region which was given if its possible
	if (-1 < notThisRegion && notThisRegion < _AMOUNT_REGIONS) {
		if (total_probability - region_probability[notThisRegion] > 0) {//there are other possibilities apart from that regon we dont want
			total_probability = total_probability - region_probability[notThisRegion];
			region_probability[notThisRegion] = 0.0;		
		}
		else {
			//std::cout << "Warning: We cannot rule out the region " << notThisRegion << ". It is the only possible.\n";
		}
	}

	//3. set a rand region according to the region_probability[r]
	double rand_region = uniform_double_dist(0.0, total_probability);
	total_probability = 0;
	for (int r = 0; r < _AMOUNT_REGIONS; r++) {
		total_probability += region_probability[r];
		if (total_probability > rand_region) {
			this->initSetRegionAndHeight(this->_RG_->getRegion(r), height);
			return true;
		}
	}

	std::cout << "Warning: Region set to random after probability failed:\n"
		<< "total_probability: " + std::to_string(total_probability) + "\n"
		+ "choosen probability: " + std::to_string(rand_region) + "\n";
	bool polar = false;
	if (this->temperature_zone_influence[0] > 0.5) {
		polar = true;
	}
	this->initSetRegionAndHeight(this->_RG_->getDefaultRegion(height, polar), height);
	return false;
}
double DeltaWorld::calcSetPointTemperature() {

	double set_point_temperature_season = 0;
	double temperature_year_offset = 0;
	double temperature_region_offset = 0;
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		if (this->_G_->SeasonMultiplier[s] > 0) {//only the seasons which influences our temperature.
			for (int tz = 0; tz < _AMOUNT_TEMPERATE_ZONES; tz++) {
				if (this->temperature_zone_influence[tz] > 0) {//only the temp_zones wich effect this delta world.
					set_point_temperature_season +=
						this->_RG_->getTemperatureZone(tz)->getSeasonDependentTemp()->getValue(s) //[celsius] set_point for the season s in the temperate zone tz
						* this->temperature_zone_influence[tz] //[0-1] influence of the temperature zone
						* this->_G_->SeasonMultiplier[s]; //[0-1] influence of the season

					temperature_region_offset +=
						this->region->getSeasonDependantTempVariation(s,tz) //[celsius] different offset for each season in different temperate zones
						* this->temperature_zone_influence[tz] //[0-1] influence of the temperature zone
						* this->_G_->SeasonMultiplier[s]; //[0-1] influence of the season
				}
			}
			temperature_year_offset += this->_G_->CurrentYearTempOffset[s] //different offset for each season in each year
				* this->_G_->SeasonMultiplier[s]; //[0-1] influence of the season			
		}
	}

	return set_point_temperature_season + temperature_year_offset + temperature_region_offset + temperature_drop_due_height;
}

void DeltaWorld::calcTemp(double dt) {
	
	double last_temperature = this->temperature;
	
	this->temperature = last_temperature * _TEMP_INFLUENCE_IS_TEMP
		+ this->mean_neigbour_temperature * _TEMP_INFLUENCE_NEIGHBOURS
		+ this->ground.layer_temperature[0] * _TEMP_INFLUENCE_GROUND
		+ this->calcSetPointTemperature() * _TEMP_INFLUENCE_TEMPERATE_ZONE;

	//for drawing purpose
	this->recent_delta_temperature += this->temperature - last_temperature;	
	
}
void DeltaWorld::calcIceThicknes(double dt) {

	double freezing_temperature = _WATER_FREEZING_TEMPERATURE[0];
	if (this->region->getRegionId() == 0) {//ocean
		freezing_temperature = _WATER_FREEZING_TEMPERATURE[1];
	}
	if (this->ground.layer_temperature[0] < freezing_temperature) {
		this->is_frozen = true;

		for (unsigned int FrozenLayer = 1; FrozenLayer < this->ground.amount_layers; FrozenLayer++) {
			if (this->ground.layer_temperature[FrozenLayer] > freezing_temperature) { //this layer is not frozen anymore
				//linear interpolation between this layer and the last
				//temp_freez = a*ice_thickness + b;			seaching ice_thickness
				//t_neg = a*((n-1)*layer_thickness) + b		first equation
				//t_pos = a*(n*layer_thicknes) + b			second equation
				//a = t_pos-t_neg/layer_thickness			gradient
				//b = t_pos - a*layer_thickness*n			offset
				//ice_thickness = (temp_freez - b)/a		

				double a = (this->ground.layer_temperature[FrozenLayer] - this->ground.layer_temperature[FrozenLayer-1]) / this->region->getGroundProperties()->ground_layer_thickness;
				double b = this->ground.layer_temperature[FrozenLayer] - a*this->region->getGroundProperties()->ground_layer_thickness*(FrozenLayer);
				this->ice_thickness = (freezing_temperature - b) / a;
				return;
			}
		}
		this->ice_thickness = this->ground.amount_layers * this->region->getGroundProperties()->ground_layer_thickness;
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
	initilized = false;
	temperature = 0.0;
	t_last_update = -1.0;
	t0 = 0.0;
	ice_thickness = 0.0;
	is_frozen = false;
	resources.reset();
	ground.init(this->region->getGroundProperties(), _G_, latitude);
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

bool DeltaWorld::getBorderEntryPoint(Point2d& entry_point, Point2d& begin_travel_point, Point2d& end_travel_point) {
	/*______
	|		|
	|	.	|
	|_______| .begin
	*/

	//which border is next to the begin point?
	bool above = false;
	bool right = false;
	if (begin_travel_point.x > position.x) {
		right = true;
	}
	if (begin_travel_point.y > position.y) {
		above = true;
	}

	if (above) {
		Point2d edge_upper_left  = Point2d(position.x - _DELTA_SIZE_HALF,position.y - _DELTA_SIZE_HALF);
		Point2d edge_upper_right = Point2d(position.x + _DELTA_SIZE_HALF, position.y - _DELTA_SIZE_HALF);;
		Point2d intersection = getSchnittpunkt(begin_travel_point, end_travel_point, edge_upper_left, edge_upper_right);
		if (isPointCinLineAB(begin_travel_point, end_travel_point, intersection, 0.1) && isPointCinLineAB(edge_upper_left, edge_upper_right, intersection, 1.0)) {
			entry_point = intersection;
			return true;
		}
	}
	else {
		Point2d edge_lower_left  = Point2d(position.x - _DELTA_SIZE_HALF, position.y + _DELTA_SIZE_HALF);;
		Point2d edge_lower_right = Point2d(position.x + _DELTA_SIZE_HALF, position.y + _DELTA_SIZE_HALF);;
		Point2d intersection = getSchnittpunkt(begin_travel_point, end_travel_point, edge_lower_left, edge_lower_right);
		if (isPointCinLineAB(begin_travel_point, end_travel_point, intersection, 0.1) && isPointCinLineAB(edge_lower_left, edge_lower_right, intersection, 1.0)) {
			entry_point = intersection;
			return true;
		}
	}
	if (right) {
		Point2d edge_lower_right = Point2d(position.x + _DELTA_SIZE_HALF, position.y + _DELTA_SIZE_HALF);;
		Point2d edge_upper_right = Point2d(position.x + _DELTA_SIZE_HALF, position.y - _DELTA_SIZE_HALF);;
		Point2d intersection = getSchnittpunkt(begin_travel_point, end_travel_point, edge_lower_right, edge_upper_right);
		if (isPointCinLineAB(begin_travel_point, end_travel_point, intersection, 0.1) && isPointCinLineAB(edge_lower_right, edge_upper_right, intersection, 1.0)) {
			entry_point = intersection;
			return true;
		}
	}
	else {
		Point2d edge_lower_left = Point2d(position.x - _DELTA_SIZE_HALF, position.y + _DELTA_SIZE_HALF);;
		Point2d edge_upper_left = Point2d(position.x - _DELTA_SIZE_HALF, position.y - _DELTA_SIZE_HALF);;
		Point2d intersection = getSchnittpunkt(begin_travel_point, end_travel_point, edge_upper_left, edge_lower_left);
		if (isPointCinLineAB(begin_travel_point, end_travel_point, intersection, 0.1) && isPointCinLineAB(edge_upper_left, edge_lower_left, intersection, 1.0)) {
			entry_point = intersection;
			return true;
		}
	}
	return false;	
}

double DeltaWorld::TravelDeltaWorld(Point2d& travelvector, double dt, double size, double density, double feather) {
	double weight = size*density;
	double velocity = travelvector.getR() / dt;
	if (_MIN_FLYING_WEIGHT_PER_SIZE_RATIO * feather > density && feather > _MIN_FLYING_FEATHERS) { //animal flyes
		double distance = travelvector.getR()*gradient.getR();
		double angle = std::abs(gradient.getArg() - travelvector.getArg());
		double height_difference = distance * cos(angle); //can be negative->gets energy back

		return weight* (0.5 *  velocity*velocity + _EARTH_ACCELERATION * height_difference);
		//1/2 m_animal * v_travel^2  + m*g*h
	}

	if (region->getRegionId() == 0 || region->getRegionId() == 1) {//lake or ocean
		if (!is_frozen) {
			double swimming_weight = weight - _WATER_WEIGHT_PER_CUBIC_METER*size;//if positive the animal does not float
			return 0.5 * weight* velocity*velocity*_WATER_FRICTION_MULTIPLICATOR + largerValue(0.0, swimming_weight) * _EARTH_ACCELERATION * dt ;
			//1/2 m_animal * v_travel^2 * water resistance +   m_not_swimming * 9.81 * t
		}
	}

	double height_difference;
	double distance = travelvector.getR()*gradient.getR();
	double angle = std::abs(gradient.getArg() - travelvector.getArg());
	if (angle > _PI_HALF) {//downwards
		height_difference = -_WORK_REDUCTION_DOWNWARDS * distance * cos(angle);
	}
	else {//upwards
		height_difference = distance * cos(angle);
	}
	return weight* (0.5 *  velocity*velocity +  _EARTH_ACCELERATION * height_difference);
	//1/2 m_animal * v_travel^2  + m*g*h
}