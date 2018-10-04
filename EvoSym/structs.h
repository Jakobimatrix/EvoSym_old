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
	double atp = 0; //[mol] primary energy source
	double sugar = 0; //[mol] secondary energy source
	double fat = 0; //[mol] tertiary energy source
	double water = 0; //[l] 
};

////////////////////////
// GLOBAL VARIABLES
////////////////////////
/*Credits for calculation X to ATP: Adrian Pfleger. */
constexpr double _SUGAR_TO_ATP = 32; //[mol] how many ATP from one sugar-molecule
constexpr double _FAT_TO_ATP = 122; //[mol] how many ATP from one fat-molecule
constexpr double _FAT_TO_ATP_WATER_LOS = 18;//[mol] Conversation from 1 mol fat to 122 mol ATP needs 18 mol water
constexpr double _LITER_2_MOL_H2O = 55.5;//[mol/l] => two hydrogen and one oxygen atom: atomic mass: 2*1.008 + 15.999 = 18.015. A litre is 1000 gm of water so 1000/18.015 = 55.5
constexpr double _MOL_2_LITER_H20 = 1.0 / _LITER_2_MOL_H2O;
constexpr double _SUGAR_TO_FAT = 1.0/4.5; // [mol] 4.5 mol sugar to 1 mol fat
constexpr double _SUGAR_TO_FAT_ATP_LOSS = 29; // [mol] how many FAT from one sugar-molecule
constexpr double _MOL_ATP_TO_ENERGY = 42300; //[Jule] Wikipedia:  Hydrolyse des abgespalteten Phosphats unter Standardbedingungen jeweils 32,3 kJ/mol (Spaltung einer Bindung) oder 64,6 kJ/mol (Spaltung beider Bindungen) Energie für Arbeitsleistungen in den Zellen frei.
constexpr double _ENERGY_TO_MOL_ATP = 1.0/_MOL_ATP_TO_ENERGY; //[mol atp]  
constexpr double _MOl_SUGAR_TO_KG = 0.180156; //[Kg sugar] C6H12O6 6*15.999 + 12*1.008 + 6*12.0107 = x: 1000/x = 5.5525
constexpr double _KG_SUGAR_TO_MOL = 1.0 / _MOl_SUGAR_TO_KG;
constexpr double _MOl_FAT_TO_KG = 0.3817; //[Kg fat] // 1 glycerin (6*O + 18*H) und 3 Fettsäuren: je(19 C und 18*2+ 3 H). Atomic masses: 6*15.999 + 18*1.008 + (18*2+3)*1.008 + 19*12.0107 = 381.6533; 1kg = 1000g -> 1000/381.6533 = 2.6202Kg/mol
constexpr double _KG_FAT_TO_MOL = 1.0 / _MOl_FAT_TO_KG;

struct AnimalEnergyStorage {
	EnergyStorage is;
	EnergyStorage max; //maybe fet not max -> weight gets more untill animal cant move
	void refillATP() {
		double refill = max.atp - is.atp;
		double needed_sugar = refill / _SUGAR_TO_ATP;
		if (needed_sugar < is.sugar) {//enough sugar stored to fill up ATP
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
				is.water -= needed_fat*_MOL_2_LITER_H20; //converting fat to atp needs water
			}
			else {//fatreserve empty
				is.atp = _FAT_TO_ATP*is.fat;
				is.water -= is.fat*_MOL_2_LITER_H20; //converting fat to atp needs water
				is.fat = 0;
			}
		}
	}

	//sugar in kg, fat in kg
	void store_food(double sugar, double fat) {
		is.sugar += sugar * _KG_SUGAR_TO_MOL;
		double sugar_surplus = 0;
		if (is.sugar > max.sugar) { //sugar reserve is full
			sugar_surplus = is.sugar - max.sugar;
			is.sugar = max.sugar;
		}
		is.fat += fat + sugar_surplus*_SUGAR_TO_FAT; // convert rest (surplus sugar) to fat
		is.atp -= sugar_surplus * _SUGAR_TO_FAT_ATP_LOSS;
		if (is.fat > max.fat) {
			is.fat = max.fat;
		}
	}
	//in water in liter
	void store_water(double water) {
		this->is.water += water;
		if (this->is.water > this->max.water) {
			is.water = max.water;
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

