#include "Region.h"
#include <cmath>

//resourcenzuwachs ist angegeben in wie viel Tagen sich der Bestand verdoppelt haben soll
//2=x^t == e^(ln(x)*t)
//x = 2^(1/t) -> in t * delta_SimulationsZeit verdoppelt sich die resource 
// also pro updateeinheit: resourceNeu = resourceAlt * x^deltaT_Update

void Region::setTo(int i) {
	this->regionId = i;
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
		this->setToHeighland();
		break;
	case 10:
		this->setToMountain();
		break;
	default:
		this->setToDefault();
		break;
	}
}

void Region::setToOcean() {
	this->regionId = 0;
	this->VariationInTemperature = Season<Propability>(Propability(+2, 2.0), Propability(-2, 4.0), Propability(-1, 2.0), Propability(+4, 2.0));
	this->Height = MinMax(_MIN_TERRANE_HEIGHT,10.0);
	this->occurrenceInTemperateZone[0] = true;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.0;
	this->maxPlants = 0.25;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth = 0.5*_QUARTER_YEAR_IN_S;
	this->max_ice_thickness = 10.0;

	this->SkillToCross[0] = Swim();
	this->SkillToCross[1] = Fly();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Swim();
	this->SkillToReachResources[1] = Dive();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.01;
	ResourceDistributionViaSkill[1] = 0.99;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 30;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 0;		//lake
	this->possibleNeigbourChanseFaktor[2] = 5;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 2;		//forest
	this->possibleNeigbourChanseFaktor[4] = 2;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 5;		//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 5;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 5;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//10;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 10;		//mountain

	this->minNeigboursWithSameRegion = 2;
}
void Region::setToLake() {
	this->regionId = 1;
	this->VariationInTemperature = Season<Propability>(Propability(-1.0, 7.0), Propability(-3, 5.0), Propability(-2.0, 5.0), Propability(-3.0, 7.0));
	this->Height = MinMax(100,5000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;
	
	this->maxFreshWater = 5000.0;
	this->maxPlants = 0.25;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =(1.2*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 2.0;

	this->SkillToCross[0] = Swim();
	this->SkillToCross[1] = Fly();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Swim();
	this->SkillToReachResources[1] = Dive();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.05;
	ResourceDistributionViaSkill[1] = 0.95;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 0;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 5;		//lake
	this->possibleNeigbourChanseFaktor[2] = 10;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 10;		//forest
	this->possibleNeigbourChanseFaktor[4] = 10;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 10;		//moor
	this->possibleNeigbourChanseFaktor[7] = 10;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//5;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 5;		//mountain

	this->minNeigboursWithSameRegion = 0;
}
void Region::setToMeadow() {
	this->regionId = 2;
	this->VariationInTemperature = Season<Propability>(Propability(1.0, 7.0), Propability(5.0, 2.0), Propability(3.0, 7.0), Propability(-4, 5.0));
	this->Height = MinMax(-100,3000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = false;

	this->maxFreshWater = 0.05;
	this->maxPlants = 1.0;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth = (0.5*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.2;
	ResourceDistributionViaSkill[1] = 0.8;
	ResourceDistributionViaSkill[2] = 0.0;


	this->possibleNeigbourChanseFaktor[0] = 7;	//ocean
	this->possibleNeigbourChanseFaktor[1] = 5;		//lake
	this->possibleNeigbourChanseFaktor[2] = 15;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 7;		//forest
	this->possibleNeigbourChanseFaktor[4] = 5;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 6;		//moor
	this->possibleNeigbourChanseFaktor[7] = 4;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 15;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//6;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 5;		//mountain

	this->minNeigboursWithSameRegion = 1;
}
void Region::setToForest() {
	this->regionId = 3;
	this->VariationInTemperature = Season<Propability>(Propability(-1.0, 1.0), Propability(-2.0, 1.0), Propability(-1.0, 1.0), Propability(0.0, 1.0));
	this->Height = MinMax(-100,2200.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = false;

	this->maxFreshWater = 0.05;
	this->maxPlants = 4.0;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth = (1.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	
	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
    this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.5;
	ResourceDistributionViaSkill[1] = 0.3;
	ResourceDistributionViaSkill[2] = 0.2;

	this->possibleNeigbourChanseFaktor[0] = 2;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 3;		//lake
	this->possibleNeigbourChanseFaktor[2] = 6;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 15;	//forest
	this->possibleNeigbourChanseFaktor[4] = 0;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 4;		//moor
	this->possibleNeigbourChanseFaktor[7] = 1;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//3;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 1;		//mountain

	this->minNeigboursWithSameRegion = 2;
}
void Region::setToSteppe() {
	this->regionId = 4;
	this->VariationInTemperature = Season<Propability>(Propability(4.0, 2.0), Propability(10.0, 5.0), Propability(6.0, 2.0), Propability(3.0, 1.0));
	this->Height = MinMax(-100,800.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 5.0;
	this->maxPlants = 4.0;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =(10.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.1;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.8;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 2;		//lake
	this->possibleNeigbourChanseFaktor[2] = 4;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 0;		//forest
	this->possibleNeigbourChanseFaktor[4] = 6;	//steppe
	this->possibleNeigbourChanseFaktor[5] = 5;		//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 2;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//3;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 1;		//mountain

	this->minNeigboursWithSameRegion = 2;
}
void Region::setToDesert() {
	this->regionId = 5;
	this->VariationInTemperature = Season<Propability>(Propability(10.0, 4.0), Propability(18.0, 4.0), Propability(14.0, 4.0), Propability(10.0, 4.0));
	this->Height = MinMax(-100,700.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = false;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.001;
	this->maxPlants = 0.01;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =  (10.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.01;
	ResourceDistributionViaSkill[1] = 0.99;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 0;		//lake
	this->possibleNeigbourChanseFaktor[2] = 0;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 0;		//forest
	this->possibleNeigbourChanseFaktor[4] = 8;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 20;	//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 0;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;// 5;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 0;		//mountain

	this->minNeigboursWithSameRegion = 4;
}
void Region::setToMoor() {
	this->regionId = 6;
	this->VariationInTemperature =	Season<Propability>(Propability(0.0, 7.0), Propability(0.0, 3.5), Propability(1.0, 6.5), Propability(-3.5, 6.5));
	this->Height = MinMax(-100,2000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = false;

	this->maxFreshWater = 1.0;
	this->maxPlants = 2.0;
	this->TauFreshWater = Season<double>(100, 100, 100, 100);
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =  (1.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.7;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.1;
	ResourceDistributionViaSkill[1] = 0.9;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 0;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 7;		//lake
	this->possibleNeigbourChanseFaktor[2] = 5;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 1;		//forest
	this->possibleNeigbourChanseFaktor[4] = 0;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 10;		//moor
	this->possibleNeigbourChanseFaktor[7] = 0;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 1;		//mountain

	this->minNeigboursWithSameRegion = 1;
}
void Region::setToTropicalForest() {
	this->regionId = 7;
	this->VariationInTemperature = Season<Propability>(Propability(+5.0, 2.0), Propability(+5.0, 1.0), Propability(+5.0, 1.0), Propability(+5.0, 2.0));
	this->Height = MinMax(-100,2000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = false;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.25;
	this->maxPlants = 8.0;
	this->TauFreshWater = Season<double>(100, 100, 100, 100);
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth = (10.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.6;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.3;

	this->possibleNeigbourChanseFaktor[0] = 5;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 5;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 0;		//forest
	this->possibleNeigbourChanseFaktor[4] = 4;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 8;	//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 2;	//mountain

	this->minNeigboursWithSameRegion = 3;
}
void Region::setToIceDesert() {
	this->regionId = 8;
	this->VariationInTemperature = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->Height = MinMax(-100, _MAX_TERRANE_HEIGHT);
	this->occurrenceInTemperateZone[0] = true;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = false;
	this->occurrenceInTemperateZone[3] = false;
	
	this->maxFreshWater = 0.05;
	this->maxPlants = 0.005;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =(0.1*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 2.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = NoSkill();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 1.0;
	ResourceDistributionViaSkill[1] = 0.0;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 5;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 2;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 0;		//forest
	this->possibleNeigbourChanseFaktor[4] = 0;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 0;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 2;	//mountain

	this->minNeigboursWithSameRegion = 2;
}
void Region::setToHeighland() {
	this->regionId = 9;
	this->VariationInTemperature = Season<Propability>(Propability(-5, 10.0), Propability(-3, 5.0), Propability(-5.0, 10.0), Propability(-9, 10.0));
	this->Height = MinMax(800,2000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = false;
	this->occurrenceInTemperateZone[3] = false;
	
	this->maxFreshWater = 0.1;
	this->maxPlants = 1.0;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =  (1.0*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.5;

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.1;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.8;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 5;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 2;		//forest
	this->possibleNeigbourChanseFaktor[4] = 1;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 1;		//desert
	this->possibleNeigbourChanseFaktor[6] = 1;		//moor
	this->possibleNeigbourChanseFaktor[7] = 1;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 5;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//10;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 10;		//mountain

	this->minNeigboursWithSameRegion = 0;
}
void Region::setToMountain() {
	this->regionId = 10;
	this->VariationInTemperature = Season<Propability>(Propability(-5,10.0), Propability(-1, 10.0), Propability(-2.0, 10.0), Propability(-6, 10.0));
	this->Height = MinMax(1000.0, _MAX_TERRANE_HEIGHT);
	this->occurrenceInTemperateZone[0] = true;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.1;
	this->maxPlants = 0.4;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->tau_ice_growth =(0.8*_QUARTER_YEAR_IN_S);
	this->max_ice_thickness = 0.7;

	this->SkillToCross[0] = Climb();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.9;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 5;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 1;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 1;		//forest
	this->possibleNeigbourChanseFaktor[4] = 0;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 2;		//moor
	this->possibleNeigbourChanseFaktor[7] = 2;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 2;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;// 5;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 8;		//mountain

	this->minNeigboursWithSameRegion = 2;
}
void Region::setToDefault() {
	this->setToMeadow();
}

Season<Propability>* Region::getAllSeasonDependantTempVariation() {
	return &this->VariationInTemperature;
}
double Region::getSeasonDependantTempVariation(int seasonId) {
	return this->VariationInTemperature.getValue(seasonId).mean_min;
}