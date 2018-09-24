#include "Region.h"
#include <cmath>


void Region::setTo(int i) {
	this->region_id = i;
	switch (i)
	{
	case 0:
		this->setToOcean();
		break;
	case 1:
		this->setToLake();
		break;
	case 2:
		this->setToMeadow();
		break;
	case 3:
		this->setToForest();
		break;
	case 4:
		this->setToSteppe();
		break;
	case 5:
		this->setToDesert();
		break;
	case 6:
		this->setToMoor();
		break;
	case 7:
		this->setToTropicalForest();
		break;
	case 8:
		this->setToIceDesert();
		break;
	case 9:
		this->setToMountain();
		break;
	default:
		this->setToDefault();
		break;
	}
}

void Region::setToOcean() {
	this->region_id = 0;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-1, 2.0), Propability(-4, 4.0), Propability(-2.5, 2.0), Propability(-1, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(+1, 2.0), Propability(-2, 4.0), Propability(-1, 2.0), Propability(+4, 2.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(0, 2.0), Propability(-2, 4.0), Propability(-1, 2.0), Propability(+3, 2.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(0, 2.0), Propability(-2, 4.0), Propability(-2, 2.0), Propability(0, 2.0));
	
	this->height = MinMax(_MIN_TERRANE_HEIGHT,10.0);
	this->occurrence_in_temperature_zone[0] = 1.0;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 1.0;

	this->max_fresh_water = 0.0;
	this->max_plants = 0.25;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _DAY_IN_S;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S*15.0;

	this->possible_neighbour_chanse_factor[0] = 0.2;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.0;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 2;

	this->ground_properties.ground_pt1_t = 777600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.33; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.67; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 3.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -5.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToLake() {
	this->region_id = 1;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-2.5, 2.0), Propability(-1.75, 2.0), Propability(0.0, 2.0), Propability(0, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(-1.5, 2.0), Propability(-1, 2.0), Propability(0.75, 2.0), Propability(1.8, 2.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(-1.5, 2.0), Propability(-0.75, 2.0), Propability(0.75, 2.0), Propability(1.8, 2.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(-0.5, 2.0), Propability(0.0, 2.0), Propability(0.75, 2.0), Propability(3, 2.0));

	this->height = MinMax(-200,5000.0);
	this->occurrence_in_temperature_zone[0] = 0.01;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 1.0;
	
	this->max_fresh_water = 5000.0;
	this->max_plants = 0.25;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = 0.1; //like in an instance
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S*15.0;

	this->possible_neighbour_chanse_factor[0] = 0.0;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.01;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.21;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.18;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 0;

	this->ground_properties.ground_pt1_t = 1641600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.60; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.40; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 6.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 1.0; //[m] how thick one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = 4.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 4.0; //[°C] The temperature of the last layer for latitude [90°]
}
void Region::setToMeadow() {
	this->region_id = 2;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-3.0, 2.0), Propability(-1.0, 2.0), Propability(-4.0, 2.0), Propability(-7.0, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(1.0, 6.0), Propability(2.0, 3.0), Propability(-1.0, 7.0), Propability(-4, 5.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(2.0, 4.0), Propability(3.0, 1.0), Propability(0.0, 5.0), Propability(-1, 3.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(3.0, 2.0), Propability(4.0, 0.0), Propability(1.0, 4.0), Propability(0, 2.0));

	this->height = MinMax(-50,2500.0);
	this->occurrence_in_temperature_zone[0] = 0.01;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 0.3;

	this->max_fresh_water = 0.05;
	this->max_plants = 1.0;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 15;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 5.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.05;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.15;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 3;

	this->ground_properties.ground_pt1_t = 201600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.51; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.49; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -2.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToForest() {
	this->region_id = 3;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-3.0, 3.0), Propability(-5.0, 1.0), Propability(-3.0, 1.0), Propability(-2.0, 3.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(-1.0, 3.0), Propability(-2.0, 1.0), Propability(-1.0, 1.0), Propability(0.0, 3.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(0.0, 2.0), Propability(0.0, 1.0), Propability(0.0, 1.0), Propability(0.0, 2.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(+5.0, 1.0), Propability(+5.0, 1.0), Propability(+5.0, 2.0));

	this->height = MinMax(-50,2200.0);
	this->occurrence_in_temperature_zone[0] = 0.01;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 0.01;

	this->max_fresh_water = 0.05;
	this->max_plants = 4.0;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 15.0;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 7.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.03;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.17;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 3;

	this->ground_properties.ground_pt1_t = 201600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.51; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.49; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -2.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToSteppe() {
	this->region_id = 4;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(1.0, 2.0), Propability(4.0, 5.0), Propability(3.0, 2.0), Propability(0.0, 1.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(4.0, 2.0), Propability(10.0, 5.0), Propability(6.0, 2.0), Propability(3.0, 1.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(12.0, 5.0), Propability(7.0, 2.0), Propability(4.0, 1.0));

	this->height = MinMax(-50,800.0);
	this->occurrence_in_temperature_zone[0] = 0.001;
	this->occurrence_in_temperature_zone[1] = 0.01;
	this->occurrence_in_temperature_zone[2] = 0.8;
	this->occurrence_in_temperature_zone[3] = 1.0;

	this->max_fresh_water = 5.0;
	this->max_plants = 4.0;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 15;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 5.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.001;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.199;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 4;

	//todo abhängig von trockenzeit = wüßte/regenzeit=meadow
	this->ground_properties.ground_pt1_t = 201600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.51; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.49; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = 0.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToDesert() {
	this->region_id = 5;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(4.0, 4.0), Propability(9.0, 4.0), Propability(7.0, 4.0), Propability(0.0, 4.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(7.0, 4.0), Propability(15.0, 4.0), Propability(13.0, 4.0), Propability(7.0, 4.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(10.0, 4.0), Propability(18.0, 4.0), Propability(14.0, 4.0), Propability(10.0, 4.0));

	this->height = MinMax(-50,1000.0);
	this->occurrence_in_temperature_zone[0] = 0.0;
	this->occurrence_in_temperature_zone[1] = 0.0;
	this->occurrence_in_temperature_zone[2] = 0.1;
	this->occurrence_in_temperature_zone[3] = 1.0;

	this->max_fresh_water = 0.001;
	this->max_plants = 0.01;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _HOUR_IN_S * 3;
	this->exp_distrib_lambda_find_all_plants = _HOUR_IN_S;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.0001;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1999;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 3;

	this->ground_properties.ground_pt1_t = 86400; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.33; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.67; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 1.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = 24.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToMoor() {
	this->region_id = 6;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-1.0, 7.0), Propability(-1.0, 3.5), Propability(0.0, 6.5), Propability(-3.5, 6.5));
	this->variation_in_temperature[1] = Season<Propability>(Propability(0.0, 7.0), Propability(0.0, 3.5), Propability(1.0, 6.5), Propability(-3.5, 6.5));
	this->variation_in_temperature[2] = Season<Propability>(Propability(+1.0, 5.0), Propability(+1.0, 1.5), Propability(2.0, 3.5), Propability(-1, 3));
	this->variation_in_temperature[3] = Season<Propability>(Propability(+2.0, 2.0), Propability(+2.0, 0.5), Propability(2.0, 1.5), Propability(0, 1.5));

	this->height = MinMax(-50,2000.0);
	this->occurrence_in_temperature_zone[0] = 0.01;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 0.1;

	this->max_fresh_water = 1.0;
	this->max_plants = 2.0;
	this->tau_fresh_water = Season<double>(100, 100, 100, 100);
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 1;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 5.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.1;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 2;

	this->ground_properties.ground_pt1_t = 345600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.44; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.56; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -8.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 20.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToTropicalForest() {
	this->region_id = 7;
	this->variation_in_temperature[0] = Season<Propability>(Propability(0.0, 3.0), Propability(-1.0, 1.0), Propability(0.0, 1.0), Propability(0.0, 3.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(1.5, 3.0), Propability(0.5, 1.0), Propability(1.5, 1.0), Propability(1.5, 3.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(3.5, 2.0), Propability(2.5, 1.0), Propability(3.5, 1.0), Propability(3.5, 2.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(+5.0, 1.0), Propability(+5.0, 1.0), Propability(+5.0, 2.0));

	this->height = MinMax(-50,2500.0);
	this->occurrence_in_temperature_zone[0] = 0.0;
	this->occurrence_in_temperature_zone[1] = 0.0;
	this->occurrence_in_temperature_zone[2] = 0.2;
	this->occurrence_in_temperature_zone[3] = 1.0;

	this->max_fresh_water = 0.25;
	this->max_plants = 8.0;
	this->tau_fresh_water = Season<double>(100, 100, 100, 100);
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 10.0;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 19.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.03;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.17;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 3;

	this->ground_properties.ground_pt1_t = 777600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.65; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.35; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.0; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = 5.0; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToIceDesert() {
	this->region_id = 8;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(2.0, 2.0), Propability(0, 2.0), Propability(2, 2.0), Propability(4, 2.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0, 0.0));

	this->height = MinMax(-50, _MAX_TERRANE_HEIGHT);
	this->occurrence_in_temperature_zone[0] = 1.0;
	this->occurrence_in_temperature_zone[1] = 0.01;
	this->occurrence_in_temperature_zone[2] = 0.0;
	this->occurrence_in_temperature_zone[3] = 0.0;
	
	this->max_fresh_water = 0.05;
	this->max_plants = 0.005;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 35.0;
	this->exp_distrib_lambda_find_all_plants = _HOUR_IN_S * 7.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.001;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.199;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.1;	//mountain

	this->min_neighbour_with_same_region = 4;

	this->ground_properties.ground_pt1_t = 489600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.50; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.50; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.5; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -20; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 0.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToMountain() {
	this->region_id = 9;
	this->variation_in_temperature[0] = Season<Propability>(Propability(-10,4.0), Propability(-8, 4.0), Propability(-9.0, 4.0), Propability(-11, 4.0));
	this->variation_in_temperature[1] = Season<Propability>(Propability(-5, 10.0), Propability(-2, 10.0), Propability(-3.0, 10.0), Propability(-7, 10.0));
	this->variation_in_temperature[2] = Season<Propability>(Propability(-3, 4.0), Propability(-1, 4.0), Propability(-2.0, 4.0), Propability(-4.5, 4.0));
	this->variation_in_temperature[3] = Season<Propability>(Propability(0.0, 2.0), Propability(0.0, 2.0), Propability(0.0, 2.0), Propability(0, 2.0));

	this->height = MinMax(800.0, _MAX_TERRANE_HEIGHT);
	this->occurrence_in_temperature_zone[0] = 1.0;
	this->occurrence_in_temperature_zone[1] = 1.0;
	this->occurrence_in_temperature_zone[2] = 1.0;
	this->occurrence_in_temperature_zone[3] = 1.0;

	this->max_fresh_water = 0.1;
	this->max_plants = 0.4;
	this->tau_fresh_water = this->getRegionBasedWaterRegeneration(this->region_id);
	this->tau_plants = this->getRegionBasedPlantGrowth(this->region_id);
	this->temp_optimal_growth = this->getOptimalTempForGrowth(this->region_id);

	this->exp_distrib_lambda_find_all_water = _MINUTE_IN_S * 45.0;
	this->exp_distrib_lambda_find_all_plants = _MINUTE_IN_S * 10.0;

	this->possible_neighbour_chanse_factor[0] = 0.1;	//ocean
	this->possible_neighbour_chanse_factor[1] = 0.02;	//lake
	this->possible_neighbour_chanse_factor[2] = 0.1;	//meadow
	this->possible_neighbour_chanse_factor[3] = 0.1;	//forest
	this->possible_neighbour_chanse_factor[4] = 0.1;	//steppe
	this->possible_neighbour_chanse_factor[5] = 0.1;	//desert
	this->possible_neighbour_chanse_factor[6] = 0.1;	//moor
	this->possible_neighbour_chanse_factor[7] = 0.1;	//tropical forest
	this->possible_neighbour_chanse_factor[8] = 0.1;	//ice desert
	this->possible_neighbour_chanse_factor[9] = 0.18;	//mountain

	this->min_neighbour_with_same_region = 4;

	this->ground_properties.ground_pt1_t = 489600; //T parameter of discrete Pt1 behaviour
	this->ground_properties.ground_above_layer_factor = 0.50; //weigheted mean factor for the layer above
	this->ground_properties.ground_below_layer_factor = 0.50; //weigheted mean factor for the layer below
	this->ground_properties.ground_depth = 2.5; //[m] at which layer we have a stable temperature
	this->ground_properties.ground_layer_thickness = 0.25; //[m] how thik one layer shall be
	this->ground_properties.ground_last_layer_temp[0] = -15; //[°C] The temperature of the last layer for latitude [0°]
	this->ground_properties.ground_last_layer_temp[1] = 20.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToDefault() {
	this->setToMeadow();
	this->region_id = -1;
}

Season<Propability>* Region::getAllSeasonDependantTempVariation(int tempZohne) {
	return &this->variation_in_temperature[tempZohne];
}
double Region::getSeasonDependantTempVariation(int seasonId, int tempZohne) {
	return this->variation_in_temperature[tempZohne].getValue(seasonId).mean_min;
}
GroundProperties* Region::getGroundProperties() {
	return &this->ground_properties;
}
