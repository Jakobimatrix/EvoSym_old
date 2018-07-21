#ifndef _PLANT_
#define _PLANT_
#include "structs.h"

class Plant {
protected:
	MinMax grow_temperature;		//where it can grow
	MinMax die_temperature;
	double max_altitude;		//where it can grow
	double nutritional_value;
	double digestable;		//from 1 to n; 1 means easy n means difficult (multiplier for digestion in animal)
	double max_population_per_square_meter;
};

class Weed : public Plant {
public:
	Weed() {
		this->grow_temperature = MinMax(0, 35);
		this->die_temperature = MinMax(-25, 50);
		this->max_altitude = 3000;
		this->nutritional_value = 10;
		this->digestable = 1;

	}
};
#endif // !_PLANT_

