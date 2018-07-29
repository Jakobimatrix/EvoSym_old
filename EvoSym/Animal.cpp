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
	//properties_max = ...;
	//properties_is = ...;
	int i = 0;

	neurones_input(i++, 0) = _WORLD_DIAMETER_HALF;
	neurones_input(i++, 0) = position.x; //place of birth
	neurones_input(i++, 0) = position.y; //place of birth
	num_input_static_neurons = i;


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

}
//TODO
void Animal::isCloned(const Animal &mother) {
	this->randinit();
	return;
	//todo;
	this->init();
}
//TODO
void Animal::die() {
	//this->animal_characteristics.gene.scent = Code(_scent_decay);//todo maybe slowly shift the scent into "decay scent"->do this within this->decay()
	is_alive = false;
	this->delta_t_update = _DAY_IN_S;// a dead animal dont need to be updated every deltaTime

	//reset time for decay process
	this->t0 = 0;
	this->t_last_update = 0;
}
//TODO
void Animal::live(double dt) {
	//calc if still alive
	//calc new is-properties using age
	//calc energy needen to live (depends on metabolism and is_temperature and is_sleeping, senses, etc )
	properties_is.energie_storage.refillATP();
	if (!properties_is.energie_storage.is.atp > 0 || !properties_is.energie_storage.is.water > 0) {
		die();
	}
}

void Animal::decay(double dt) {//decayfactor depends on region
	//energy_is *= decayfactor;
	//den scent of the animal slowly into the scent of death 
	if (current_delta_world->getTemp() > 0) {
		if (properties_is.energie_storage.is.sugar > 0) {
			properties_is.energie_storage.is.sugar -= current_delta_world->getTemp() * _DECAY_FACTOR_SUGAR; //todo depends on surface?
		}
		if (properties_is.energie_storage.is.fat > 0) {
			properties_is.energie_storage.is.fat -= current_delta_world->getTemp() * _DECAY_FACTOR_FAT; //todo depends on surface?
		}

		if (!properties_is.energie_storage.is.sugar > 0 && !properties_is.energie_storage.is.fat > 0) {
			is_zero = true;
		}
	}
}

void Animal::updateInputNeurons() {
	//animal stats are linked via pointer and always up to date
	int i = num_input_static_neurons + 1;

	neurones_input(i++, 0) = properties_is.energie_storage.is.atp;
	neurones_input(i++, 0) = properties_is.energie_storage.is.sugar;
	neurones_input(i++, 0) = properties_is.energie_storage.is.fat;
	neurones_input(i++, 0) = properties_is.energie_storage.is.water;
	neurones_input(i++, 0) = energie_difference.atp;
	neurones_input(i++, 0) = energie_difference.sugar;
	neurones_input(i++, 0) = energie_difference.fat;
	neurones_input(i++, 0) = energie_difference.water;

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
	if (!is_alive || !UpdateCurrentDeltaWorld()) {
		decay(dt);
		return;
	}

	is_sleeping = false;
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

	energie_difference.atp = properties_is.energie_storage.is.atp;
	energie_difference.water = properties_is.energie_storage.is.water;

	switch (decision)
	{
	case 3:

		break;
	case 4:
	{
		double summ = 0.0;
		for (int i = 3; i < _AMOUNT_OUTPUT_NEURONES_NN; i++) {
			summ += neurones_output(i);
		}
		if (summ != 0.0) {
			double max_speed = calcMaxPossibleSpeed(dt);
			travel(Point2d(neurones_output(0) * max_speed / summ, neurones_output(1) * max_speed / summ),dt);
		}
		break;
	}
	case 5:
		seachFood(dt);
		break;
	case 6:
		seachWater(dt);
		break;
	case 7:
		is_sleeping = true;
		break;
	case 8:
		seachHideout(dt);
		break;
	case 9:
		clone();
		break;
	}	
	energie_difference.atp = energie_difference.atp - properties_is.energie_storage.is.atp;//difference t_-1 - t_0
	energie_difference.water = energie_difference.water - properties_is.energie_storage.is.water;//difference t_-1 - t_0
}

void Animal::travel(Point2d& travelvector, double dt) {
	Point2d target_position = this->position + travelvector;
	
	DeltaWorld* target_delta_world; //new point in new DeltaWorld? Assuming the animal wont be able to travel over 2 borders in one simulation step.
	if (!world->getWorldPartByPosition(target_position, target_delta_world)) {
		die();//fall over the border of this world
		return;
	}
	if (target_delta_world == current_delta_world) { //same delta World
			calcTravel(travelvector, dt);	
	}

	else {//target region is a different region
		//travel to the border of the current delta world		
		Point2d borderPoint;
		if (current_delta_world->getBorderEntryPoint(borderPoint, position, target_position)) {
			Point2d first_part_travel_vector = borderPoint - position;
			first_part_travel_vector = first_part_travel_vector*1.1;//TODO hack that we will be in the next region for sure
			double dt_first_part = dt * (first_part_travel_vector.getR()/travelvector.getR());
			Point2d second_part_travel_vector = travelvector - first_part_travel_vector;
			calcTravel(first_part_travel_vector, dt_first_part, true);
			travel(travelvector - first_part_travel_vector, dt - dt_first_part);//traveling the rest					
		}
		std::cout << "Warning: Animal.cpp: Animal travels too another region, but border intersection couldnt be calculated!\n";
		std::getchar();
	}
}
void Animal::calcTravel(Point2d& travelvector, double dt, bool new_delta_world) {

	double travel_energy = current_delta_world->TravelDeltaWorld(travelvector, dt, properties_is.build.size, properties_is.build.density, properties_is.skin.feather);
	properties_is.energie_storage.is.atp -= travel_energy;
	properties_is.energie_storage.is.water -= _WATER_LOSS_FACTOR*travel_energy;	

	this->position = this->position + travelvector;
	if (new_delta_world) {
		if (!world->getWorldPartByPosition(position, current_delta_world)) {
			die();//fall over the border of this world
			return;
		}
	}

}


double Animal::calcMaxPossibleSpeed(double dt) {
	double weight = properties_is.build.density * properties_is.build.size;
	return properties_is.build.strength / weight * dt ;
}

//TODO
void Animal::clone() {

}
//TODO
void Animal::seachFood(double dt){}
//TODO
void Animal::seachWater(double dt){}
//TODO
void Animal::sleep(double dt){}
//TODO
void Animal::seachHideout(double dt){}


//TODO
bool Animal::hasReproduced() {
	return has_reproduced;
}


void Animal::update(double tnow) {
	double dt = (tnow - this->t_last_update);
	this->nextAction(dt);
	this->live(dt);
}

bool Animal::UpdateCurrentDeltaWorld() {
	if (!world->getWorldPartByPosition(position, current_delta_world)) {
		return false;
	}
	return true;
}
