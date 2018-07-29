#ifndef _GLOBALS_
#define _GLOBALS_
#include <thread>
#include <string>
#include <math.h>
#include <iostream>
#include <chrono>
#include "structs.h"

//constexpression constexbr -> ändern wird zur compilezeit erstellt
//wind?


//constants
constexpr double _PI = 3.141592653589793238463; //[math.const] The constant pi.
constexpr double _PI_HALF = _PI / 2.0;  //[math.const] The constant pi/2.
constexpr double _INVERSE_PI = 0.318309886183790671537; //[math.const] The inverse of the constant pi.
constexpr double _2PI = 2 * 3.141592653589793238463; //[math.const] Two times the constant pi.
constexpr double _INVERSE_2PI = 1.0 / _2PI;	//[math.const] The inverse of two times the constant pi.
constexpr double _PI_QUARTER = 0.785398163397448309615; //[math.const] A fourth of the constant pi.

//Dimensiones of this world
constexpr int _AMOUNT_REGIONS = 10; //[int] The amount of defined regions.
constexpr int _AMOUNT_TEMPERATE_ZONES = 4; //[int] The amount of defined temperature zones.
constexpr int _AMOUNT_SEASONS = 4; //[int] the amount of defined seasons.
constexpr int _AMOUNT_NEIGHBOURS = 8;
constexpr double _EARTH_ACCELERATION = 9.81; //[m/s^2]
constexpr double _WORK_REDUCTION_DOWNWARDS = 0.3; // walking down needs also a little energy m*g*h*_WORK_REDUCTION_DOWNWARDS
constexpr double _WATER_WEIGHT_PER_CUBIC_METER = 1000; //[Kg]

constexpr double _LAND_TO_OCEAN_RATIO = 1.3; //[ratio] This ratio determines how much land compared to ocean there shall be.
constexpr int _DIMENSION_HALF = 200; //[DeltaWorld] The radius in tiles the world should consist of. Program Crashes for more than 300.
constexpr int _WORLD_DIMENSION = _DIMENSION_HALF * 2; //[DeltaWorld] The diameter in tiles the world consist of.
constexpr int _AMOUNT_DELTA_WORLDS = _WORLD_DIMENSION * _WORLD_DIMENSION; //[DeltaWorld] The amount of all tiles creating this world.
constexpr double _WORLD_DIAMETER = 40000000.0; // [m] The Diameter of the created world in meters. This sets the size of the world in relation to the animals.
constexpr double _WORLD_DIAMETER_HALF = _WORLD_DIAMETER / 2.0; [m]; //The radius of the created world in meters.
constexpr double _DELTA_SIZE = _WORLD_DIAMETER/ _AMOUNT_DELTA_WORLDS; //[m] The size of one tile of the world in meter.
constexpr double _DELTA_SIZE_HALF = _DELTA_SIZE / 2.0; //the half of the size of one side of an tile
constexpr double _MAX_TERRANE_HEIGHT = 3000;//[m] The highst possible point of the world.
constexpr double _MIN_TERRANE_HEIGHT = -3000;//[m] The lowest possible point of this world.

//TileMap
const std::string _SRC_TILE_IMAGE = "bilder/backgrounds/Tiles.png";//[path] Path to the tile map.
constexpr int _TILE_RESULUTION = 500;//[pixel] Resulution of the individual tile pattern in _SRC_TILE_IMAGE.

//Drawing
constexpr double _VISUALIZED_TEMPERATURE_DIFFERENCE = 0.5; //[°C] At which temperature difference a tile should change apearence. The smaller this number the more often a tile has to be redrawn.
const std::string _DEFINED_MAP = "bilder/map2.jpg"; //[path] Loades a hight profile from an black and white image. If empty the height profile will be randomized with perlian noise.
constexpr int _WORLD_DIAMETER_IN_PIXEL = _WORLD_DIMENSION*_TILE_RESULUTION; //[pix] gesammt anzahl der Pixel in Länge und breite

constexpr std::chrono::milliseconds _MIN_DIFFERENCE_BETWEEN_CKLICKS = std::chrono::milliseconds(150);

//Parameter for a random generated world with perlian noise.

/*Perlian noise is like the pattern of a military uniform. This pattern is only pseudorandom but i can choose randomly with
the offset O (_MIN_OFFSET_PERLIAN_NOISE < O < _MAX_OFFSET_PERLIAN_NOISE) where to start on a large predifined pattern map.*/
constexpr int _MAX_OFFSET_PERLIAN_NOISE = 1000000;			//[int] Empirical value, should not be more than  1000000.
constexpr int _MIN_OFFSET_PERLIAN_NOISE = _WORLD_DIMENSION;	//[int] Dont go into the negatives of the perlian noise.

//divided by _DIMENSION_HALF to assure the same pattern independent from the amount of tiles 
/*One way to make perlian noise more random is to zoom randomly into the predefined pattern map.*/
constexpr double _MIN_ZOOM_PERLIAN_NOISE = 0.6 / _DIMENSION_HALF;//Min zoomfactor 1.1->less Islands.
constexpr double _MAX_ZOOM_PERLIAN_NOISE = 3.0 / _DIMENSION_HALF;//Max zoomfactor 3.0->many Islands. 

/*With these factors we can distort the pattern of the perlian noise with a second perlian noise pattern.*/
constexpr double _MIN_DISTORTION_PERLIAN_NOISE = 0.0;					//Min distorting factor for the perlian noise map. 0.0-> no distortion
constexpr double _MAX_DISTORTION_PERLIAN_NOISE = 3.0 / _DIMENSION_HALF;	//Max distorting factor for the perlian noise map. 3->strong distortion.

/*With these factors we can distort the second perlian noise pattern with a third pattern. This will cause
localy strong changes in the resulting pattern.*/
constexpr double _CURVATURE_CHANGE_RATE_PERLIAN_NOISE = 0.5 / _DIMENSION_HALF; //Constant changerate for the pattern.
constexpr double _MIN_CURVATURE_PERLIAN_NOISE = 0.0;					//Min. factor 0 -> no peaks / sudden changes in height. 
constexpr double _MAX_CURVATURE_PERLIAN_NOISE = 0.28 / _DIMENSION_HALF;	//Max. factor 200.0 -> strong peaks / sudden changes in height.


//Simulation Time units
constexpr double _YEAR_IN_S = 31536000.0; //[s] 365*24*60*60
constexpr double _QUARTER_YEAR_IN_S = _YEAR_IN_S*0.25; //[s] 365*24*60*60*0.25
constexpr double _INVERSE_QUARTER_YEAR_IN_S = 1.0/ _QUARTER_YEAR_IN_S; //[1/s] 1/(365*24*60*60*0.25)
constexpr double _INVERSE_YEAR_IN_S = 1.0/ _YEAR_IN_S; //[1/s] 1/(365*24*60*60)
constexpr double _MONTH_IN_S = 30 * 24 * 60 * 60; //[s] 30*24*60*60
constexpr double _INVERSE_MONTH_IN_S = 1.0/ _MONTH_IN_S; //[1/s] 1/(30*24*60*60)
constexpr double _YEAR_IN_D = 8544; //[s] 365*24
constexpr double _DAY_IN_S = 86400; //[s] 24*60*60
constexpr double _INVERSE_DAY_IN_S = 1/ _DAY_IN_S; //[1/s] 1/(24*60*60)
constexpr double _HOUR_IN_S = 3600; //[s] 60*60
constexpr double _INVERSE_HOUR_IN_S = 1 / _HOUR_IN_S; //[1/s] 17(60*60)
constexpr double _MINUTE_IN_S = 60; //[s] 60
constexpr double _INVERSE_MINUTE_IN_S = 1 / _MINUTE_IN_S; //[1/s] 1/60

//simulation temperature
constexpr double _TIME_DEPENDANTCY_FOR_TEMP_IN_S = 60 * 60 * 24 *7; //[s] After this time intervall the calculation for the new temperature depends only on seasonal influences.
constexpr double _INVERSE_TIME_DEPENDANTCY_FOR_TEMP_IN_S = 1/ _TIME_DEPENDANTCY_FOR_TEMP_IN_S; //[1/s] After 1/(this time intervall) the calculation for the new temperature depends only on seasonal influences.
constexpr double _PT_1_T_SEASONAL_OFFSET = 10000000; //[/] Good values between: [4.233.600 - 86.313.600] Pt_1 diskrete time constant for the change of the seasonal offset between years.
//!!!The summ of all _TEMP_INFLUENCE_... has to be 1.0! 
constexpr double _TEMP_INFLUENCE_NEIGHBOURS = 7.0 / 11.0;// 0.37; //[ratio] How much influence neighbour tiles have on the temperature.
constexpr double _TEMP_INFLUENCE_TEMPERATE_ZONE = 1.0 / 11.0;// 0.45; //[ratio] How much influence the temperate zone has on the temperature.
constexpr double _TEMP_INFLUENCE_IS_TEMP = 1 / 11.0;// 0.09; //[ratio] How much the temperature of the last simulation step influences the new temperature.
constexpr double _TEMP_INFLUENCE_GROUND = 2 / 11.0;// 0.09; //[ratio] How much the temperature of the ground influences the new temperature.
//constexpr double _TEMP_INFLUENCE_GROUND_TEMP; //[ratio] How much the groundtemperature the temperature influences.

//simulation physics
constexpr double _WATER_FREEZING_TEMPERATURE[2] = { -0.2, -1.9 }; //[°C,°C] At this temperature [water, marine water] begin to freeze.
constexpr double _TEMPERATURE_DROP_PER_METER = -0.005; //[°C/m] With every meter the average temperature drops by this value.
constexpr double _BEGIN_HEIGHT_TEMP_DROP = 0; //[m] Above this value the average temperature drops per meter with _TEMPERATURE_DROP_PER_METER.

constexpr double _plants_stop_growth_temp = 4.0;//this should be defined for each region 

constexpr int _AMOUNT_NEW_INPUT_PARAMS_NN = 25; //number of new input parameters for the neuronal network for each animal
constexpr int _MEMORY_DEPTH_NN = 4; //how output of output neurons get passed back to the input neurons at the next decision step
constexpr int _AMOUNT_HIDDEN_NEURONS_1_NN = 100; //number of neurones for each hidden layer
constexpr int _AMOUNT_HIDDEN_NEURONS_2_NN = 100; //number of neurones for each hidden layer
constexpr int _AMOUNT_ACTION_OUTPUT_NEURONES_NN = 10; //all possible actions an animal can do 
constexpr int _AMOUNT_MEMORY_OUTPUT_NEURONES_NN = 10; //places for active memory the animal might use as it likes
constexpr int _AMOUNT_OUTPUT_NEURONES_NN = _AMOUNT_ACTION_OUTPUT_NEURONES_NN + _AMOUNT_MEMORY_OUTPUT_NEURONES_NN; //all possible actions an animal can do + "memory" the animal can use to save things
constexpr int _AMOUNT_ALL_INPUT_PARAMS_NN = _AMOUNT_NEW_INPUT_PARAMS_NN + _MEMORY_DEPTH_NN *_AMOUNT_OUTPUT_NEURONES_NN; //number of input parameters for the neuronal network for each animal + memory



///animalPropertys
constexpr double _WATER_LOSS_FACTOR = 0.1; // if a action needs X energie, it needs _WATER_LOSS_FACTOR*X water.
constexpr double _DECAY_FACTOR_FAT = 0.25 / _DAY_IN_S; //kg/s
constexpr double _DECAY_FACTOR_SUGAR = _DECAY_FACTOR_FAT * 20; //kg/s
constexpr double _WATER_FRICTION_MULTIPLICATOR = 3.0; //how much more energy traveling in water costs
constexpr double _MIN_FLYING_WEIGHT_PER_SIZE_RATIO = 0.001; //how dense an anmal needs to be to be able to fly
constexpr double _MIN_FLYING_FEATHERS = 0.7;

constexpr uint16_t _scent_decay[5] = { 0b101010101010101,0b101010101010101,0b101010101010101,0b101010101010101,0b101010101010101};
//energyconsumption
//size: (factor*weight)^exponent
constexpr double _energy_consumption_size_factor = 2.0;
constexpr double _energy_consumption_size_exponent = 3.0;
//size: (factor*size)^exponent
constexpr double _energy_consumption_weight_factor = 0.2;
constexpr double _energy_consumption_weight_exponent = 5.2;



//Resourcengerneration

/**
* @function cosResurceGeneration f(double max, double Tau, double t)
* @brief: Resourc regeneration: between f(t = 0) = 0, and f(t = +-Tau) = +-max
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
* @brief: Calculates how many time passed given the amount of resources currently avaiable: between f(resources = 0) = 0, and f(resources = max) = Tau
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
* @brief: Calculates the inverse of the regeneration rate Tau considering temperature at which plants grow best.
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

//veränderliche
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
	double _DELTA_SEASON_CHANGE_GRAD;//[grad] wie viel Grad die season pro simulierten zeitabschnitt wandern. 360° / Simulationseinheiten pro Jahr
	double _DELTA_SEASON_CHANGE_RAD;

	double CurrentYearTempOffset[4];
	double NextYearTempOffset[4];

	double SeasonMultiplier[4];//-1 bis 1; //in welcher Season wir uns gerade befinden the positive numbers always add to 1 == 100%
	int dominantSeason;

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
		this->setSeason();
		this->_BREITENGRAD = MinMax(0.0, 90.0);
		/*Wikipedia:
		Tropen von 0°–23,5°			90° - 66,5°
		Subtropen von 23,5°–40°		66,5°-50°
		gemäßigte Zone von 40°–60°	50° - 30°
		kalte Zone von 60°–90°		30° - 0°
		*/
		this->_POLAR_latitude = MinMax(0.0, 16.0);
		this->_MODERATE_latitude = MinMax(16.0, 50.0);
		this->_SUBTROPE_latitude = MinMax(50.0, 70.0);
		this->_TROPICAL_latitude = MinMax(70.0, 90.0);

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
	void setSeason() {
		this->SeasonMultiplier[0] = sin(_SeasonShift);
		this->SeasonMultiplier[1] = cos(_SeasonShift);
		double vzSin = 1.0;
		double vzCos = 1.0;
		if (this->SeasonMultiplier[0] < 0) {
			vzSin = -1.0;
		}
		if (this->SeasonMultiplier[1] < 0) {
			vzCos = -1.0;
		}
		this->SeasonMultiplier[0] = vzSin * this->SeasonMultiplier[0] * this->SeasonMultiplier[0];
		this->SeasonMultiplier[1] = vzCos * this->SeasonMultiplier[1] * this->SeasonMultiplier[1];
		this->SeasonMultiplier[2] = -this->SeasonMultiplier[0];
		this->SeasonMultiplier[3] = -this->SeasonMultiplier[1];

		double max = 0;
		for (int i = 0; i < _AMOUNT_SEASONS; i++) {
			if (this->SeasonMultiplier[i] > max) {
				max = this->SeasonMultiplier[i];
				dominantSeason = i;
			}
		}
	}

};


#endif // !_GLOBALS_

