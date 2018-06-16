#include "Animal.h"


void Animal::init(){
	this->deltaT_Update = _Hour_in_s;
	//this->deltaT_Update = _Minute_in_s;
	this->AnimalCharacteristics.evolution.points = 0;
	this->AnimalCharacteristics.isCondition.isAlive = true;
	this->AnimalCharacteristics.isCondition.ofense = 0;
	this->AnimalCharacteristics.isCondition.defense = 0;
}
void Animal::randinit() {
	this->init();

	this->AnimalCharacteristics.isCondition.size = uniform_double_dist(0.0, 40.0);		//0-40m

	this->AnimalCharacteristics.isCondition.hearing = uniform_double_dist(1.0, 100.0);	//min dB recognizable
	this->AnimalCharacteristics.isCondition.vision = uniform_double_dist(this->AnimalCharacteristics.isCondition.size*1000.0, this->AnimalCharacteristics.isCondition.size*100000.0);		//m max distance

	this->AnimalCharacteristics.isCondition.energy_max = uniform_double_dist(this->AnimalCharacteristics.isCondition.size*0.01, this->AnimalCharacteristics.isCondition.size * 100);	//J
	this->AnimalCharacteristics.isCondition.water_max = uniform_double_dist(this->AnimalCharacteristics.isCondition.size*0.01, this->AnimalCharacteristics.isCondition.size * 1.0);	//L

	this->AnimalCharacteristics.isCondition.energy_is = this->AnimalCharacteristics.isCondition.energy_max*0.5;	//J
	this->AnimalCharacteristics.isCondition.water_is = this->AnimalCharacteristics.isCondition.water_max*0.5;	//L

	double sizeEnergyWaterFactor = (this->AnimalCharacteristics.isCondition.energy_max + 10 * this->AnimalCharacteristics.isCondition.water_max) * this->AnimalCharacteristics.isCondition.size;

	this->AnimalCharacteristics.isCondition.weight = uniform_double_dist(sizeEnergyWaterFactor*0.01, sizeEnergyWaterFactor*100.0);		//Kg
	this->AnimalCharacteristics.isCondition.strength = uniform_double_dist(this->AnimalCharacteristics.isCondition.weight*0.1, this->AnimalCharacteristics.isCondition.weight*10.0);	//kg bewegt werden kann

	this->AnimalCharacteristics.Gene.color.makeRandom();

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
	//this->AnimalCharacteristics.Gene.scent = Code(_scent_decay);//todo maybe slowly shift the scent into "decay scent"->do this within this->decay()
	this->AnimalCharacteristics.isCondition.isAlive = false;
	this->deltaT_Update = _Hour_in_s;// a dead animal dont need to be updated every deltaTime
}
//TODO
void Animal::growth() {
	//isAlive

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
	this->AnimalCharacteristics.isCondition.weightStrengthFactor = _energy_consumption_size_factor*std::pow(this->AnimalCharacteristics.isCondition.size, _energy_consumption_weight_exponent) \
		+ \
		_energy_consumption_weight_factor*std::pow(this->AnimalCharacteristics.isCondition.weight, _energy_consumption_weight_exponent);

}
//TODO
void Animal::decay(double decayfactor) {//decayfactor depends on region
	this->AnimalCharacteristics.isCondition.energy_is *= decayfactor;
	//den scent of the animal slowly into the scent of death 
	/*for (int i = _MAX_CODE_DEPTH-1; i > -1; i--) {
	if (this->AnimalCharacteristics.Gene.scent.part[i] != _scent_decay) {
	for (int b = 15; b < 1; b--) {
	<< b
	}
	return;
	}
	}*/
}

//TODO
void Animal::nextAction() {
	////neuronal network decission
	//energy = action
	//energyconsumption
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
double Animal::sex() {
	return 2.0;
}
