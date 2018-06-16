#ifndef _PLANT_
#define _PLANT_
#include "structs.h"

class Plant {
protected:
	MinMax GrowTemperatur;		//where it can grow
	MinMax DieTemperature;
	double MaxAltitude;		//where it can grow
	double NutritionalValue;
	double digestable;		//from 1 to n; 1 means easy n means difficult (multiplier for digestion in animal)
	double maxPopulationPerMeterSquared;
};

class Weed : public Plant {
public:
	Weed() {
		this->GrowTemperatur = MinMax(0, 35);
		this->DieTemperature = MinMax(-25, 50);
		this->MaxAltitude = 3000;
		this->NutritionalValue = 10;
		this->digestable = 1;

	}
};
#endif // !_PLANT_

