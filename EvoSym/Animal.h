/**
* @file Animal.h
* @brief This inherits from SimulatedUnit.
* @date 10.11.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _ANIMAL_
#define _ANIMAL_
#include "globals.h"
#include "structs.h"
#include "SimulatedUnit.h"
#include "Point2d.h"
#include "Skills.h"
#include "DeltaWorld.h"


class Animal :public SimulatedUnit {
public:
	Animal() {
		this->randinit();
	}
	Animal(const Animal &mother, const Animal &father) {
		this->isBorn(mother, father);
	}
	Animal(const Animal &mother) {
		this->isCloned(mother);
	}
	~Animal(){}
private:

	struct{
		bool sex;			//male = false, female = true
		struct {
			Code color;		//visual difference
			//Code scent;		//
			//Code development;//how the animal will evolve; when its adult, when its grow up, when it dies
			//Code bodyProportions;		//weight/energy reservoir/size
			//Code stats;		//ofense,defense,topspeed
			//Code senses;	//hearing/smelling/vision/taste
		}Gene;

		struct {
			bool isAlive;
			double size;		//m
			double weight;		//Kg
			double strength;	//kg bewegt werden kann
			double ofense;		//points 
			double defense;		//points 
			double hearing;		//min dB recognizable //je kleiner desto besser > 0
			double vision;		//m max distance
			double energy_is;	//J
			double water_is;	//L
			double energy_max;	//J
			double water_max;	//L

			//factos to reduce calculationtime
			double weightStrengthFactor;

		}isCondition;	

		struct {
			DeltaWorld CurrentEnvironment;
		}localWorld;

		struct {
			double points;
			
		}evolution;

	}AnimalCharacteristics;

	///initiation
	void init();
	void randinit();
	void isBorn(const Animal &mother, const  Animal &father);
	void isCloned(const Animal &mother);

	///update
	void growth();
	void die();
	void decay(double decayfactor);


	///actions

	// \brief travel(Point2d travelvector, double speed, double heightDiff);
	// moves the animal according to the given travelvector
	//[Point2d travelvector]	vector to the next location
	//[speed] speed =! length of travelvector due to simullated time difference
	//[heightDiff] difference in height between 2 deltaWords
	double travelGround(Point2d travelvector, double speed, double heightDiff);
	double travelWater();
	double climb();
	double eat();
	double drink();
	double clone();
	double sex();

	void nextAction();

public:
	bool isAlive() {
		return this->AnimalCharacteristics.isCondition.isAlive;
	}

	void setEnvironment();

	void update(double tnow) {
		this->nextAction();
		this->growth();
	}
};
#endif
