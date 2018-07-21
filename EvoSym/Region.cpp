#include "Region.h"
#include <cmath>


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
	this->VariationInTemperature[0] = Season<Propability>(Propability(-1, 2.0), Propability(-4, 4.0), Propability(-2.5, 2.0), Propability(-1, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(+1, 2.0), Propability(-2, 4.0), Propability(-1, 2.0), Propability(+4, 2.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(0, 2.0), Propability(-2, 4.0), Propability(-1, 2.0), Propability(+3, 2.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(0, 2.0), Propability(-2, 4.0), Propability(-2, 2.0), Propability(0, 2.0));
	
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

	this->SkillToCross[0] = Swim();
	this->SkillToCross[1] = Fly();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Swim();
	this->SkillToReachResources[1] = Dive();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.01;
	ResourceDistributionViaSkill[1] = 0.99;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 1000;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 0;		//lake
	this->possibleNeigbourChanseFaktor[2] = 70;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 50;		//forest
	this->possibleNeigbourChanseFaktor[4] = 50;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 50;		//desert
	this->possibleNeigbourChanseFaktor[6] = 50;		//moor
	this->possibleNeigbourChanseFaktor[7] = 50;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 100;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 200;	//mountain

	this->minNeigboursWithSameRegion = 2;

	this->groundProperties.groundPt1T = 777600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.33; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.67; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 3.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -5.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToLake() {
	this->regionId = 1;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-2.5, 2.0), Propability(-1.75, 2.0), Propability(0.0, 2.0), Propability(0, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(-1.5, 2.0), Propability(-1, 2.0), Propability(0.75, 2.0), Propability(1.8, 2.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(-1.5, 2.0), Propability(-0.75, 2.0), Propability(0.75, 2.0), Propability(1.8, 2.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(-0.5, 2.0), Propability(0.0, 2.0), Propability(0.75, 2.0), Propability(3, 2.0));

	this->Height = MinMax(10,5000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;
	
	this->maxFreshWater = 5000.0;
	this->maxPlants = 0.25;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

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
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 100;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 100;		//forest
	this->possibleNeigbourChanseFaktor[4] = 50;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 50;		//moor
	this->possibleNeigbourChanseFaktor[7] = 20;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 140;		//mountain

	this->minNeigboursWithSameRegion = 0;

	this->groundProperties.groundPt1T = 1641600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.60; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.40; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 6.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 1.0; //[m] how thick one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = 4.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 4.0; //[°C] The temperature of the last layer for latitude [90°]
}
void Region::setToMeadow() {
	this->regionId = 2;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-3.0, 2.0), Propability(-1.0, 2.0), Propability(-4.0, 2.0), Propability(-7.0, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(1.0, 6.0), Propability(2.0, 3.0), Propability(-1.0, 7.0), Propability(-4, 5.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(2.0, 4.0), Propability(3.0, 1.0), Propability(0.0, 5.0), Propability(-1, 3.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(3.0, 2.0), Propability(4.0, 0.0), Propability(1.0, 4.0), Propability(0, 2.0));

	this->Height = MinMax(-100,2500.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = false;

	this->maxFreshWater = 0.05;
	this->maxPlants = 1.0;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.2;
	ResourceDistributionViaSkill[1] = 0.8;
	ResourceDistributionViaSkill[2] = 0.0;


	this->possibleNeigbourChanseFaktor[0] = 10;	//ocean
	this->possibleNeigbourChanseFaktor[1] = 3;		//lake
	this->possibleNeigbourChanseFaktor[2] = 600;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 400;		//forest
	this->possibleNeigbourChanseFaktor[4] = 100;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 75;		//moor
	this->possibleNeigbourChanseFaktor[7] = 100;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//6;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 150;		//mountain

	this->minNeigboursWithSameRegion = 3;

	this->groundProperties.groundPt1T = 201600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.51; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.49; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -2.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToForest() {
	this->regionId = 3;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-3.0, 3.0), Propability(-5.0, 1.0), Propability(-3.0, 1.0), Propability(-2.0, 3.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(-1.0, 3.0), Propability(-2.0, 1.0), Propability(-1.0, 1.0), Propability(0.0, 3.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(0.0, 2.0), Propability(0.0, 1.0), Propability(0.0, 1.0), Propability(0.0, 2.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(+5.0, 1.0), Propability(+5.0, 1.0), Propability(+5.0, 2.0));

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
	
	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
    this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.5;
	ResourceDistributionViaSkill[1] = 0.3;
	ResourceDistributionViaSkill[2] = 0.2;

	this->possibleNeigbourChanseFaktor[0] = 50;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 3;		//lake
	this->possibleNeigbourChanseFaktor[2] = 300;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 700;	//forest
	this->possibleNeigbourChanseFaktor[4] = 50;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 100;		//moor
	this->possibleNeigbourChanseFaktor[7] = 10;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//3;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 200;		//mountain

	this->minNeigboursWithSameRegion = 3;

	this->groundProperties.groundPt1T = 201600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.51; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.49; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -2.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToSteppe() {
	this->regionId = 4;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(1.0, 2.0), Propability(4.0, 5.0), Propability(3.0, 2.0), Propability(0.0, 1.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(4.0, 2.0), Propability(10.0, 5.0), Propability(6.0, 2.0), Propability(3.0, 1.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(12.0, 5.0), Propability(7.0, 2.0), Propability(4.0, 1.0));

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
	this->possibleNeigbourChanseFaktor[1] = 3;		//lake
	this->possibleNeigbourChanseFaktor[2] = 70;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 10;		//forest
	this->possibleNeigbourChanseFaktor[4] = 800;	//steppe
	this->possibleNeigbourChanseFaktor[5] = 300;		//desert
	this->possibleNeigbourChanseFaktor[6] = 10;		//moor
	this->possibleNeigbourChanseFaktor[7] = 100;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//3;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 110;		//mountain

	this->minNeigboursWithSameRegion = 4;

	//todo abhängig von trockenzeit = wüßte/regenzeit=meadow
	this->groundProperties.groundPt1T = 201600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.51; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.49; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = 0.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToDesert() {
	this->regionId = 5;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(4.0, 4.0), Propability(9.0, 4.0), Propability(7.0, 4.0), Propability(0.0, 4.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(7.0, 4.0), Propability(15.0, 4.0), Propability(13.0, 4.0), Propability(7.0, 4.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(10.0, 4.0), Propability(18.0, 4.0), Propability(14.0, 4.0), Propability(10.0, 4.0));

	this->Height = MinMax(-100,1000.0);
	this->occurrenceInTemperateZone[0] = false;
	this->occurrenceInTemperateZone[1] = false;
	this->occurrenceInTemperateZone[2] = false;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.001;
	this->maxPlants = 0.01;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.01;
	ResourceDistributionViaSkill[1] = 0.99;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 600;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 3;		//lake
	this->possibleNeigbourChanseFaktor[2] = 20;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 0;		//forest
	this->possibleNeigbourChanseFaktor[4] = 400;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 800;	//desert
	this->possibleNeigbourChanseFaktor[6] = 0;		//moor
	this->possibleNeigbourChanseFaktor[7] = 10;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;// 5;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 150;		//mountain

	this->minNeigboursWithSameRegion = 3;

	this->groundProperties.groundPt1T = 86400; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.33; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.67; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 1.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = 24.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToMoor() {
	this->regionId = 6;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-1.0, 7.0), Propability(-1.0, 3.5), Propability(0.0, 6.5), Propability(-3.5, 6.5));
	this->VariationInTemperature[1] = Season<Propability>(Propability(0.0, 7.0), Propability(0.0, 3.5), Propability(1.0, 6.5), Propability(-3.5, 6.5));
	this->VariationInTemperature[2] = Season<Propability>(Propability(+1.0, 5.0), Propability(+1.0, 1.5), Propability(2.0, 3.5), Propability(-1, 3));
	this->VariationInTemperature[3] = Season<Propability>(Propability(+2.0, 2.0), Propability(+2.0, 0.5), Propability(2.0, 1.5), Propability(0, 1.5));

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

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Dig();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.1;
	ResourceDistributionViaSkill[1] = 0.9;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 20;		//lake
	this->possibleNeigbourChanseFaktor[2] = 300;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 200;		//forest
	this->possibleNeigbourChanseFaktor[4] = 10;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 500;		//moor
	this->possibleNeigbourChanseFaktor[7] = 10;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 150;		//mountain

	this->minNeigboursWithSameRegion = 2;

	this->groundProperties.groundPt1T = 345600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.44; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.56; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -8.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 20.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToTropicalForest() {
	this->regionId = 7;
	this->VariationInTemperature[0] = Season<Propability>(Propability(0.0, 3.0), Propability(-1.0, 1.0), Propability(0.0, 1.0), Propability(0.0, 3.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(1.5, 3.0), Propability(0.5, 1.0), Propability(1.5, 1.0), Propability(1.5, 3.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(3.5, 2.0), Propability(2.5, 1.0), Propability(3.5, 1.0), Propability(3.5, 2.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(5.0, 2.0), Propability(+5.0, 1.0), Propability(+5.0, 1.0), Propability(+5.0, 2.0));

	this->Height = MinMax(-100,2500.0);
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

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.6;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.3;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 20;		//lake
	this->possibleNeigbourChanseFaktor[2] = 50;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 10;		//forest
	this->possibleNeigbourChanseFaktor[4] = 50;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 50;		//moor
	this->possibleNeigbourChanseFaktor[7] = 900;	//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 0;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 400;	//mountain

	this->minNeigboursWithSameRegion = 3;

	this->groundProperties.groundPt1T = 777600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.65; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.35; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = 5.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 24.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToIceDesert() {
	this->regionId = 8;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(-6.0, 2.0), Propability(-2.0, 2.0), Propability(-6.0, 2.0), Propability(-11.0, 2.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0, 0.0));

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

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = NoSkill();
	this->SkillToReachResources[1] = NoSkill();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 1.0;
	ResourceDistributionViaSkill[1] = 0.0;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 400;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 50;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 50;		//forest
	this->possibleNeigbourChanseFaktor[4] = 0;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 0;		//desert
	this->possibleNeigbourChanseFaktor[6] = 50;		//moor
	this->possibleNeigbourChanseFaktor[7] = 0;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 900;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//4;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 200;	//mountain

	this->minNeigboursWithSameRegion = 4;

	this->groundProperties.groundPt1T = 489600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.50; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.50; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.5; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -20; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 0.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToHeighland() {
	this->regionId = 9;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-5, 10.0), Propability(-3, 5.0), Propability(-5.0, 10.0), Propability(-9, 10.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0, 0.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0, 0.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0.0, 0.0), Propability(0, 0.0));

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

	this->SkillToCross[0] = NoSkill();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.1;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.8;

	this->possibleNeigbourChanseFaktor[0] = 100;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 10;		//lake
	this->possibleNeigbourChanseFaktor[2] = 50;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 20;		//forest
	this->possibleNeigbourChanseFaktor[4] = 10;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 10;		//moor
	this->possibleNeigbourChanseFaktor[7] = 20;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 50;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;//10;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 100;		//mountain

	this->minNeigboursWithSameRegion = 0;

	this->groundProperties.groundPt1T = 201600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.55; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.45; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.0; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -10.0; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 20.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToMountain() {
	this->regionId = 10;
	this->VariationInTemperature[0] = Season<Propability>(Propability(-10,4.0), Propability(-8, 4.0), Propability(-9.0, 4.0), Propability(-11, 4.0));
	this->VariationInTemperature[1] = Season<Propability>(Propability(-5, 10.0), Propability(-2, 10.0), Propability(-3.0, 10.0), Propability(-7, 10.0));
	this->VariationInTemperature[2] = Season<Propability>(Propability(-3, 4.0), Propability(-1, 4.0), Propability(-2.0, 4.0), Propability(-6, 4.0));
	this->VariationInTemperature[3] = Season<Propability>(Propability(0.0, 2.0), Propability(0.0, 2.0), Propability(0.0, 2.0), Propability(0, 2.0));

	this->Height = MinMax(800.0, _MAX_TERRANE_HEIGHT);
	this->occurrenceInTemperateZone[0] = true;
	this->occurrenceInTemperateZone[1] = true;
	this->occurrenceInTemperateZone[2] = true;
	this->occurrenceInTemperateZone[3] = true;

	this->maxFreshWater = 0.1;
	this->maxPlants = 0.4;
	this->TauFreshWater = this->getRegionBasedWaterRegeneration(this->regionId);
	this->TauPlants = this->getRegionBasedPlantGrowth(this->regionId);
	this->tempOptimalGroth = this->getOptimalTempForGrowth(this->regionId);

	this->SkillToCross[0] = Climb();
	this->SkillToCross[1] = NoSkill();
	this->SkillToCross[2] = NoSkill();
	this->SkillToReachResources[0] = Climb();
	this->SkillToReachResources[1] = Dig();
	this->SkillToReachResources[2] = NoSkill();
	ResourceDistributionViaSkill[0] = 0.9;
	ResourceDistributionViaSkill[1] = 0.1;
	ResourceDistributionViaSkill[2] = 0.0;

	this->possibleNeigbourChanseFaktor[0] = 10;		//ocean
	this->possibleNeigbourChanseFaktor[1] = 1;		//lake
	this->possibleNeigbourChanseFaktor[2] = 100;		//meadow
	this->possibleNeigbourChanseFaktor[3] = 150;		//forest
	this->possibleNeigbourChanseFaktor[4] = 40;		//steppe
	this->possibleNeigbourChanseFaktor[5] = 10;		//desert
	this->possibleNeigbourChanseFaktor[6] = 100;		//moor
	this->possibleNeigbourChanseFaktor[7] = 150;		//tropical forest
	this->possibleNeigbourChanseFaktor[8] = 10;		//ice desert
	this->possibleNeigbourChanseFaktor[9] = 0;// 5;		//highlands
	this->possibleNeigbourChanseFaktor[10] = 800;		//mountain

	this->minNeigboursWithSameRegion = 4;

	this->groundProperties.groundPt1T = 489600; //T parameter of discrete Pt1 behaviour
	this->groundProperties.groundaboveLayerFacator = 0.50; //weigheted mean factor for the layer above
	this->groundProperties.groundbelowLayerFacator = 0.50; //weigheted mean factor for the layer below
	this->groundProperties.groundDepth = 2.5; //[m] at which layer we have a stable temperature
	this->groundProperties.groundLayerThickness = 0.25; //[m] how thik one layer shall be
	this->groundProperties.groundLastLayerTemp[0] = -15; //[°C] The temperature of the last layer for latitude [0°]
	this->groundProperties.groundLastLayerTemp[1] = 5.0; //[°C] The temperature of the last layer for latitude [90°]

}
void Region::setToDefault() {
	this->setToMeadow();
}

Season<Propability>* Region::getAllSeasonDependantTempVariation(int tempZohne) {
	return &this->VariationInTemperature[tempZohne];
}
double Region::getSeasonDependantTempVariation(int seasonId, int tempZohne) {
	return this->VariationInTemperature[tempZohne].getValue(seasonId).mean_min;
}
GroundProperties* Region::getGroundProperties() {
	return &this->groundProperties;
}