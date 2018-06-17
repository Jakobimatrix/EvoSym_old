#ifndef _GLOBALS_
#define _GLOBALS_
#include <thread>
#include <string>
#include <math.h>
#include <iostream>
#include "structs.h"

//constants
const double _PI = 3.141592653589793238463; //[math.const] The constant pi.
const double _INVERSE_PI = 0.318309886183790671537; //[math.const] The inverse of the constant pi.
const double _2PI = 2 * 3.141592653589793238463; //[math.const] Two times the constant pi.
const double _INVERSE_2PI = 1.0 / _2PI;	//[math.const] The inverse of two times the constant pi.
const double _PI_QUARTER = 0.785398163397448309615; //[math.const] A fourth of the constant pi.

//Dimensiones of this world
const int _AMOUNT_REGIONS = 11; //[int] The amount of defined regions.
const int _AMOUNT_TEMPERATE_ZONES = 4; //[int] The amount of defined temperature zones.
const int _AMOUNT_SEASONS = 4; //[int] the amount of defined seasons.

const double _LAND_TO_OCEAN_RATIO = 1.3; //[ratio] This ratio determines how much land compared to ocean there shall be.
const int _DIMENSION_HALF = 150; //[DeltaWorld] The radius in tiles the world should consist of. Program Crashes for more than 150.
const int _WORLD_DIMENSION = _DIMENSION_HALF * 2; //[DeltaWorld] The diameter in tiles the world consist of.
const int _AMOUNT_DELTA_WORLDS = _WORLD_DIMENSION * _WORLD_DIMENSION; //[DeltaWorld] The amount of all tiles creating this world.
const double _WORLD_DIAMETER = 40000000.0; // [m] The Diameter of the created world in meters. This sets the size of the world in relation to the animals.
const double _DELTA_SIZE = _WORLD_DIAMETER/ _AMOUNT_DELTA_WORLDS; //[m] The size of one tile of the world in meter.
const double _MAX_TERRANE_HEIGHT = 3000;//[m] The highst possible point of the world.
const double _MIN_TERRANE_HEIGHT = -3000;//[m] The lowest possible point of this world.

//TileMap
const std::string _SRC_TILE_IMAGE = "bilder/backgrounds/Tiles.png";//[path] Path to the tile map.
const int _TILE_RESULUTION = 500;//[pixel] Resulution of the individual tile pattern in _SRC_TILE_IMAGE.

//Drawing
const double _VISUALIZED_TEMPERATURE_DIFFERENCE = 0.5; //[�C] At which temperature difference a tile should change apearence. The smaller this number the more often a tile has to be redrawn.
const std::string _DEFINED_MAP = "bilder/map2.jpg"; //[path] Loades a hight profile from an black and white image. If empty the height profile will be randomized with perlian noise.
const int _WORLD_DIAMETER_IN_PIXEL = _WORLD_DIMENSION*_TILE_RESULUTION; //[pix] gesammt anzahl der Pixel in L�nge und breite

//Parameter for a random generated world with perlian noise.

/*Perlian noise is like the pattern of a military uniform. This pattern is only pseudorandom but i can choose randoomly with
the offset O (_MIN_OFFSET_PERLIAN_NOISE < O < _MAX_OFFSET_PERLIAN_NOISE) where to start on a large predifined pattern map.*/
const int _MIN_OFFSET_PERLIAN_NOISE = -1000000;//[int] Empirical value, should not be less than -1000000.
const int _MAX_OFFSET_PERLIAN_NOISE =  1000000;//[int] Empirical value, should not be more than  1000000.

//divided by _DIMENSION_HALF to assure the same pattern independent from the amount of tiles 
/*One way to make perlian noise more randoom is to zoom randoomly into the predefined pattern map.*/
const double _MIN_ZOOM_PERLIAN_NOISE = 1.1 / _DIMENSION_HALF;//Min zoomfactor 1.1->less Islands.
const double _MAX_ZOOM_PERLIAN_NOISE = 3.0 / _DIMENSION_HALF;//Max zoomfactor 3.0->many Islands. 

/*With these factors we can distort the pattern of the perlian noise with a second perlian noise pattern.*/
const double _MIN_DISTORTION_PERLIAN_NOISE = 0.005 / _DIMENSION_HALF;//Min distorting factor for the perlian noise map. 0.005->weak distortion.
const double _MAX_DISTORTION_PERLIAN_NOISE = 0.03 / _DIMENSION_HALF;//Max distorting factor for the perlian noise map. 0.03->strong distortion.

/*With these factors we can distort the second perlian noise pattern with a third pattern. This will cause
localy strong changes in the resulting pattern.*/
const double _CURVATURE_CHANGE_RATE_PERLIAN_NOISE = 0.01 / _DIMENSION_HALF; //Constant changerate for the pattern. 0.01;
const double _MIN_CURVATURE_PERLIAN_NOISE = 10.0 / _DIMENSION_HALF; //Min. factor 10.0 -> weak peaks / sudden changes in height. 
const double _MAX_CURVATURE_PERLIAN_NOISE = 20.0 / _DIMENSION_HALF;//Max. factor 200.0 -> strong peaks / sudden changes in height.



//Simulation Time units
const double _YEAR_IN_S = 31536000.0; //[s] 365*24*60*60
const double _QUARTER_YEAR_IN_S = _YEAR_IN_S*0.25; //[s] 365*24*60*60*0.25
const double _INVERSE_QUARTER_YEAR_IN_S = 1.0/ _QUARTER_YEAR_IN_S; //[1/s] 1/(365*24*60*60*0.25)
const double _INVERSE_YEAR_IN_S = 1.0/ _YEAR_IN_S; //[1/s] 1/(365*24*60*60)
const double _MONTH_IN_S = 30 * 24 * 60 * 60; //[s] 30*24*60*60
const double _INVERSE_MONTH_IN_S = 1.0/ _MONTH_IN_S; //[1/s] 1/(30*24*60*60)
const double _YEAR_IN_D = 8544; //[s] 365*24
const double _DAY_IN_S = 86400; //[s] 24*60*60
const double _INVERSE_DAY_IN_S = 1/ _DAY_IN_S; //[1/s] 1/(24*60*60)
const double _HOUR_IN_S = 3600; //[s] 60*60
const double _INVERSE_HOUR_IN_S = 1 / _HOUR_IN_S; //[1/s] 17(60*60)
const double _MINUTE_IN_S = 60; //[s] 60
const double _INVERSE_MINUTE_IN_S = 1 / _MINUTE_IN_S; //[1/s] 1/60

//simulation temperature
const double _TIME_DEPENDANTCY_FOR_TEMP_S = 60 * 60 * 24 *7; //[s] After this time intervall the calculation for the new temperature depends only on seasonal influences.
const double _INVERSE_TIME_DEPENDANTCY_FOR_TEMP_S = 1/ _TIME_DEPENDANTCY_FOR_TEMP_S; //[1/s] After 1/(this time intervall) the calculation for the new temperature depends only on seasonal influences.
//!!!The summ of all _TEMP_INFLUENCE_... has to be 1.0! 
const double _TEMP_INFLUENCE_NEIGHBOURS = 0.40; //[ratio] How much influence neighbour tiles have on the temperature.
const double _TEMP_INFLUENCE_TEMPERATE_ZONE = 0.5; //[ratio] How much influence the temperate zone has on the temperature.
const double _TEMP_INFLUENCE_IS_TEMP = 0.05; //[ratio] How much the temperature of the last simulation step influences the new temperature.
const double _TEMP_INFLUENCE_GROUND = 0.05; //[ratio] How much the temperature of the ground influences the new temperature.
//const double _TEMP_INFLUENCE_GROUND_TEMP; //[ratio] How much the groundtemperature the temperature influences.

//simulation physics
const double _WATER_FREEZING_TEMPERATURE[2] = { -0.2, -1.9 }; //[�C,�C] At this temperature [water, marine water] begin to freeze.
const double _TEMPERATURE_DROP_PER_METER = -0.01; //[�C/m] With every meter the average temperature drops by this value.
const double _BEGIN_HEIGHT_TEMP_DROP = 0; //[m] Above this value the average temperature drops per meter with _TEMPERATURE_DROP_PER_METER.



//this block willbe gone soon
const double _ice_tau_thickness_factor = 1000000.0;	//[1/(s*m)]
const double _ice_tau_per_grad_celsius_factor = 0.1;//[1/�C]
const double _ice_temperature_gradient = 0.1;
const double _ice_melt_temp_expo = 5;
const double _ice_freeze_temp_expo = 5;
//..

const double _plants_stop_growth_temp = 4.0;//this should be defined for each region 



///animalPropertys
const uint16_t _scent_decay[5] = { 0b101010101010101,0b101010101010101,0b101010101010101,0b101010101010101,0b101010101010101};
//energyconsumption
//size: (factor*weight)^exponent
const double _energy_consumption_size_factor = 2.0;
const double _energy_consumption_size_exponent = 3.0;
//size: (factor*size)^exponent
const double _energy_consumption_weight_factor = 0.2;
const double _energy_consumption_weight_exponent = 5.2;



//Resourcengerneration

/**
* @function cosResurceGeneration f(double max, double Tau, double t)
* @brief Resourc regeneration: between f(t = 0) = 0, and f(t = +-Tau) = +-max
* @param[in] max: Max capacity of the resource
* @param[in] Tau: After Tau seconds the resource is full. Tau != INF
* @param[in] t: Passed time.
* @retval double: Resourcen after t seconds.
**/
inline double cosResurceGeneration(double max, double Tau, double t) {
	if (max == 0) {
		return 0.0;
	}

	if (Tau > 0) {
		if (t >= Tau) {
			return max;
		}
	}
	else {
		if (t > 0) {
			return 0.0;
		}
	}

	return (0.5*-std::cos(_PI / Tau * t) + 0.5)*max;
}
/**
* @function inverse_cosResurceGeneration f(double max, double Tau, double resources)
* @brief Calculates how many time passed given the amount of resources currently avaiable: between f(resources = 0) = 0, and f(resources = max) = Tau
* @param[in] max: Max capacity of the resource.
* @param[in] Tau: After Tau seconds the resource is full. Tau != INF
* @param[in] resources: Amount of resources at this point of time.
* @retval double: Time needed to generate the given amount of resources.
**/
inline double inverse_cosResurceGeneration(double max, double Tau,  double resources) {
	if (resources >= max) {
		return Tau;
	}
	else if (resources <= 0.0) {
		return 0.0;
	}
	return Tau / _PI * std::acos(-2 * (resources / max - 0.5));

}
/**
* @function one_divid_Tau_depend_Temp(double temp, double bestGrowthTemp)
* @brief Calculates the inverse of the regeneration rate Tau considering temperature at which plants grow best.
* @param[in] temp: is-temperature.
* @param[in] bestGrowth: Optimal temperature for plant growth.
* @retval double: Inverse of Tau.
**/
inline double one_divid_Tau_depend_Temp(double temp, double bestGrowthTemp) {
	if (temp > _plants_stop_growth_temp) {
		double high_temp_growthBorder = bestGrowthTemp*2.0 - _plants_stop_growth_temp;
		if (temp < high_temp_growthBorder) {
			return (10.0 / _QUARTER_YEAR_IN_S)*(0.5*std::sin(_PI * temp / (bestGrowthTemp - _plants_stop_growth_temp) + _plants_stop_growth_temp) + 0.5);//todo factor
		}
		else {
			return (-1.0 / _QUARTER_YEAR_IN_S)*(high_temp_growthBorder - temp)*(high_temp_growthBorder - temp);
		}
	}
	else if(temp < 0.0){
		return (-1.0 / _QUARTER_YEAR_IN_S ) * temp * temp ;// Resource nimmt mit der Temperatur quadratisch ab.
	}
	else {
		return 0.0;
	}
}

//ver�nderliche
class GlobalSingleton {
public:
	double _SeasonShift;
	//definitionen der Temperaturzohnen in Breitengrade
	MinMax _BREITENGRAD;
	MinMax _POLAR_latitude;
	MinMax _MODERATE_latitude;
	MinMax _SUBTROPE_latitude;
	MinMax _TROPICAL_latitude;

	//DeltaTime
	double _DeltaTime;
	double _DELTA_SEASON_CHANGE_GRAD;//[grad] wie viel Grad die season pro simulierten zeitabschnitt wandern. 360� / Simulationseinheiten pro Jahr
	double _DELTA_SEASON_CHANGE_RAD;

	double CurrentYearTempOffset[4];
	double NextYearTempOffset[4];

	int THREADS;
	unsigned int logicalCores;
	unsigned int Cores;

private:
	// Private Constructor
	GlobalSingleton() {
		this->_DeltaTime = _YEAR_IN_S;
		double DeltaTime = 1;//jede sekunde wird simuliert
		DeltaTime = 60;//jede Minute wird simuliert
		DeltaTime = 60 * 60;//jede Stunde wird Simuliert
		DeltaTime = 60 * 60 * 24 ;//jeder Tag wird Simuliert

		this->announceDeltaTime(DeltaTime);

		this->_SeasonShift = 0.0;
		this->_BREITENGRAD = MinMax(0.0, 90.0);
		this->_POLAR_latitude = MinMax(0.0, 14.0);
		this->_MODERATE_latitude = MinMax(14.0, 35.0);
		this->_SUBTROPE_latitude = MinMax(35.0, 67.0);
		this->_TROPICAL_latitude = MinMax(67.0, 90.0);

		for (int i = 0; i < 4; i++) {
			this->CurrentYearTempOffset[i] = 0.0;
			this->NextYearTempOffset[i] = 0.0;
		}

		this->getCPUFeautures();
	}


	void cpuID(unsigned i, unsigned regs[4]) {
		#ifdef _WIN32
				__cpuid((int *)regs, (int)i);

		#else
				asm volatile
					("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
						: "a" (i), "c" (0));
				// ECX is set to zero for CPUID function 4
		#endif
	}
	void getCPUFeautures() {
		//https://stackoverflow.com/questions/150355/programmatically-find-the-number-of-cores-on-a-machine
		this->THREADS = std::thread::hardware_concurrency();//C++11

		unsigned regs[4];

		// Get vendor
		char vendor[12];
		cpuID(0, regs);
		((unsigned *)vendor)[0] = regs[1]; // EBX
		((unsigned *)vendor)[1] = regs[3]; // EDX
		((unsigned *)vendor)[2] = regs[2]; // ECX
		std::string cpuVendor = std::string(vendor, 12);

		// Get CPU features
		cpuID(1, regs);
		unsigned cpuFeatures = regs[3]; // EDX

										// Logical core count per CPU
		cpuID(1, regs);
		this->logicalCores = (regs[1] >> 16) & 0xff; // EBX[23:16]

		this->Cores = this->logicalCores;

		if (cpuVendor == "GenuineIntel") {
			// Get DCP cache info
			cpuID(4, regs);
			this->Cores = ((regs[0] >> 26) & 0x3f) + 1; // EAX[31:26] + 1

		}
		else if (cpuVendor == "AuthenticAMD") {
			// Get NC: Number of CPU cores - 1
			cpuID(0x80000008, regs);
			this->Cores = ((unsigned)(regs[2] & 0xff)) + 1; // ECX[7:0] + 1
		}
	}

	// Stop the compiler generating methods of copy the object
	GlobalSingleton(GlobalSingleton const& copy);            // Not Implemented
	GlobalSingleton& operator=(GlobalSingleton const& copy); // Not Implemented

public:
	static GlobalSingleton& getInstance()
	{
		// The only instance
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static GlobalSingleton instance;
		return instance;
	}
	void announceDeltaTime(double dt) {
		if (dt < this->_DeltaTime) {
			this->_DeltaTime = dt;
			this->_DELTA_SEASON_CHANGE_GRAD = (360.0  * _INVERSE_YEAR_IN_S) * this->_DeltaTime;
			this->_DELTA_SEASON_CHANGE_RAD = Deg2Rad(this->_DELTA_SEASON_CHANGE_GRAD);
		}
	}
};


#endif // !_GLOBALS_

