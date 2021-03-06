﻿#ifndef _STRUCTS_
#define _STRUCTS_

#include <Windows.h> //WORD BYTE etc
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
	Code(const uint16_t (&_template)[5]) {
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
#endif // !_STRUCTS_

