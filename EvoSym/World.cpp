#include "World.h"

void World::reset() {
	this->time = 0;

	if (!this->init) {
		for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
			this->world_parts.emplace_back(DeltaWorld());
			this->at2xy_lookup_table.emplace_back(xyMap());//init
		}
		for (unsigned int x = 0; x < _WORLD_DIMENSION; x++) {
			for (unsigned int y = 0; y < _WORLD_DIMENSION; y++) {
				this->at2xy_lookup_table[x*_WORLD_DIMENSION + y].x = x;
				this->at2xy_lookup_table[x*_WORLD_DIMENSION + y].y = y;
			}
		}
	}
	else {
		for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
			this->world_parts[i].reset();
		}
	}
	this->init = this->createWorld();
}

bool World::createWorld() {
	//int *ary = new int[sizeX*sizeY];
	//// ary[x][y] is then rewritten as
	//ary[x*sizeY + y]

	int x;//koordinaten
	int y;//koordinaten
	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {//pixel
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {//pixel
			x = xi - _DIMENSION_HALF;	//koordinaten (x,y)
			y = yi - _DIMENSION_HALF;	//koordinaten (x,y)
			int i = (xi)*_WORLD_DIMENSION + yi;	//array element

			//1.Breitengrad;
			//2.TemperateZone;
			//3.Season
			//4.Give every DeltaWorld his Neigbours
			//5.Region and Height
			//6.InitTemp
			//7.Resources

			//1.Breitengrad;
			Point2d DeltaPosition = Point2d(x*_DELTA_SIZE, y*_DELTA_SIZE); //die Mitte der Kachel
			double radiusToCenter = DeltaPosition.getR();
			double angle = DeltaPosition.getArg();
			double RtoEdge;
			if (( Deg2Rad(-45)< angle && angle < Deg2Rad(45) ) || angle > Deg2Rad(135) || angle < Deg2Rad(-135)) {
				RtoEdge = abs(_DIMENSION_HALF*_DELTA_SIZE / cos(angle));
			}
			else {
				RtoEdge = abs(_DIMENSION_HALF*_DELTA_SIZE / sin(angle));
			}  
			double latitude = _G_->_BREITENGRAD.max*radiusToCenter / RtoEdge;


			//2.TemperateZone;
			//done in constructor of delta world using latitude
			
			this->world_parts.at(i).initSetPositionAndLatitude(DeltaPosition, latitude);
		}
	}
	

	////4.Region and Height and initTemp and resources
	if (this->load_from_image) {
		this->createSetHeightPredefined();
	}
	else {
		this->createSetHeightRand();
	}

	/*cleanup der Regionen
	Erosion*/
	volatile int i_er = 0;
	volatile int imax = 7;
	volatile bool erosion;

	std::vector<int> indices;	//array with all indices
	indices.reserve(_AMOUNT_DELTA_WORLDS);
	std::vector<int> bad_region_setting;

	for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
		indices.emplace_back(i);
	}
	do{
		std::random_shuffle(indices.begin(), indices.end());//shuffle the idices for rand access
		erosion = false;
		i_er++;
		for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
			if (!this->_RG_->getRegion(this->world_parts[indices[i]].getRegionId())->hasEnoughNeigboursWithSameRegion(this->getNumNeigboursSameRegion(indices[i], this->world_parts[indices[i]].getRegionId())) ){
				
				int Region_of_neigbours[8];
				this->getNeigbourRegionIdAT(Region_of_neigbours, indices[i]);
				if (!this->world_parts[indices[i]].changeRegionToFitNeigbours(Region_of_neigbours)) {
					bad_region_setting.emplace_back(i);
					getNeigboursAT(bad_region_setting, i);
				}
				erosion = true;
			}
		}
		resetBadDeltaWorldRegions(bad_region_setting);
		std::cout << "erosion Nr. " << i_er << std::endl;		

	} while (erosion && (i_er < imax));

	std::vector<int>().swap(indices);//That will create an empty vector with no memory allocated and swap it with indices, effectively deallocating the memory.

	for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
		if (!this->world_parts[i].isInitialized()) {
			std::cout << "ERROR: Some WorldParts remain uninitialized!" << std::endl;
			std::getchar();
			return false;
		}
	}
	return true;
}

void World::resetBadDeltaWorldRegions(std::vector<int>& bad_regions) {
	while (bad_regions.size() != 0) {
		int Region_of_neigbours[8];
		this->getNeigbourRegionIdAT(Region_of_neigbours, bad_regions.back());
		this->world_parts[bad_regions.back()].changeRegionToFitNeigbours(Region_of_neigbours);
		bad_regions.pop_back();
	}
}

void World::createSetHeightRand() {

	////randomize Perlian Noice
	int x;//koordinaten
	int y;//koordinaten
	int xOffset;
	int yOffset;

	std::vector<double> perlNoiseSmootFaktor;
	perlNoiseSmootFaktor.reserve(_AMOUNT_DELTA_WORLDS);

	SimplexNoise1234 Noise = SimplexNoise1234(true);
	//Verteilung der Krümmung
	//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear. dont go in negativ offset.
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);

	double rand_perlian_distortion_factor = uniform_double_dist(0, _CURVATURE_CHANGE_RATE_PERLIAN_NOISE);
	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;

			perlNoiseSmootFaktor.emplace_back(double(Noise.noise(x*rand_perlian_distortion_factor + xOffset, y*rand_perlian_distortion_factor + yOffset)));//-1...1
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 1) / 2.0;//0...1
			double a = _MAX_CURVATURE_PERLIAN_NOISE - _MIN_CURVATURE_PERLIAN_NOISE;
			perlNoiseSmootFaktor.at(i) = a*perlNoiseSmootFaktor.at(i) + _MIN_CURVATURE_PERLIAN_NOISE;//min...max
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Krümmung ändern darf in i


	//Verzerrung
	//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear. dont go in negativ offset.
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);


	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;

			perlNoiseSmootFaktor.at(i) = Noise.noise(x*perlNoiseSmootFaktor.at(i) + xOffset, y*perlNoiseSmootFaktor.at(i) + yOffset);//return [-1,1]
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 1) / 2.0;//0...1
			double a = _MAX_DISTORTION_PERLIAN_NOISE - _MIN_DISTORTION_PERLIAN_NOISE;
			perlNoiseSmootFaktor.at(i) = a*perlNoiseSmootFaktor.at(i) + _MIN_DISTORTION_PERLIAN_NOISE;//min...max
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Steigung ändern darf in i


	//zoom
	//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear. dont go in negativ offset.
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);

	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;
			perlNoiseSmootFaktor.at(i) = Noise.noise(x*perlNoiseSmootFaktor.at(i) + xOffset, y*perlNoiseSmootFaktor.at(i) + yOffset);//return [-1,1]
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 1) / 2.0;//0...1
			double a = _MAX_ZOOM_PERLIAN_NOISE - _MIN_ZOOM_PERLIAN_NOISE;
			perlNoiseSmootFaktor.at(i) = a*perlNoiseSmootFaktor.at(i) + _MIN_ZOOM_PERLIAN_NOISE;//min...max
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, der zoomfaktor: between _MIN_ZOOM_PERLIAN_NOISE and _MAX_ZOOM_PERLIAN_NOISE

	//height
	//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear. dont go in negativ offset.
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);

	std::vector<int> bad_region_setting;

	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;
			//height

			double height = Noise.noise(x*perlNoiseSmootFaktor.at(i) + xOffset, y*perlNoiseSmootFaktor.at(i) + yOffset);//return [-1,1]

			//|-1        |0       |1 => ratio Land/Ocean		 = 1/1-> dx = 0;
			//dx |-2/5   |0          |8/5 => ratio Land/Ocean	 = 4-> dx = 3/5
			//um wie viel muss verschoben werden (dx), damit gegebener Ratio stimmt?
			//L/O = ratio (1)
			//L + O = 2 = 1-(-1) -> L = 2 - O in (1)
			//(2 -O)/O = ratio -> O = 2/(ratio + 1)
			//dx = 1 - O = 1 - 2/(ratio +1)
			double dx = 1 - 2 / (_LAND_TO_OCEAN_RATIO + 1);
			height = height + dx; //verschiebe "See level"
			if (height < 0) {
				height = height*(1 / (-1 + dx));// [1 0];
				height = height * _MIN_TERRANE_HEIGHT;	// [-min,0]
			}
			else {
				height = height*(1 / (1 + dx));	// [1 0];
				height = height * _MAX_TERRANE_HEIGHT;	// [0,max]
			}
			//region
			//gehe alle Nachbarn durch, errechne für jeden Nachbarn die Wahrscheinlichkeit für this DeltaWorld Region
			//Durch Höhe und Breitengrad wird die Auswahl begrenzt
			int NeigbourRegionId[8];
			this->getNeigbourRegionIdXY(NeigbourRegionId, xi, yi);
			if (this->world_parts.at(i).setRandRegion(height, NeigbourRegionId)) {
				bad_region_setting.emplace_back(i);
				getNeigboursAT(bad_region_setting, i);
			}
		}
	}
	std::vector<double>().swap(perlNoiseSmootFaktor);//That will create an empty vector with no memory allocated and swap it with perlNoiseSmootFaktor, effectively deallocating the memory.

}

void World::createSetHeightPredefined() {
	std::cout << "create world from predefined image" << std::endl;
	sf::Vector2u ImageSize = this->world_height_map.getSize();

	u_int *a = new u_int[ImageSize.x*ImageSize.y];//array aller Pixel RGB
	u_char *b = new u_char[_AMOUNT_DELTA_WORLDS];//array resultierende Pixel GREY

	for (unsigned int x = 0; x < ImageSize.x; x++) {
		for (unsigned int y = 0; y < ImageSize.y; y++) {
			sf::Color pix = this->world_height_map.getPixel(x, y);
			a[x*ImageSize.x + y] = (pix.r << 16) | (pix.g << 8) | (pix.b);
		}
	}
	resample(a, b, ImageSize.x, ImageSize.y, _WORLD_DIMENSION, _WORLD_DIMENSION);

	std::vector<int> bad_region_setting;

	for (unsigned int x = 0; x < _WORLD_DIMENSION; x++) {
		for (unsigned int y = 0; y < _WORLD_DIMENSION; y++) {
			int i = x*_WORLD_DIMENSION + y;
			double height = b[i];//[0...255];
			height = height - 127.5;
			height = height / 127.5;//[-1...1]


			if (height < 0) {
				height = -height;// [1 0];
				height = height * _MIN_TERRANE_HEIGHT;	// [-min,0]
			}
			else {
				height = height * _MAX_TERRANE_HEIGHT;	// [0,max]
			}
			//region
			//gehe alle Nachbarn durch, errechne für jeden Nachbarn die Wahrscheinlichkeit für this DeltaWorld Region
			//Durch Höhe und Breitengrad wird die Auswahl begrenzt

			int Region_of_neigbours[8];
			this->getNeigbourRegionIdXY(Region_of_neigbours, x, y);

			if (!this->world_parts.at(i).setRandRegion(height, Region_of_neigbours)) {
				bad_region_setting.emplace_back(i);
				getNeigboursAT(bad_region_setting, i);
			}
		}
	}
	resetBadDeltaWorldRegions(bad_region_setting);
	delete[] a;
	delete[] b;
}

//TODO go 3D with interpolation between heights http://vis.lbl.gov/NERSC/Software/gsharp/help/Gsharp/userguide/interpolate/index.html

std::string World::GetTimeReadable(bool y,bool d,bool h, bool m, bool s) {
	std::string output = "";
	double t = this->time;
	double tRest = t;
	if (y && tRest > _YEAR_IN_S) {
		t = std::floor(tRest * _INVERSE_YEAR_IN_S);
		if (t > 0.0) {
			output += "Y: " + std::to_string((int)t);
			tRest -= t*_YEAR_IN_S;
		}
	}
	if (d && tRest > _DAY_IN_S) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _INVERSE_DAY_IN_S);
		if (t > 0.0) {
			output += "d: " + std::to_string((int)t);
			tRest -= t*_DAY_IN_S;
		}		
	}
	if (h && tRest > _HOUR_IN_S) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _INVERSE_HOUR_IN_S);
		if (t > 0.0) {
			output += "h: " + std::to_string((int)t);
			tRest -= t*_HOUR_IN_S;
		}				
	}
	if (m && tRest > _MINUTE_IN_S) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _INVERSE_MINUTE_IN_S);
		if (t > 0.0) {
			output += "m: " + std::to_string((int)t);
			tRest -= t*_MINUTE_IN_S;
		}		
	}
	if (s) {
		if (output != "") {
			output += " ";
		}
		if (tRest > 0.0) {
			output += "s: " + std::to_string((int)tRest);
		}		
	}
	return output;
}

void World::Update() {

	//time update
	this->time = this->time + this->_G_->_DeltaTime;
	//shift season //if new year->update for each season::setNewYearTempVariation() 
	this->_G_->_SeasonShift -= this->_G_->_DELTA_SEASON_CHANGE_RAD;

	TemperateZone T;
	if (this->_G_->_SeasonShift < -_2PI) {
		this->_G_->_SeasonShift = this->_G_->_SeasonShift + _2PI;//ein Jahr ist um
		for (int s = 0; s < _AMOUNT_SEASONS; s++) {//new temperature offset for the 4 Seasons
			T = TemperateZone(s);
			this->_G_->NextYearTempOffset[s] = T.getNewYearTempVariation();
		}		
	}
	this->_G_->setSeason();
	double pt_T = PT1_T_Discrete(this->_G_->_DeltaTime, _PT_1_T_SEASONAL_OFFSET); //get the discret t constant of a dynamic pt1 behaviour
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		T = TemperateZone(s);
		//To avoid sudden changes CurrentYearTempOffset is moddeled with a pt1 behaviour
		this->_G_->CurrentYearTempOffset[s] = pt_T*(this->_G_->NextYearTempOffset[s] - this->_G_->CurrentYearTempOffset[s]) + this->_G_->CurrentYearTempOffset[s]; //adding the last difference to prevent huge changes and create trends
	}

	//UPDATE DeltaWorld

	//BOOST_FOREACH
	BOOST_FOREACH(DeltaWorld &WorldPart, this->world_parts) {
		WorldPart.simulate(this->time);
	}	
	//BOOST_FOREACH

	//update temp for every neigbour
	int i;
	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			i = (xi)*_WORLD_DIMENSION + yi;
			this->world_parts[i].setNeigboursTemperature(this->getNeigbourMeanTempXY(this->world_parts[i].getTemp(), xi, yi));
		}
	}
}

bool World::getIsReady() {
	return this->init;
}

bool World::getWorldPartByPosition(Point2d& position, DeltaWorld& dw) {
	int x = (int) (position.x / (double)_TILE_RESULUTION);
	int y = (int) (position.y / (double)_TILE_RESULUTION);

	int dw_element = (x)*_WORLD_DIMENSION + y;
	if (dw_element > -1 && dw_element < _AMOUNT_DELTA_WORLDS) {
		dw = world_parts[dw_element];
		return true;
	}
	return false;
}

void World::getNeigbourRegionIdXY(int(&neigbourRegionId)[_AMOUNT_NEIGHBOURS], int x, int y) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight

			bool left_border_violated = (x - 1 > -1) ? false : true;
			bool right_border_violated = (x + 1 < _WORLD_DIMENSION) ? false : true;

			bool top_border_violated = (y - 1 > -1) ? false : true;
			bool bottom_border_violated = (y + 1 < _WORLD_DIMENSION) ? false : true;

			int in;
			if (!left_border_violated) {//left
				in = (x - 1)*_WORLD_DIMENSION + y;
				neigbourRegionId[4] = this->world_parts.at(in).getRegionId();
			}
			else {
				neigbourRegionId[4] = -1;
			}
			if (!right_border_violated) {//right
				in = (x + 1)*_WORLD_DIMENSION + y;
				neigbourRegionId[0] = this->world_parts.at(in).getRegionId();
			}
			else {
				neigbourRegionId[0] = -1;
			}
			if (!bottom_border_violated) {//bottom
				in = (x)*_WORLD_DIMENSION + y + 1;
				neigbourRegionId[6] = this->world_parts.at(in).getRegionId();

				if (!left_border_violated) {//bottom-left
					in = (x - 1)*_WORLD_DIMENSION + y + 1;
					neigbourRegionId[5] = this->world_parts.at(in).getRegionId();
				}
				else {
					neigbourRegionId[5] = -1;
				}

				if (!right_border_violated) {//bottom-right
					in = (x + 1)*_WORLD_DIMENSION + y + 1;
					neigbourRegionId[7] = this->world_parts.at(in).getRegionId();
				}
				else {
					neigbourRegionId[7] = -1;
				}
			}
			else {//botto outer range
				neigbourRegionId[5] = -1;
				neigbourRegionId[6] = -1;
				neigbourRegionId[7] = -1;
			}
			if (!top_border_violated) {//top
				in = (x)*_WORLD_DIMENSION + y - 1;
				neigbourRegionId[2] = this->world_parts.at(in).getRegionId();

				if (!left_border_violated) {//top-left
					in = (x - 1)*_WORLD_DIMENSION + y - 1;
					neigbourRegionId[3] = this->world_parts.at(in).getRegionId();
				}
				else {
					neigbourRegionId[3] = -1;
				}

				if (!right_border_violated) {//top-right
					in = (x + 1)*_WORLD_DIMENSION + y - 1;
					neigbourRegionId[1] = this->world_parts.at(in).getRegionId();
				}
				else {
					neigbourRegionId[1] = -1;
				}
			}
			else {//top outer range
				neigbourRegionId[1] = -1;
				neigbourRegionId[2] = -1;
				neigbourRegionId[3] = -1;
			}
}
void World::getNeigbourRegionIdAT(int(&neigbourRegionId)[_AMOUNT_NEIGHBOURS], int at) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight

	//at to x y

	int x = this->at2xy_lookup_table.at(at).x;
	int y = this->at2xy_lookup_table.at(at).y;

	bool left_border_violated = (x - 1 > -1) ? false : true;
	bool right_border_violated = (x + 1 < _WORLD_DIMENSION) ? false : true;

	bool top_border_violated = (y - 1 > -1) ? false : true;
	bool bottom_border_violated = (y + 1 < _WORLD_DIMENSION) ? false : true;

	int in;
	if (!left_border_violated) {//left
		in = (x - 1)*_WORLD_DIMENSION + y;
		neigbourRegionId[4] = this->world_parts.at(in).getRegionId();
	}
	else {
		neigbourRegionId[4] = -1;
	}
	if (!right_border_violated) {//right
		in = (x + 1)*_WORLD_DIMENSION + y;
		neigbourRegionId[0] = this->world_parts.at(in).getRegionId();
	}
	else {
		neigbourRegionId[0] = -1;
	}
	if (!bottom_border_violated) {//bottom
		in = (x)*_WORLD_DIMENSION + y + 1;
		neigbourRegionId[6] = this->world_parts.at(in).getRegionId();

		if (!left_border_violated) {//bottom-left
			in = (x - 1)*_WORLD_DIMENSION + y + 1;
			neigbourRegionId[5] = this->world_parts.at(in).getRegionId();
		}
		else {
			neigbourRegionId[5] = -1;
		}

		if (!right_border_violated) {//bottom-right
			in = (x + 1)*_WORLD_DIMENSION + y + 1;
			neigbourRegionId[7] = this->world_parts.at(in).getRegionId();
		}
		else {
			neigbourRegionId[7] = -1;
		}
	}
	else {//botto outer range
		neigbourRegionId[5] = -1;
		neigbourRegionId[6] = -1;
		neigbourRegionId[7] = -1;
	}
	if (!top_border_violated) {//top
		in = (x)*_WORLD_DIMENSION + y - 1;
		neigbourRegionId[2] = this->world_parts.at(in).getRegionId();

		if (!left_border_violated) {//top-left
			in = (x - 1)*_WORLD_DIMENSION + y -1;
			neigbourRegionId[3] = this->world_parts.at(in).getRegionId();
		}
		else {
			neigbourRegionId[3] = -1;
		}

		if (!right_border_violated) {//top-right
			in = (x + 1)*_WORLD_DIMENSION + y - 1;
			neigbourRegionId[1] = this->world_parts.at(in).getRegionId();
		}
		else {
			neigbourRegionId[1] = -1;
		}
	}
	else {//top outer range
		neigbourRegionId[1] = -1;
		neigbourRegionId[2] = -1;
		neigbourRegionId[3] = -1;
	}
}
void World::getNeigboursAT(std::vector<int>& neighbour_pos, int at, const int num_neigbours){
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight

	//at to x y

	int x = this->at2xy_lookup_table.at(at).x;
	int y = this->at2xy_lookup_table.at(at).y;

	bool left_border_violated = (x - 1 > -1) ? false : true;
	bool right_border_violated = (x + 1 < _WORLD_DIMENSION) ? false : true;

	bool top_border_violated = (y - 1 > -1) ? false : true;
	bool bottom_border_violated = (y + 1 < _WORLD_DIMENSION) ? false : true;

	int in;
	if (!left_border_violated) {//left
		in = (x - 1)*_WORLD_DIMENSION + y;
		neighbour_pos.push_back(in);
	}
	if (!right_border_violated) {//right
		in = (x + 1)*_WORLD_DIMENSION + y;
		neighbour_pos.push_back(in);
	}
	if (!bottom_border_violated) {//bottom
		in = (x)*_WORLD_DIMENSION + y + 1;
		neighbour_pos.push_back(in);

		if (!left_border_violated) {//bottom-left
			in = (x - 1)*_WORLD_DIMENSION + y + 1;
			neighbour_pos.push_back(in);
		}

		if (!right_border_violated) {//bottom-right
			in = (x + 1)*_WORLD_DIMENSION + y + 1;
			neighbour_pos.push_back(in);
		}
	}
	if (!top_border_violated) {//top
		in = (x)*_WORLD_DIMENSION + y - 1;
		neighbour_pos.push_back(in);

		if (!left_border_violated) {//top-left
			in = (x - 1)*_WORLD_DIMENSION + y - 1;
			neighbour_pos.push_back(in);
		}

		if (!right_border_violated) {//top-right
			in = (x + 1)*_WORLD_DIMENSION + y - 1;
			neighbour_pos.push_back(in);
		}
	}
}
void World::getNeigbourTempXY(double temp, double(&neigbourTemp)[_AMOUNT_NEIGHBOURS], int x, int y) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight

	bool left_border_violated = (x - 1 > -1) ? false : true;
	bool right_border_violated = (x + 1 < _WORLD_DIMENSION) ? false : true;

	bool top_border_violated = (y - 1 > -1) ? false : true;
	bool bottom_border_violated = (y + 1 < _WORLD_DIMENSION) ? false : true;

	int in;
	if (!left_border_violated) {//left
		in = (x - 1)*_WORLD_DIMENSION + y;
		neigbourTemp[4] = this->world_parts[in].getTemp();
	}
	else {
		neigbourTemp[4] = temp;
	}
	if (!right_border_violated) {//right
		in = (x + 1)*_WORLD_DIMENSION + y;
		neigbourTemp[0] = this->world_parts[in].getTemp();
	}
	else {
		neigbourTemp[0] = temp;
	}
	if (!bottom_border_violated) {//bottom
		in = (x)*_WORLD_DIMENSION + y + 1;
		neigbourTemp[6] = this->world_parts[in].getTemp();

		if (!left_border_violated) {//bottom-left
			in = (x - 1)*_WORLD_DIMENSION + y + 1;
			neigbourTemp[5] = this->world_parts[in].getTemp();
		}
		else {
			neigbourTemp[5] = temp;
		}

		if (!right_border_violated) {//bottom-right
			in = (x + 1)*_WORLD_DIMENSION + y + 1;
			neigbourTemp[7] = this->world_parts[in].getTemp();
		}
		else {
			neigbourTemp[7] = temp;
		}
	}
	else {//botto outer range
		neigbourTemp[5] = temp;
		neigbourTemp[6] = temp;
		neigbourTemp[7] = temp;
	}
	if (!top_border_violated) {//top
		in = (x)*_WORLD_DIMENSION + y - 1;
		neigbourTemp[2] = this->world_parts[in].getTemp();

		if (!left_border_violated) {//top-left
			in = (x - 1)*_WORLD_DIMENSION + y -1;
			neigbourTemp[3] = this->world_parts[in].getTemp();
		}
		else {
			neigbourTemp[3] = temp;
		}

		if (!right_border_violated) {//top-right
			in = (x + 1)*_WORLD_DIMENSION + y - 1;
			neigbourTemp[1] = this->world_parts[in].getTemp();
		}
		else {
			neigbourTemp[1] = temp;
		}
	}
	else {//top outer range
		neigbourTemp[1] = temp;
		neigbourTemp[2] = temp;
		neigbourTemp[3] = temp;
	}
}
double World::getNeigbourMeanTempXY(double temp, int x, int y) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
	double meantTemp = 0;
	int count = 0;
	bool left_border_violated = (x - 1 > -1) ? false : true;
	bool right_border_violated = (x + 1 < _WORLD_DIMENSION) ? false : true;

	bool top_border_violated = (y - 1 > -1) ? false : true;
	bool bottom_border_violated = (y + 1 < _WORLD_DIMENSION) ? false : true;

	int in;
	if (!left_border_violated) {//left
		in = (x - 1)*_WORLD_DIMENSION + y;
		meantTemp += this->world_parts[in].getTemp();
		count++;
	}

	if (!right_border_violated) {//right
		in = (x + 1)*_WORLD_DIMENSION + y;
		meantTemp += this->world_parts[in].getTemp();
		count++;
	}
	if (!bottom_border_violated) {//bottom
		in = (x)*_WORLD_DIMENSION + y + 1;
		meantTemp += this->world_parts[in].getTemp();
		count++;

		if (!left_border_violated) {//bottom-left
			in = (x - 1)*_WORLD_DIMENSION + y + 1;
			meantTemp += this->world_parts[in].getTemp();
			count++;
		}

		if (!right_border_violated) {//bottom-right
			in = (x + 1)*_WORLD_DIMENSION + y + 1;
			meantTemp += this->world_parts[in].getTemp();
			count++;
		}
	}

	if (!top_border_violated) {//top
		in = (x)*_WORLD_DIMENSION + y - 1;
		meantTemp += this->world_parts[in].getTemp();
		count++;

		if (!left_border_violated) {//top-left
			in = (x - 1)*_WORLD_DIMENSION + y - 1;
			meantTemp += this->world_parts[in].getTemp();
			count++;
		}

		if (!right_border_violated) {//top-right
			in = (x + 1)*_WORLD_DIMENSION + y - 1;
			meantTemp += this->world_parts[in].getTemp();
			count++;
		}
	}

	if (count > 0) {
		return meantTemp / count;
	}
	else {
		return temp;
	}
}
int World::getNumNeigboursSameRegion(int at, int regionId) {
	
	int Region_of_neigbours[8];
	this->getNeigbourRegionIdAT(Region_of_neigbours,at);

	int num = 0;
	for (int i = 0; i < 8; i++) {
		if (Region_of_neigbours[i] == regionId) {
			num++;
		}
	}

	return num;
}