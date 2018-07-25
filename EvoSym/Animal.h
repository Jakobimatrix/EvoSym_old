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
#include <math.h>
#include <iostream>
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
	DeltaWorld* current_delta_world;

	/*
	different NNs:
	1 DecisionNetwork: makes decision
	2-n ActionNetworks: decide (for the choosen decision) what exactly to do. These might not need all the input from the DecisionNetwork
	*/
	Eigen::Matrix<double, _AMOUNT_ALL_INPUT_PARAMS_NN, 1> neurones_input;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, 1> neurone_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, 1> neurone_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, 1> neurones_output;

	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, _AMOUNT_ALL_INPUT_PARAMS_NN> weights_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, _AMOUNT_HIDDEN_NEURONS_1_NN> weights_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, _AMOUNT_HIDDEN_NEURONS_2_NN> weights_output;

	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_1_NN, 1> bias_hidden_1;
	Eigen::Matrix<double, _AMOUNT_HIDDEN_NEURONS_2_NN, 1> bias_hidden_2;
	Eigen::Matrix<double, _AMOUNT_OUTPUT_NEURONES_NN, 1> bias_output;
	
	int num_input_initiated_neurons;
	//animal characteristiks
	

	double* size;		//m
	double* weight;		//Kg
	double* strength;	//kg bewegt werden kann
	double* health;		//points "healthbar"
	double* health_previous;		//"healthbar"
	double* ofense;		//points 
	double* defense;		//points 
	double* energy_is;	//J
	double* energy_is_previous;	//J
	double* water_is;	//L
	double* water_is_previous;	//L
	double* energy_max;	//J
	double* water_max;	//L

	bool sex;	//male = false, female = true
	bool has_reproduced;
	bool is_alive; 

						//factos to reduce calculationtime
	double weight_strength_factor;
	Gene genom;


	///initiation
	void init();
	void randinit();
	void isBorn(const Animal &mother, const  Animal &father);
	void isCloned(const Animal &mother);

	///update
	void UpdateCurrentDeltaWorld();
	void growth();
	void die();
	void decay();
	bool hasReproduced();
	void updateInputNeurons();


	///actions


	void doNothing();

	/**
	* @function travel(Point2d &travelvector)
	* @brief: Uses travelGround() or travelWater() depending on the current region. If transition to another region. current_region will be updated.
	* @param[in] Point2d &travelvector: Travel velocity (includes direction).
	**/
	void travel(Point2d &travelvector);
	void seachFood();
	void seachWater();
	void sleep();
	void seachHideout();

	/**
	* @function travelGround(Point2d &travelvector)
	* @brief:Updates the position depending on the travel vector. Calculates the new animal is conditions after the travel.
	* @param[in] Point2d &travelvector: Travel velocity (includes direction).
	**/
	void travelGround(Point2d &travelvector);

	/**
	* @function travelWater(Point2d &travelvector)
	* @brief:Updates the position depending on the travel vector. Calculates the new animal is conditions after the travel.
	* @param[in] Point2d &travelvector: Travel velocity (includes direction).
	**/
	void travelWater(Point2d &travelvector);
	void clone();


	void doAction(int action, double dt);

	void nextAction(double dt);

	double calcMaxPossibleSpeed();

public:
	Animal(World* world, Point2d& position) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		this->position = position;
		if (!world->getWorldPartByPosition(position, current_delta_world)) {
			die();
			return;
		}
		this->init();
		this->randinit();
	}
	Animal(Animal &mother, Animal &father, World* world) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		if (!world->getWorldPartByPosition(mother.position, current_delta_world)) {
			die();
			return;
		}
		this->init();
		this->isBorn(mother, father);
	}
	Animal(Animal &mother, World* world) {
		this->_G_ = &this->_G_->getInstance();
		this->world = world;
		if (!world->getWorldPartByPosition(mother.position, current_delta_world)) {
			die();
			return;
		}
		this->init();
		this->isCloned(mother);
	}
	~Animal() {}

	bool isAlive() {
		return this->is_alive;
	}

	void update(double tnow);
};
#endif
