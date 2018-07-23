#include "Animal.h"

/*
//input neurones:
0
1
2
3
4
5
6
7
8
9
10

//output neurones;
0	travel_x
1	travel_x_speed
2	travel_y
3	travel_y_speed

*/

void Animal::init(){
	this->delta_t_update = _HOUR_IN_S;
	//this->deltaT_Update = _Minute_in_s;
	this->animal_characteristics.evolution.points = 0;
	this->animal_characteristics.is_condition.is_alive = true;
	this->animal_characteristics.is_condition.ofense = 0;
	this->animal_characteristics.is_condition.defense = 0;
}
void Animal::randinit() {
	this->init();

	this->animal_characteristics.is_condition.size = uniform_double_dist(0.0, 40.0);		//0-40m

	this->animal_characteristics.is_condition.hearing = uniform_double_dist(1.0, 100.0);	//min dB recognizable
	this->animal_characteristics.is_condition.vision = uniform_double_dist(this->animal_characteristics.is_condition.size*1000.0, this->animal_characteristics.is_condition.size*100000.0);		//m max distance

	this->animal_characteristics.is_condition.energy_max = uniform_double_dist(this->animal_characteristics.is_condition.size*0.01, this->animal_characteristics.is_condition.size * 100);	//J
	this->animal_characteristics.is_condition.water_max = uniform_double_dist(this->animal_characteristics.is_condition.size*0.01, this->animal_characteristics.is_condition.size * 1.0);	//L

	this->animal_characteristics.is_condition.energy_is = this->animal_characteristics.is_condition.energy_max*0.5;	//J
	this->animal_characteristics.is_condition.water_is = this->animal_characteristics.is_condition.water_max*0.5;	//L

	double sizeEnergyWaterFactor = (this->animal_characteristics.is_condition.energy_max + 10 * this->animal_characteristics.is_condition.water_max) * this->animal_characteristics.is_condition.size;

	this->animal_characteristics.is_condition.weight = uniform_double_dist(sizeEnergyWaterFactor*0.01, sizeEnergyWaterFactor*100.0);		//Kg
	this->animal_characteristics.is_condition.strength = uniform_double_dist(this->animal_characteristics.is_condition.weight*0.1, this->animal_characteristics.is_condition.weight*10.0);	//kg bewegt werden kann

	this->animal_characteristics.genom.makeRandom();

	this->growth();
}
//TODO
void Animal::isBorn(const Animal &mother, const  Animal &father) {
	this->randinit();
	return;
	//todo
	this->init();

	this->growth();

}
//TODO
void Animal::isCloned(const Animal &mother) {
	this->randinit();
	return;
	//todo;
	this->init();

	this->growth();
}
//TODO
void Animal::die() {
	//this->animal_characteristics.gene.scent = Code(_scent_decay);//todo maybe slowly shift the scent into "decay scent"->do this within this->decay()
	this->animal_characteristics.is_condition.is_alive = false;
	this->delta_t_update = _HOUR_IN_S;// a dead animal dont need to be updated every deltaTime
}
//TODO
void Animal::growth() {
	//is_alive

	//update bodycharacteristics
	//size

	//maxEnergy

	//maxWater

	//weight

	//strength

	//hering

	//vision

	//update ofense, defense

	//update factors
	this->animal_characteristics.is_condition.weight_strength_factor = 
		_energy_consumption_size_factor*std::pow(this->animal_characteristics.is_condition.size, _energy_consumption_weight_exponent)
		+ _energy_consumption_weight_factor*std::pow(this->animal_characteristics.is_condition.weight, _energy_consumption_weight_exponent);

}
//TODO
void Animal::decay(double decayfactor) {//decayfactor depends on region
	this->animal_characteristics.is_condition.energy_is *= decayfactor;
	//den scent of the animal slowly into the scent of death 
	/*for (int i = _MAX_CODE_DEPTH-1; i > -1; i--) {
	if (this->animal_characteristics.gene.scent.part[i] != _scent_decay) {
	for (int b = 15; b < 1; b--) {
	<< b
	}
	return;
	}
	}*/
}

int Animal::nextAction() {
	neurone_hidden_1 = weights_hidden_1*neurones_input + bias_hidden_1;
	neurone_hidden_1.unaryExpr(&ReLU);

	neurone_hidden_2 = weights_hidden_2*neurone_hidden_1 + bias_hidden_2;
	neurone_hidden_2.unaryExpr(&ReLU);

	neurones_output = weights_output*neurone_hidden_2 + bias_output;

	//strongest neuron wins
	double next_action = 0;
	double next_action_value = 0;
	for (int i = 0; i < _AMOUNT_OUTPUT_NEURONES_NN; i++) {
		if (next_action_value < neurones_output(i)) {
			next_action = i;
			next_action_value = neurones_output(i);
		}
	}
	return next_action;
}
//TODO
double Animal::travelGround(Point2d travelvector, double speed, double heightDiff) {
	double way = travelvector.getR() * speed*speed;//what energy-way we have
	if (heightDiff > 0) {
		way += heightDiff;//we go up 
	}
	else {
		way -= heightDiff*0.1;//we go down->not as much energy is needed to go down than to go up
	}
	if (way > 0) {
		//todo energyconsumption
	}
	this->position = this->position + travelvector;

	return 2.0;//TODO energyconsumption
}
//TODO
double Animal::travelWater() {
	return 2.0;
}
double Animal::climb() {
	return 2.0;
}
double Animal::eat() {
	return 2.0;
}
double Animal::drink() {
	return 2.0;
}
double Animal::clone() {
	return 2.0;
}


bool Animal::hasReproduced() {
	return this->animal_characteristics.is_condition.has_reproduced;
}


void Animal::update(double tnow) {
	double dt = (tnow - this->t_last_update);
	doAction(this->nextAction(), dt);
	this->growth();
}

void Animal::doAction(int action, double dt) {
	switch (action)
	{
	case 0:

	default:
		break;
	}
}