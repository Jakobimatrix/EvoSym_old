/**
* @file
* @brief
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#include "World.h"
void World::setTemperateZone() {
	this->Polar			= TemperateZone(0);
	this->Moderate		= TemperateZone(1);
	this->Subtropical	= TemperateZone(2);
	this->Tropical		= TemperateZone(3);
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
			
			this->WorldParts.at(i).setPositionAndLatitude(DeltaPosition, latitude);
			this->WorldParts.at(i).setSeason(angle);//season
		}
	}
	/*
	//4.Give every DeltaWorld his Neigbours
	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {//pixel
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {//pixel

			int i = (xi)*_WORLD_DIMENSION + yi;	//array element

			DeltaWorld* left	= nullptr;
			DeltaWorld* right	= nullptr;
			DeltaWorld* top		= nullptr;
			DeltaWorld* bottom	= nullptr;

			DeltaWorld* top_left		= nullptr;
			DeltaWorld* top_right		= nullptr;
			DeltaWorld* bottom_left		= nullptr;
			DeltaWorld* bottom_right	= nullptr;

			bool left_border_violated	= (xi - 1 > -1) ? false : true;
			bool right_border_violated	= (xi + 1 < _WORLD_DIMENSION) ? false : true;

			bool top_border_violated	= (yi - 1 > -1) ? false : true;
			bool bottom_border_violated = (yi + 1 < _WORLD_DIMENSION) ? false : true;
			
			int in;
			if (!left_border_violated) {//left
				in = (xi - 1)*_WORLD_DIMENSION + yi;
				left = this->WorldParts[in];
			}
			if (!right_border_violated) {//right
				in = (xi + 1)*_WORLD_DIMENSION + yi;
				right = this->WorldParts[in];
			}
			if (!bottom_border_violated) {//bottom
				in = (xi)*_WORLD_DIMENSION + yi + 1;
				bottom = this->WorldParts[in];

				if (!left_border_violated) {//bottom-left
					in = (xi - 1)*_WORLD_DIMENSION + yi + 1;
					bottom_left = this->WorldParts[in];
				}

				if (!right_border_violated) {//bottom-right
					in = (xi + 1)*_WORLD_DIMENSION + yi + 1;
					bottom_right = this->WorldParts[in];
				}
			}
			if (!top_border_violated) {//top
				in = (xi)*_WORLD_DIMENSION + yi - 1;
				top = this->WorldParts[in];

				if (!left_border_violated) {//top-left
					in = (xi - 1)*_WORLD_DIMENSION + yi - 1;
					top_left = this->WorldParts[in];
				}

				if (!right_border_violated) {//top-right
					in = (xi + 1)*_WORLD_DIMENSION + yi - 1;
					top_right = this->WorldParts[in];
				}
			}					
			this->WorldParts[i]->setNeigbours(left, right, top, bottom, bottom_right, bottom_left,top_left,top_right);
		}
	}
	*/

	////5.Region and Height and initTemp and resources
	if (this->loadFromImage) {
		this->createSetHeightPredefined();
	}
	else {
		this->createSetHeightRand();
	}


	/*cleanup der Regionen
	Erosion*/
	int i_er = 0;
	int imax = 10;
	bool erosion;

	//array with all indices

	std::vector<int> indices;
	indices.reserve(_AMOUNT_DELTA_WORLDS);
	for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
		indices.push_back(i);
	}
	do{
		std::random_shuffle(indices.begin(), indices.end());//shuffle the idices for rand access
		erosion = false;
		i_er++;
		for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
			if (! this->_RG_->getRegion(this->WorldParts[indices.at(i)].getRegionId())->hasEnoughNeigboursWithSameRegion(this->WorldParts[indices.at(i)].getNumNeigboursWithSameRegion())) {
				
				int Region_of_neigbours[8];
				for (int i = 0; i < 8; i++) {
					//TODO Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
				}
				this->WorldParts.at(indices.at(i)).changeRegionToFitNeigbours(Region_of_neigbours);
				bool erosion = true;
			}
		}
	} while (erosion && i_er < imax);

	std::vector<int>().swap(indices);//That will create an empty vector with no memory allocated and swap it with indices, effectively deallocating the memory.

	for (int i = 0; i < _AMOUNT_DELTA_WORLDS; i++) {
		if (!this->WorldParts[i].isInitialized()) {
			std::cout << "ERROR: Some WorldParts remain uninitialized!" << std::endl;
			std::getchar();
			return false;
		}
	}
	return true;
}

void World::createSetHeightRand() {

	////randoomize Perlian Noice
	int x;//koordinaten
	int y;//koordinaten
	int xOffset;
	int yOffset;

	std::vector<double> perlNoiseSmootFaktor;
	perlNoiseSmootFaktor.reserve(_AMOUNT_DELTA_WORLDS);

	SimplexNoise1234 Noise = SimplexNoise1234(true);
	//Verteilung der Krümmung
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear	

	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;
			perlNoiseSmootFaktor.push_back(double(Noise.noise(x*_CURVATURE_CHANGE_RATE_PERLIAN_NOISE + xOffset, y*_CURVATURE_CHANGE_RATE_PERLIAN_NOISE + yOffset)));//-1...1
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 0.1) / 2.0;//0...1
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) * (_MIN_CURVATURE_PERLIAN_NOISE + _MAX_CURVATURE_PERLIAN_NOISE);//0..._max - _min
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) + _MIN_CURVATURE_PERLIAN_NOISE;//_max ... _min
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Krümmung ändern darf in i


	//Verzerrung
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;
			perlNoiseSmootFaktor.at(i) = Noise.noise(x*perlNoiseSmootFaktor.at(i) + xOffset, y*perlNoiseSmootFaktor.at(i) + yOffset);
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 0.1) / 2.0;//0...1
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) * (_MIN_DISTORTION_PERLIAN_NOISE + _MAX_DISTORTION_PERLIAN_NOISE);
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) + _MIN_DISTORTION_PERLIAN_NOISE;
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Steigung ändern darf in i

	//Zoom
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

	for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
		for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_WORLD_DIMENSION + yi;
			perlNoiseSmootFaktor.at(i) = Noise.noise(x*perlNoiseSmootFaktor.at(i) + xOffset, y*perlNoiseSmootFaktor.at(i) + yOffset);
			perlNoiseSmootFaktor.at(i) = (perlNoiseSmootFaktor.at(i) + 0.1) / 2.0;//0...1
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) * (_MIN_ZOOM_PERLIAN_NOISE + _MAX_ZOOM_PERLIAN_NOISE);
			perlNoiseSmootFaktor.at(i) = perlNoiseSmootFaktor.at(i) + _MIN_ZOOM_PERLIAN_NOISE;
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Höhe ändern darf in i

	//Höhe
	xOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MIN_OFFSET_PERLIAN_NOISE, _MAX_OFFSET_PERLIAN_NOISE);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

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
			for (int i = 0; i < 8; i++) {
				//TODO Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
			}
			this->WorldParts.at(i).setRandRegion(height, NeigbourRegionId);
		}
	}
	std::vector<double>().swap(perlNoiseSmootFaktor);//That will create an empty vector with no memory allocated and swap it with perlNoiseSmootFaktor, effectively deallocating the memory.

}

void World::createSetHeightPredefined() {
	std::cout << "create world from predefined image" << std::endl;
	sf::Vector2u ImageSize = this->WorldHeightMap.getSize();

	u_int *a = new u_int[ImageSize.x*ImageSize.y];//array aller Pixel RGB
	u_char *b = new u_char[_AMOUNT_DELTA_WORLDS];//array resultierende Pixel GREY

	for (unsigned int x = 0; x < ImageSize.x; x++) {
		for (unsigned int y = 0; y < ImageSize.y; y++) {
			sf::Color pix = this->WorldHeightMap.getPixel(x, y);
			a[x*ImageSize.x + y] = (pix.r << 16) | (pix.g << 8) | (pix.b);
		}
	}
	resample(a, b, ImageSize.x, ImageSize.y, _WORLD_DIMENSION, _WORLD_DIMENSION);

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

			int NeigbourRegionId[8];
			int Region_of_neigbours[8];
			for (int i = 0; i < 8; i++) {
				//TODO Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
			}
			this->WorldParts.at(i).setRandRegion(height, NeigbourRegionId);
		}
	}
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
	double pt_T = PT1_T_Discrete(this->_G_->_DeltaTime, _PT_1_T_SEASONAL_OFFSET); //get the discret t constant of a dynamic pt1 behaviour
	for (int s = 0; s < _AMOUNT_SEASONS; s++) {
		T = TemperateZone(s);
		//To avoid sudden changes CurrentYearTempOffset is moddeled with a pt1 behaviour
		this->_G_->CurrentYearTempOffset[s] = pt_T*(this->_G_->NextYearTempOffset[s] - this->_G_->CurrentYearTempOffset[s]) + this->_G_->CurrentYearTempOffset[s]; //adding the last difference to prevent huge changes and create trends
	}

	//UPDATE DeltaWorld

	//BOOST_FOREACH
	BOOST_FOREACH(DeltaWorld WorldPart, this->WorldParts) {
		WorldPart.setSeason(this->_G_->_SeasonShift + WorldPart.getPosition().getArg());
		WorldPart.simulate(this->time);
	}
	//BOOST_FOREACH

	BOOST_FOREACH(Animal animal, this->Animals) {
		animal.simulate(this->time);
	}

	//for-schleife langsamer als Boost
	//for (int i = 0; i < _Amount_Delta_Worlds; i++) {
	//	this->WorldParts[i]->setSeason(this->_G_->_SeasonShift + this->WorldParts[i]->getPosition().getArg());
	//	this->WorldParts[i]->simulate(this->time);
	//}
	//for-schleife

	//Update Animal
}

bool World::getIsReady() {
	return this->init;
}

void World::getNeigbourRegionId(int neigbourRegionId[], int x, int y, const int num_neigbours = 8) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
	neigbourRegionId[0] = this->WorldParts.at((x+1)*_WORLD_DIMENSION + y).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y-1).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y).getRegionId();
	neigbourRegionId[0] = this->WorldParts.at((x + 1)*_WORLD_DIMENSION + y).getRegionId();

}
void World::getNeigbourRegionId(int neigbourRegionId[], int at, const int num_neigbours = 8) {
	//Array with the region id for all neigbours: right, topRight, top, topLeft, left, botLeft, bot, bottRight
}