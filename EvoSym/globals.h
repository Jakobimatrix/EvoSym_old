#ifndef _GLOBALS_
#define _GLOBALS_
#include <thread>
#include <string>
#include <math.h>
#include <iostream>
#include "structs.h"


//TileMap

/**/
const std::string _srcTileImage = "bilder/backgrounds/Tiles.png";
const int _TileResulution = 500;//resulution of the individual tile pattern of _srcTileImage

/*
const std::string _srcTileImage = "bilder/backgrounds/TilesColor.png";
const int TileResulution = 1;//resulution of the individual tile pattern in pixel of _srcTileImage
*/
const double _NoticableTemperaturDifference = 0.5; //erst bei einem temp diff von ... wird die neue Temperatur angezeigt-> je höher die erlaubte diff ist, desto schneller die Sim, da weniger gezeichent wird
const std::string _DefinedMap = "bilder/map2.jpg"; //höhenprofiel Wenn string leer, wird rand generiert

const double _PI = 3.141592653589793238463;
const double _1_durch_PI = 0.318309886183790671537;
const double _2PI = 2 * 3.141592653589793238463;
const double _1_durch_2PI = 1.0 / _2PI;

const double _PI_4tel = 0.785398163397448309615;

///Dimensionen der Welt
const double _LandToOceanRate = 1.3;
const int _DIMENSION_HALF = 150; // [DeltaParts]//max 150
const int _World_Dimension = _DIMENSION_HALF * 2; //amount of tiles for one side
const int _Amount_Delta_Worlds = _World_Dimension*_World_Dimension; //amount of all tiles creating this world
const double _WorldDiameter = 40000000.0; // [m] Umfang = Durchmesser
const double _DELTA_SIZE = _WorldDiameter/ _Amount_Delta_Worlds; //[m]
const double _MaxHeight = 3000;//[m]
const double _MinHeight = -3000;//[m]

///Parameter für Random generation. Werte in Kommentar bezogen auf 200X200 Kacheln
const int _Graphic_pix_diameter = _World_Dimension*_TileResulution; //[pix] gesammt anzahl der Pixel in Länge und breite

const int _MinOffset = -1000000;//-1000000
const int _MaxOffset =  1000000;//-1000000

//divided by _DIMENSION_HALF to assure the same pattern independent from the amount of tiles 
const double _minSmoothFactor_Zoom = 1.1 / _DIMENSION_HALF;//Min smoothfactor für die Höhendistribution 1.1->wenig Inseln
const double _maxSmoothFactor_Zoom = 3.0 / _DIMENSION_HALF;//Max smoothfactor für die Höhendistribution 2.0->viele Inseln 

const double _minSmoothFactor_Verzerrung = 0.005 / _DIMENSION_HALF;//Min smoothfactor für die Steigungsdistribution 0.0.005->Schwache Verzerrung der Strucktur
const double _maxSmoothFactor_Verzerrung = 0.03 / _DIMENSION_HALF;//Max smoothfactor für die Steigungsdistribution 0.03->starke Verzerrung der Strucktur

const double _smoothingfactor_krummungChnage = 0.01 / _DIMENSION_HALF; //constant changerate for curvature 0.01;
const double _minSmoothFactor_krummungChnage = 10.0 / _DIMENSION_HALF; //10.0 -> schwache Peaks
const double _maxSmoothFactor_krummungChnage = 20.0 / _DIMENSION_HALF;//200.0 -> starke Peaks



///Simulations-DeltaEinheiten-Einheit
const double _YEAR_in_s = 31536000.0; //[s] 365*24*60*60
const double _QuaterYEAR_in_s = _YEAR_in_s*0.25; //[s] 365*24*60*60
const double _1_divided_QuaterYEAR_in_s = 1.0/ _QuaterYEAR_in_s; //[s] 365*24*60*60
const double _1_divided_YEAR_in_s = 1.0/ _YEAR_in_s;
const double _Month_in_s = 30 * 24 * 60 * 60;
const double _1_divided_Month_in_s = 1.0/ _Month_in_s;
const double _YEAR_in_d = 8544; //[s] 365*24
const double _DAY_in_s = 86400;//[s] 24*60*60
const double _1_divided_DAY_in_s = 1/ _DAY_in_s;
const double _Hour_in_s = 3600;//[s] 60*60
const double _1_divided_Hour_in_s = 1 / _Hour_in_s;
const double _Minute_in_s = 60;//[s] 60
const double _1_divided_Minute_in_s = 1 / _Minute_in_s;

//nach dieser Zeit soll die Tempertur nur noch duch sollgrößen, nicht mehr von ist größen abhängen
//const double _time_dependency_for_temp_seconds_ = 60 * 60 * 24 * 7;//1 Wochen
const double _time_dependency_for_temp_seconds_ = 60 * 60 * 24 *7;//1 Tag Je größer, desto mehr hängt die Temp von der Season ab
const double _1_divided_time_dependency_for_temp_seconds_ = 1/ _time_dependency_for_temp_seconds_;

//Physikalische Eigenschaften der Welt
const double _water_freez_temp[2] = { -0.2, -1.9 };
const double _tempDropPer1m = -0.003;
const double _beginTempDrop = 0; //m erst ab x meter soll es kontinuierlich kälter werden

const double _ice_tau_per_degree_latitude = 1.0;//[1/(s*°lat)]
const double _ice_tau_thickness_factor = 1000000.0;//[1/(s*m)]
const double _ice_tau_per_grad_celsius_factor = 0.1;//[1/°C]
const double _ice_temperature_gradient = 0.1;
const double _ice_melt_temp_expo = 5;
const double _ice_freeze_temp_expo = 5;

const double _plants_stop_growth_temp = 4.0;


//Definition für Arraygröße
const int _amountRegions = 11;
const int _amountTemperateZones = 4;
const int _amountSeasons = 4;



//Tempersturberechnung für DeltaWorld müssen zusammen 1 ergeben
const double _multiplier_neigbours = 0.45;
const double _multiplier_temperateZone = 0.5;
const double _ist_temp = 0.05;


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
* @brief Resourcengeneration: zwischen f(t = 0) = 0, und f(t = +-Tau) = +-max
* @param[in] max: Maximaler Wert für resource
* @param[in] Tau: Nach Tau sekunden ist die resource voll. Tau darf nicht INF sein!
* @param[in] t: Zeit, die Vergangen ist.
* @retval double: resourcen nach t secunden
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
* @brief Berechnet, wie viel Zeit bereits vergangen ist bei einer ist-resource: zwischen f(resources = 0) = 0, und f(resources = max) = Tau
* @param[in] max: Maximaler Wert für Resource.
* @param[in] Tau: Nach Tau sekunden ist die resource voll. Tau darf nicht INF sein!
* @param[in] resources: Ist zustand der resource.
* @retval double: Zeit, die vergangen ist, damit der ist resourcen wert generiert werden konnte
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
* @function one_divid_Tau_depend_Temp(double temp, double bestGrowth)
* @brief Berechnet, die inverse der Regenerationsrate Tau abhängig von der Temperatur und der Temp, bei der Pflanzen am besten wachsen
* @param[in] temp: Ist-Temperatur.
* @param[in] bestGrowth: Temperatur und der Temp, bei der Pflanzen am besten wachsen.
* @retval double: Inverse von Tau, der Regenerationsrate.
**/
inline double one_divid_Tau_depend_Temp(double temp, double bestGrowthTemp) {
	if (temp > _plants_stop_growth_temp) {
		double high_temp_growthBorder = bestGrowthTemp*2.0 - _plants_stop_growth_temp;
		if (temp < high_temp_growthBorder) {
			return (10.0 / _QuaterYEAR_in_s)*(0.5*std::sin(_PI * temp / (bestGrowthTemp - _plants_stop_growth_temp) + _plants_stop_growth_temp) + 0.5);//todo factor
		}
		else {
			return (-1.0 / _QuaterYEAR_in_s)*(high_temp_growthBorder - temp)*(high_temp_growthBorder - temp);
		}
	}
	else if(temp < 0.0){
		return (-1.0 / _QuaterYEAR_in_s ) * temp * temp ;// Resource nimmt mit der Temperatur quadratisch ab.
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

	int THREADS;
	unsigned int logicalCores;
	unsigned int Cores;

private:
	// Private Constructor
	GlobalSingleton() {
		this->_DeltaTime = _YEAR_in_s;
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
			this->_DELTA_SEASON_CHANGE_GRAD = (360.0  * _1_divided_YEAR_in_s) * this->_DeltaTime;
			this->_DELTA_SEASON_CHANGE_RAD = Deg2Rad(this->_DELTA_SEASON_CHANGE_GRAD);
		}
	}
};


#endif // !_GLOBALS_

