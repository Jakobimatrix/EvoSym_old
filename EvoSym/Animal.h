/**
* @file Animal.h
* @brief: This inherits from SimulatedUnit.
* @date 10.11.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _ANIMAL_
#define _ANIMAL_
#include <Eigen\Dense>
#include "globals.h"
#include "structs.h"
#include "SimulatedUnit.h"
#include "Point2d.h"
#include "Skills.h"
#include "World.h"
#include "DeltaWorld.h"


class Animal :public SimulatedUnit {

private:
	GlobalSingleton* _G_;
	World* world;

	Eigen::Matrix<double, _AMOUNT_INPUT_PARAMS_NN, 1> neurones_input;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, 1> neurone_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, 1> neurone_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, 1> neurones_output;

	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, _AMOUNT_INPUT_PARAMS_NN> weights_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, _AMOUNT_HIDDEN_NEURONS_1_NN> weights_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, _AMOUNT_HIDDEN_NEURONS_2_NN> weights_output;

	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, 1> bias_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, 1> bias_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, 1> bias_output;
	
	//animal characteristiks
	
	double sex;	//male = false, female = true
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
	bool has_reproduced;
	bool is_alive; //"healtbar"

						//factos to reduce calculationtime
	double weight_strength_factor;
	Gene genom;


	///initiation
	void init();
	void randinit();
	void isBorn(const Animal &mother, const  Animal &father);
	void isCloned(const Animal &mother);

	///update
	void growth();
	void die();
	void decay(double decayfactor);
	bool hasReproduced();


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

	void doAction(int action, double dt);

	int nextAction();

public:
	Animal(World* world) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		this->randinit();
	}
	Animal(const Animal &mother, const Animal &father, World* world) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		this->isBorn(mother, father);
	}
	Animal(const Animal &mother, World* world) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		this->isCloned(mother);
	}
	~Animal() {}

	bool isAlive() {
		return this->is_alive;
	}

	void update(double tnow);
};
#endif
