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

void World::createWorld() {
	//int *ary = new int[sizeX*sizeY];
	//// ary[x][y] is then rewritten as
	//ary[x*sizeY + y]

	int x;//koordinaten
	int y;//koordinaten
	for (int xi = 0; xi < _World_Dimension; xi++) {//pixel
		for (int yi = 0; yi < _World_Dimension; yi++) {//pixel
			x = xi - _DIMENSION_HALF;	//koordinaten (x,y)
			y = yi - _DIMENSION_HALF;	//koordinaten (x,y)
			int i = (xi)*_World_Dimension + yi;	//array element

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
			
			this->WorldParts[i]->setPositionAndLatitude(DeltaPosition, latitude);
			this->WorldParts[i]->setSeason(angle);//season
		}
	}

	//4.Give every DeltaWorld his Neigbours
	for (int xi = 0; xi < _World_Dimension; xi++) {//pixel
		for (int yi = 0; yi < _World_Dimension; yi++) {//pixel

			int i = (xi)*_World_Dimension + yi;	//array element

			DeltaWorld* left	= nullptr;
			DeltaWorld* right	= nullptr;
			DeltaWorld* top		= nullptr;
			DeltaWorld* bottom	= nullptr;

			DeltaWorld* top_left		= nullptr;
			DeltaWorld* top_right		= nullptr;
			DeltaWorld* bottom_left		= nullptr;
			DeltaWorld* bottom_right	= nullptr;

			bool left_border_violated	= (xi - 1 > -1) ? false : true;
			bool right_border_violated	= (xi + 1 < _World_Dimension) ? false : true;

			bool top_border_violated	= (yi - 1 > -1) ? false : true;
			bool bottom_border_violated = (yi + 1 < _World_Dimension) ? false : true;
			
			int in;
			if (!left_border_violated) {//left
				in = (xi - 1)*_World_Dimension + yi;
				left = this->WorldParts[in];
			}
			if (!right_border_violated) {//right
				in = (xi + 1)*_World_Dimension + yi;
				right = this->WorldParts[in];
			}
			if (!bottom_border_violated) {//bottom
				in = (xi)*_World_Dimension + yi + 1;
				bottom = this->WorldParts[in];

				if (!left_border_violated) {//bottom-left
					in = (xi - 1)*_World_Dimension + yi + 1;
					bottom_left = this->WorldParts[in];
				}

				if (!right_border_violated) {//bottom-right
					in = (xi + 1)*_World_Dimension + yi + 1;
					bottom_right = this->WorldParts[in];
				}
			}
			if (!top_border_violated) {//top
				in = (xi)*_World_Dimension + yi - 1;
				top = this->WorldParts[in];

				if (!left_border_violated) {//top-left
					in = (xi - 1)*_World_Dimension + yi - 1;
					top_left = this->WorldParts[in];
				}

				if (!right_border_violated) {//top-right
					in = (xi + 1)*_World_Dimension + yi - 1;
					top_right = this->WorldParts[in];
				}
			}					
			this->WorldParts[i]->setNeigbours(left, right, top, bottom, bottom_right, bottom_left,top_left,top_right);
		}
	}

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
	int* indices[_Amount_Delta_Worlds];
	for (int i = 0; i < _Amount_Delta_Worlds; i++) {
		indices[i] = new int(i);
	}
	do{
		std::random_shuffle(indices, indices + _Amount_Delta_Worlds);//shuffle the idices for rand access
		erosion = false;
		i_er++;
		for (int i = 0; i < _Amount_Delta_Worlds; i++) {
			if (! this->_RG_->getRegion(this->WorldParts[*indices[i]]->getRegionId())->hasEnoughNeigboursWithSameRegion(this->WorldParts[*indices[i]]->getNumNeigboursWithSameRegion())) {
				this->WorldParts[*indices[i]]->changeRegionToFitNeigbours();
				bool erosion = true;
			}
		}
	} while (erosion && i_er < imax);

	for (int i = 0; i < _Amount_Delta_Worlds; i++) {
		delete indices[i];
	}
}

void World::createSetHeightRand() {

	////randoomize Perlian Noice
	int x;//koordinaten
	int y;//koordinaten
	int xOffset;
	int yOffset;
	double* perlNoiseSmootFaktor[_Amount_Delta_Worlds];//smoothfactor f�r die H�hendistribution

	SimplexNoise1234 Noise = SimplexNoise1234(true);
	//Verteilung der Kr�mmung
	xOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear	

	for (int xi = 0; xi < _World_Dimension; xi++) {
		for (int yi = 0; yi < _World_Dimension; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_World_Dimension + yi;
			perlNoiseSmootFaktor[i] = new double(Noise.noise(x*_smoothingfactor_krummungChnage + xOffset, y*_smoothingfactor_krummungChnage + yOffset));//-1...1
			*perlNoiseSmootFaktor[i] = (*perlNoiseSmootFaktor[i] + 0.1) / 2.0;//0...1
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] * (_minSmoothFactor_krummungChnage + _maxSmoothFactor_krummungChnage);//0..._max - _min
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] + _minSmoothFactor_krummungChnage;//_max ... _min
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Kr�mmung �ndern darf in i


	//Verzerrung
	xOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

	for (int xi = 0; xi < _World_Dimension; xi++) {
		for (int yi = 0; yi < _World_Dimension; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_World_Dimension + yi;
			*perlNoiseSmootFaktor[i] = Noise.noise(x**perlNoiseSmootFaktor[i] + xOffset, y**perlNoiseSmootFaktor[i] + yOffset);
			*perlNoiseSmootFaktor[i] = (*perlNoiseSmootFaktor[i] + 0.1) / 2.0;//0...1
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] * (_minSmoothFactor_Verzerrung + _maxSmoothFactor_Verzerrung);
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] + _minSmoothFactor_Verzerrung;
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die Steigung �ndern darf in i

	//Zoom
	xOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

	for (int xi = 0; xi < _World_Dimension; xi++) {
		for (int yi = 0; yi < _World_Dimension; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_World_Dimension + yi;
			*perlNoiseSmootFaktor[i] = Noise.noise(x**perlNoiseSmootFaktor[i] + xOffset, y**perlNoiseSmootFaktor[i] + yOffset);
			*perlNoiseSmootFaktor[i] = (*perlNoiseSmootFaktor[i] + 0.1) / 2.0;//0...1
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] * (_minSmoothFactor_Zoom + _maxSmoothFactor_Zoom);
			*perlNoiseSmootFaktor[i] = *perlNoiseSmootFaktor[i] + _minSmoothFactor_Zoom;
		}
	}
	//in perlNoiseSmootFaktor[i] steht jetzt, wie schnell sich die H�he �ndern darf in i

	//H�he
	xOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear
	yOffset = uniform_int_dist(_MinOffset, _MaxOffset);//since perlian noice is only pseudo random, this is the only posibility to have a bit of real randomness appear

	for (int xi = 0; xi < _World_Dimension; xi++) {
		for (int yi = 0; yi < _World_Dimension; yi++) {
			x = xi - _DIMENSION_HALF;
			y = yi - _DIMENSION_HALF;
			int i = (xi)*_World_Dimension + yi;
			//height

			double height = Noise.noise(x*(*perlNoiseSmootFaktor[i]) + xOffset, y*(*perlNoiseSmootFaktor[i]) + yOffset);//return [-1,1]

			//|-1        |0       |1 => ratio Land/Ocean		 = 1/1-> dx = 0;
			//dx |-2/5   |0          |8/5 => ratio Land/Ocean	 = 4-> dx = 3/5
			//um wie viel muss verschoben werden (dx), damit gegebener Ratio stimmt?
			//L/O = ratio (1)
			//L + O = 2 = 1-(-1) -> L = 2 - O in (1)
			//(2 -O)/O = ratio -> O = 2/(ratio + 1)
			//dx = 1 - O = 1 - 2/(ratio +1)
			double dx = 1 - 2 / (_LandToOceanRate + 1);
			height = height + dx; //verschiebe "See level"
			if (height < 0) {
				height = height*(1 / (-1 + dx));// [1 0];
				height = height * _MinHeight;	// [-min,0]
			}
			else {
				height = height*(1 / (1 + dx));	// [1 0];
				height = height * _MaxHeight;	// [0,max]
			}
			//region
			//gehe alle Nachbarn durch, errechne f�r jeden Nachbarn die Wahrscheinlichkeit f�r this DeltaWorld Region
			//Durch H�he und Breitengrad wird die Auswahl begrenzt
			this->WorldParts[i]->setRandRegion(height);
		}
	}
	for (int i = 0; i < _Amount_Delta_Worlds; i++) {
		delete perlNoiseSmootFaktor[i];
	}
}

void World::createSetHeightPredefined() {

	sf::Vector2u ImageSize = this->WorldHeightMap.getSize();

	u_int *a = new u_int[ImageSize.x*ImageSize.y];//array aller Pixel RGB
	u_char *b = new u_char[_Amount_Delta_Worlds];//array resultierende Pixel GREY

	for (unsigned int x = 0; x < ImageSize.x; x++) {
		for (unsigned int y = 0; y < ImageSize.y; y++) {
			sf::Color pix = this->WorldHeightMap.getPixel(x, y);
			a[x*ImageSize.x + y] = (pix.r << 16) | (pix.g << 8) | (pix.b);
		}
	}
	resample(a, b, ImageSize.x, ImageSize.y, _World_Dimension, _World_Dimension);

	for (unsigned int x = 0; x < _World_Dimension; x++) {
		for (unsigned int y = 0; y < _World_Dimension; y++) {
			int i = x*_World_Dimension + y;
			double height = b[i];//[0...255];
			height = height - 127.5;
			height = height / 127.5;//[-1...1]


			if (height < 0) {
				height = -height;// [1 0];
				height = height * _MinHeight;	// [-min,0]
			}
			else {
				height = height * _MaxHeight;	// [0,max]
			}
			//region
			//gehe alle Nachbarn durch, errechne f�r jeden Nachbarn die Wahrscheinlichkeit f�r this DeltaWorld Region
			//Durch H�he und Breitengrad wird die Auswahl begrenzt
			this->WorldParts[i]->setRandRegion(height);
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
	if (y && tRest > _YEAR_in_s) {
		t = std::floor(tRest * _1_divided_YEAR_in_s);
		if (t > 0.0) {
			output += "Y: " + std::to_string((int)t);
			tRest -= t*_YEAR_in_s;
		}
	}
	if (d && tRest > _DAY_in_s) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _1_divided_DAY_in_s);
		if (t > 0.0) {
			output += "d: " + std::to_string((int)t);
			tRest -= t*_DAY_in_s;
		}		
	}
	if (h && tRest > _Hour_in_s) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _1_divided_Hour_in_s);
		if (t > 0.0) {
			output += "h: " + std::to_string((int)t);
			tRest -= t*_Hour_in_s;
		}				
	}
	if (m && tRest > _Minute_in_s) {
		if (output != "") {
			output += " ";
		}
		t = std::floor(tRest * _1_divided_Minute_in_s);
		if (t > 0.0) {
			output += "m: " + std::to_string((int)t);
			tRest -= t*_Minute_in_s;
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

	if (this->_G_->_SeasonShift < -_2PI) {
		this->_G_->_SeasonShift = 0;//ein Jahr ist um
		TemperateZone T;
		double offset = 0.0;
		for (int s = 0; s < _amountSeasons; s++) {
			T = TemperateZone(s);
			this->_G_->CurrentYearTempOffset[s] = this->_G_->NextYearTempOffset[s];
			offset = 0.6 * offset + 0.4 * T.getNewYearTempVariation(); //damit der Unterschied zwischen zwei benachbarten Zohnen nicht zu arg unterschiedlich ist
			this->_G_->NextYearTempOffset[s]	= 0.2 * offset + this->_G_->CurrentYearTempOffset[s] * 0.8; //adding the last difference to prevent huge changes and create trends
		}
	}

	//UPDATE DeltaWorld

	//BOOST_FOREACH
	BOOST_FOREACH(DeltaWorld* WorldPart, this->WorldParts) {
		WorldPart->setSeason(this->_G_->_SeasonShift + WorldPart->getPosition().getArg());
		WorldPart->simulate(this->time);
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
