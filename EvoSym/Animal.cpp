#include "Animal.h"

/*
//input neurones:
-----updated by Pointer-------
0	size
1	weight				
2	strength			
3	health				
4	health_previous		
5	ofense				
6	defense				
7	energy_is			
8	energy_is_previous	
9	water_is			
10	water_is_previous	
11	energy_max			
12	water_max	
13  _WORLD_DIAMETER_HALF World Radius
-----updated manually---------
14	position.x;
15	position.y;
16	dw->getLatitude();
17	dw->getGradient().x;
18	dw->getGradient().y;
29	dw->getHeight();
20	dw->getRegionId();
21	dw->getTemp();
22	(double)TrueFalseTo10(dw->GroundFrozen());
23	_G_->SeasonMultiplier[0];
24	_G_->SeasonMultiplier[1];
+ memory (old results of output neurones)

//output neurones;
----travel info, softmax independent from decisions
0	travel_x
1	travel_y
2   travel_slow
---------------
3	do_nothing_decision
4	travel_decision
5	seach_food_decision
6	seach_water_decision
7	sleep_decision
8	seach_hideout_decision
9	clone
//10	reproduce
*/

void Animal::init(){
	this->delta_t_update = _HOUR_IN_S;
	this->_G_->announceDeltaTime(this->delta_t_update);

	int i = 0;
	size				= &neurones_input(i++, 0);		
	weight				= &neurones_input(i++, 0);
	strength			= &neurones_input(i++, 0);
	health				= &neurones_input(i++, 0);
	health_previous		= &neurones_input(i++, 0);
	ofense				= &neurones_input(i++, 0);
	defense				= &neurones_input(i++, 0);
	energy_is			= &neurones_input(i++, 0);
	energy_is_previous	= &neurones_input(i++, 0);
	water_is			= &neurones_input(i++, 0);
	water_is_previous	= &neurones_input(i++, 0);
	energy_max			= &neurones_input(i++, 0);
	water_max			= &neurones_input(i++, 0);
	neurones_input(i++, 0) = _WORLD_DIAMETER_HALF;
	num_input_initiated_neurons = i;


}
void Animal::randinit() {
	this->init();
	/*
	size = uniform_double_dist(0.0, 40.0);		//0-40m



	energy_max = uniform_double_dist(size*0.01, size * 100);	//J
	water_max = uniform_double_dist(size*0.01,size * 1.0);	//L

	energy_is = energy_max*0.5;	//J
	water_is = water_max*0.5;	//L

	double sizeEnergyWaterFactor = (energy_max + 10 * water_max) * size;

	weight = uniform_double_dist(sizeEnergyWaterFactor*0.01, sizeEnergyWaterFactor*100.0);		//Kg
	strength = uniform_double_dist(weight*0.1, weight*10.0);	//kg bewegt werden kann

	genom.makeRandom();

	this->growth();
	*/
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
	is_alive = false;
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


	//update ofense, defense

	//update factors
	//weight_strength_factor = 
	//	_energy_consumption_size_factor*std::pow(size, _energy_consumption_weight_exponent)
	//	+ _energy_consumption_weight_factor*std::pow(weight, _energy_consumption_weight_exponent);

}
//TODO
void Animal::decay() {//decayfactor depends on region
	//energy_is *= decayfactor;
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

void Animal::updateInputNeurons() {
	//animal stats are linked via pointer and always up to date
	int i = num_input_initiated_neurons + 1;
	neurones_input(i++, 0) = position.x;
	neurones_input(i++, 0) = position.y;
	neurones_input(i++, 0) = position.x - _WORLD_DIAMETER_HALF;//distance to edge
	neurones_input(i++, 0) = position.y - _WORLD_DIAMETER_HALF;//distance to edge
	neurones_input(i++, 0) = current_delta_world->getLatitude();
	neurones_input(i++, 0) = current_delta_world->getGradient().x;
	neurones_input(i++, 0) = current_delta_world->getGradient().y;
	neurones_input(i++, 0) = current_delta_world->getHeight();
	neurones_input(i++, 0) = current_delta_world->getRegionId();
	neurones_input(i++, 0) = current_delta_world->getTemp();
	neurones_input(i++, 0) = (double)TrueFalseTo10(current_delta_world->GroundFrozen());
	neurones_input(i++, 0) = _G_->SeasonMultiplier[0];
	neurones_input(i++, 0) = _G_->SeasonMultiplier[1];
	//memory oldest->i++->newest
	for (int mem_depth = _MEMORY_DEPTH_NN-1; mem_depth > -1; mem_depth--) {//begin overwriting the oldest memory
		for (int out_put_neuron = 0; out_put_neuron < _AMOUNT_OUTPUT_NEURONES_NN; out_put_neuron++) {
			if (mem_depth == 0) {//newest memory
				neurones_input(i++, 0) = neurones_output(out_put_neuron, 0);
			}
			else {
				neurones_input(i++, 0) = neurones_input(_AMOUNT_OUTPUT_NEURONES_NN + i++, 0);//overwrite the older memory with newer
			}
		}
	}


}
void Animal::nextAction(double dt) {
	if (!is_alive) {
		decay();
		return;
	}
	updateInputNeurons();
	neurone_hidden_1 = weights_hidden_1*neurones_input + bias_hidden_1;
	neurone_hidden_1.unaryExpr(&ReLU);

	neurone_hidden_2 = weights_hidden_2*neurone_hidden_1 + bias_hidden_2;
	neurone_hidden_2.unaryExpr(&ReLU);

	neurones_output = weights_output*neurone_hidden_2 + bias_output;

	for (int i = 3; i < _AMOUNT_OUTPUT_NEURONES_NN; i++) { //output 0,1,2 is for traveling info! no ReLU
		neurones_output(i) = ReLU(neurones_output(i)); 
	}
	int decision = 0;
	double highst_value = 0;
	for (int i = 3; i < _AMOUNT_OUTPUT_NEURONES_NN; i++) {
		if (neurones_output(i) > highst_value) {
			int decision = i;
			highst_value = neurones_output(i);
		}
	}

	switch (decision)
	{
	case 3:
		doNothing();
		break;
	case 4:
	{
		double summ = 0.0;
		for (int i = 3; i < _AMOUNT_OUTPUT_NEURONES_NN; i++) {
			summ += neurones_output(i);
		}
		if (summ = 0.0) {
			doNothing();
		}
		else {
			double max_speed = calcMaxPossibleSpeed();
			travel(Point2d(neurones_output(0) * max_speed / summ, neurones_output(1) * max_speed / summ));
		}
		break;
	}
	case 5:
		seachFood();
		break;
	case 6:
		seachWater();
		break;
	case 7:
		sleep();
		break;
	case 8:
		seachHideout();
		break;
	case 9:
		clone();
		break;
	default:
		doNothing();
		break;
	}	
}

void Animal::travel(Point2d& travelvector) {
	
	Point2d target_position = this->position + travelvector;
	
	DeltaWorld* target_delta_world; //new point in new DeltaWorld? Assuming the animal wont be able to travel over 2 borders in one simulation step.
	if (!world->getWorldPartByPosition(target_position, target_delta_world)) {
		die();//fall over the border of this world
		return;
	}
	int current_regionId = current_delta_world->getRegionId();
	if (target_delta_world == current_delta_world) { //same delta World
		if (current_delta_world->getRegionId() == 0 || current_delta_world->getRegionId() == 1) {
			travelWater(travelvector);
			return;
		}
		else {
			travelGround(travelvector);
			return;
		}
	}
	else {//target region is a different region
		//travel to the border of the target delta world		
		Point2d borderPoint;
		if (current_delta_world->getBorderEntryPoint(borderPoint, position, target_position)) {
			if (current_delta_world->getRegionId() == 0 || current_delta_world->getRegionId() == 1) {
				travelWater(borderPoint - position);
				return;
			}
			else {
				travelGround(borderPoint - position);
				return;
			}
			if (is_alive) {
				current_delta_world = target_delta_world;
				if (target_delta_world->getRegionId() == 0 || target_delta_world->getRegionId() == 1) {					
					travelWater(target_position - position);
					return;
				}
				else {
					travelGround(target_position - position);
					return;
				}
			}
		}
		std::cout << "Warning: Animal.cpp: Animal travels too another region, but border intersection couldnt be calculated!\n";
		std::getchar();
	}
}
void Animal::travelGround(Point2d& travelvector) {
	
	energy_is ;
	water_is;
	*energy_is_previous = *energy_is;
	*water_is_previous = *water_is;
	this->position = this->position + travelvector;
}

void Animal::travelWater(Point2d& travelvector) {
	//todo: check if animal can swimm
	this->position = this->position + travelvector;
}

double Animal::calcMaxPossibleSpeed() {
	return 2;
}

void Animal::clone() {

}
void Animal::doNothing() {

}

void Animal::seachFood(){}
void Animal::seachWater(){}
void Animal::sleep(){}
void Animal::seachHideout(){}



bool Animal::hasReproduced() {
	return has_reproduced;
}


void Animal::update(double tnow) {
	double dt = (tnow - this->t_last_update);
	this->nextAction(dt);
	this->growth();
}

void Animal::UpdateCurrentDeltaWorld() {
	if (!world->getWorldPartByPosition(position, current_delta_world)) {
		die();
		return;
	}
}
