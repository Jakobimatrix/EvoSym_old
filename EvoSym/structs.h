#ifndef _STRUCTS_
#define _STRUCTS_

#include <Windows.h> //WORD BYTE etc
#include <array>
#include "functions.h"
#include "Point2d.h"
#include "Skills.h"

struct MinMax
{
	double min;
	double max;
	MinMax(double min = 0.0, double max = 0.0)
	{
		if(min>max)
		{
			this->min = max;
			this->max = min;
		}else
		{
			this->min = min;
			this->max = max;
		}
	}
	double getSpan()
	{
		return this->max - this->min;
	}
	bool isWithin(double value) {
		if (value < this->min || this->max < value) {
			return false;
		}
		return true;
	}
};

template <typename T>
struct Season
{
	T spring;
	T summer;
	T autumn;
	T winter;

	Season(T spring = 0, T summer = 0, T autumn = 0, T winter = 0)
	{
		this->spring = spring;
		this->summer = summer;
		this->autumn = autumn;
		this->winter = winter;
	}
	T getValue(int season) {
		switch (season)
		{
		case 0:
			return spring;
			break;
		case 1:
			return summer;
			break;
		case 2:
			return autumn;
			break;
		case 3:
			return winter;
			break;
		default:
			return spring;
		}
	}
};

////////////////////////
// GLOBAL VARIABLES
////////////////////////

const int _MAX_CODE_DEPTH = 5;
const int _SIGNIFICANCE[5] = {5,4,3,2,1};

struct Code{
	Code() {}
	Code(const uint16_t (&_template)[_MAX_CODE_DEPTH]) {
		for (int i = 0; i < _MAX_CODE_DEPTH; i++) {
			this->part[i] = _template[i];
		}
	}

	uint16_t part[_MAX_CODE_DEPTH];//A code consists of 5 Parts. -> A WORD has 16 BIT, ->5*2^6 different codes. That should be enough to distinguish sound/color/smell/poison
	/*part 1 contains the most significant Information, part 5 the least significant information. See _SIGNIFICANCE*/
	double GetEquality(Code* compareCode, int precision){
		int wrong = 0;
		double codelength = 0;
		int depth = smallerValue(_MAX_CODE_DEPTH, precision);
		for (int ii = 0; ii < depth; ii++) {
			codelength = codelength + 16 * _SIGNIFICANCE[ii];
			uint16_t wrongBits = part[ii] ^ compareCode->part[ii];//^ = EXOR
			wrong = wrong + countSetBits(wrongBits) * _SIGNIFICANCE[ii];			
		}
		if (codelength > 0) {
			return wrong / codelength;
		}
		return 0;
	}
	void makeRandom() {
		for (int i = 0; i < _MAX_CODE_DEPTH; i++) {
			this->part[i] = (short)uniform_int_dist(0, (int)std::numeric_limits<short>::max);			
		}
	}
	Code Meiosis(Code* mother, Code* father) {
		Code child;
		for (int i = 0; i < _MAX_CODE_DEPTH; i++) {
			if (randTrueFalse()) {
				child.part[i] = mother->part[i];
			}
			else {
				child.part[i] = father->part[i];
			}
		}
	}
};

struct Gene {
	std::array<Code, 6> genome;//color,scent,development,bodyProportions,stats,senses
	Code& getColor() {
		return genome[0];//visual difference
	}
	Code& getScent() {
		return genome[1];
	}
	Code& getDevelopment() {
		return genome[2]; //how the animal will evolve; when its adult, when its grow up, when it dies
	}
	Code& getBbodyProportions() {
		return genome[3];//weight/energy reservoir/size
	}
	Code& getStats() {
		return genome[4];//ofense,defense,topspeed
	}
	Code& getSenses() {
		return genome[5];//hearing/smelling/vision/taste
	}
	void makeRandom() {
		for (int g = 0; g < 6; g++) {
			genome[g].makeRandom();
		}
	}
};

struct Cry{
	//https://www.hug-technik.com/inhalt/ta/schallpegel_laermpegel.html
	double intensity_dB;
	/*
	10 dB(A)	Blätterrascheln
	30 dB(A)	Flüstern
	75 dB(A)	PKW
	100 dB(A)	Kreissäge, Discothek
	114 dB(A)   Löwe
	140 dB(A)	Flugzeugstart
	160 dB(A)	Geschützknall
	*/
	Code code;
	double age;
	Point2d origen;
	Cry(Point2d* origen, double intensity, Code code = Code()) {
		this->code = code;
		this->intensity_dB = intensity;
		this->origen = *origen;
	}
	double getDurability_s() {
		return 1;
	}
	double speed(double temp) {
		return  331.5 * sqrt(1 + (temp / 273.15));//nach http://www.sengpielaudio.com/Rechner-schallgeschw.htm
	}
	double CalcIntensity_dB(double distace) {//http://www.schweizer-fn.de/akustik/schallpegelaenderung/schallpegel.php !16.61 -> 27.5 seems better //Lion: 114 dB up to 8Km -> weight ~ 27.5 (assumed Human can detect 7 dB)
		return this->intensity_dB - 27.5 * log10(distace);
	}
	double CalcIntensity_dB(Point2d* listenerPosition) {
		return this->intensity_dB - 27.5 * log10(Point2d::PointToPointDistance(listenerPosition,&this->origen));
	}
	double GetMaxRadius(double maxSensibleHearing_dB) {
		return pow(10, (this->intensity_dB - maxSensibleHearing_dB) / 27.5); //Lion: 114 dB up to 8Km -> weight ~ 27.5 (assumed Human can detect 7 dB)
	}
};

struct Propability
{
	double mean_min;
	double variance_max;
	int distribution; //0=Gaussian,1=standard dist.
	Propability(double mean_min = 0, double variance_max = 1, int distribution = 0)
	{
		this->distribution = distribution;
		switch (distribution)
		{
		case 0:
			this->mean_min = mean_min;
			this->variance_max = variance_max;
			break;
		case 1:
			if (mean_min > variance_max)
			{
				this->mean_min = variance_max;
				this->variance_max = mean_min;
			}else
			{
				this->mean_min = mean_min;
				this->variance_max = variance_max;
			}
			break;
		default:
			this->mean_min = mean_min;
			this->variance_max = variance_max;
			break;
		}
	}
	double getRand()
	{
		switch (distribution)
		{
		case 0:
			return randGaus(this->mean_min, this->variance_max);
		case 1:
			return uniform_double_dist(this->mean_min, this->variance_max);
		default:
			return randGaus(this->mean_min, this->variance_max);
		}
	}
};

struct EnergyStorage {
	double atp = 0; //[Kg] primary energy source
	double sugar = 0; //[Kg] secondary energy source
	double fat = 0; //[Kg] tertiary energy source
	double water = 0; //[Kg] 
};

////////////////////////
// GLOBAL VARIABLES
////////////////////////
constexpr double _SUGAR_TO_ATP = 12; //how many ATP from one sugar-molecule
constexpr double _FAT_TO_ATP = 1000; //how many ATP from one fat-molecule
constexpr double _LOSS_SUGAR_FAT_CONVERSATION = 0.5; //loss in percentage, if sugar gets convertead to fat and than to atp instead directly to atp
constexpr double _SUGAR_TO_FAT = _SUGAR_TO_ATP / _FAT_TO_ATP * _LOSS_SUGAR_FAT_CONVERSATION; //how many FAT from one sugar-molecule
constexpr double _ATP_TO_ENERGY = 470; //Hydrolysis of one gram mole of ATP releases about 470 kJ of useful energy

struct AnimalEnergyStorage {
	EnergyStorage is;
	EnergyStorage max;
	void refillATP() {
		double refill = max.atp - is.atp;
		double needed_sugar = refill / _SUGAR_TO_ATP;
		if (needed_sugar < is.sugar) {//enough sugar stored
			is.atp = max.atp;
			is.sugar -= needed_sugar;
		}
		else {//not enough sugar stored use fat reserve
			is.atp = _SUGAR_TO_ATP*is.sugar;
			is.sugar = 0;
			refill = max.atp - is.atp;
			double needed_fat = refill / _FAT_TO_ATP;
			if (needed_fat < is.fat) {
				is.atp = max.atp;
				is.fat -= needed_fat;
			}
			else {//fatreserve empty
				is.atp = _FAT_TO_ATP*is.fat;
				is.fat = 0;
			}
		}
	}

	void store(double sugar, double fat) {
		is.sugar += sugar;
		double sugar_surplus = 0;
		if (is.sugar > max.sugar) {
			sugar_surplus = is.sugar - max.sugar;
			is.sugar = max.sugar;
		}
		is.fat += fat + sugar_surplus*_SUGAR_TO_FAT;
		if (is.fat > max.fat) {
			is.fat = max.fat;
		}
	}
};

struct AnimalSenses {
	double hearing	= 0; //[dB]
	double seight	= 0; //[m]
	double taste	= 0; //[level]
	double smell	= 0; //[dB]
};


struct AnimalSkin {
	double feather		= 0;//[0-1]
	double fur			= 0;//[0-1]
	double scale		= 0;//[0-1]
	double naked_skin	= 0;//[0-1]
					 //all in sum == 1!
};

struct AnimalBuild {
	double size		= 0.1;	//[m^3] volume
	double density	= 1;	//[kg/m^3]
	double strength = 0;	//[Kg*m/s^2]
	double defense	= 0;	//[multiplier]
	double offense	= 0;	//[multiplier]
	double metabolism		= 300;//heartbeats per minute
	double eupeptic_level	= 0;
	bool sex = true;	//female = true, male = false;
};

struct AnimalProperties {
	AnimalSkin skin;
	AnimalSenses senses;
	AnimalEnergyStorage energie_storage;
	AnimalBuild build;
	double last_update = 0;
	double optimal_temperature = 20;
	MinMax possible_temperature = MinMax(-10,30);

	bool can_swim = false;
	bool can_climb = false;
	bool can_fly = false;

	bool canSwimm(double time) {
		//todo 
		return true;
	}
	bool canClimb(double time) {
		//todo 
		return true;
	}
	bool canFly(double time) {
		//todo
		return true;
	}
};
#endif // !_STRUCTS_

