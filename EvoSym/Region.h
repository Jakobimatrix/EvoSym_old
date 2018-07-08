#ifndef _REGION_
#define _REGION_

#include "globals.h"
#include "structs.h"

#include "Skills.h"

typedef struct GroundProperties {
	double groundPt1T; //T parameter of discrete Pt1 behaviour
	double groundaboveLayerFacator; //weigheted mean factor for the layer above
	double groundbelowLayerFacator; //weigheted mean factor for the layer below
	double groundDepth; //[m] at which layer we have a stable temperature
	double groundLayerThickness; //[m] how thik one layer shall be
	double groundLastLayerTemp[2]; //[°C,°C] The temperature of the last layer for latitude [0°,90°]
}GroundProperties;

class Region
{
private:
	
	Season<Propability> VariationInTemperature[4]; //regionalabhängiges Temperaturrauschen für polar,mediteran,subtrop,trop
	MinMax Height;
	bool occurrenceInTemperateZone[4];	//mit welcher wahrschenlichkeit diese Region in einer der 4 Klimazonen vorkommt
		
	Skills SkillToCross[3];	//alle Skills, die ein Tier haben muss, um durch diese Region laufen/fliegen/schwimmen zu können
	Skills SkillToReachResources[3]; //alle Skills, die notwendig sind um die resourcen erreichen zu können
	double ResourceDistributionViaSkill[3];//wie viel Prozent der Resourcen mit diesem Skill zu erreichen sind

	int possibleNeigbourChanseFaktor[_AMOUNT_REGIONS];//rellative Wahrscheinlichkeit, mit der DIESE Region auftritt in Abhängigkeit eines Nachbarn
	int regionId;
	int minNeigboursWithSameRegion;//DIESE Region tritt nur auf, wenn es mindestens n Nachbarn gibt, die auch DIESE Region sind

	double maxFreshWater;	//l per m^2
	double maxPlants;		//Kg per m^2
	double tempOptimalGroth;
	Season<double> TauFreshWater;
	Season<double> TauPlants;

	GroundProperties groundProperties;

public:
	Region() {
		this->setToMeadow();

	};
	void setTo(int i);
	void setToOcean();
	void setToLake();
	void setToMeadow();
	void setToForest();
	void setToSteppe();
	void setToDesert();
	void setToMoor();
	void setToTropicalForest();
	void setToIceDesert();
	void setToHeighland();
	void setToMountain();
	void setToDefault();

	bool occoursInTempZone(int i)
	{
		if (i > _AMOUNT_TEMPERATE_ZONES) return 0;
		return this->occurrenceInTemperateZone[i];
	}
	bool occoursInHeight(double h) {
		return this->Height.isWithin(h);
	}
	int getRegionChanseFaktor(int region) {
		if (region > _AMOUNT_REGIONS) {
			std::cout << "ERROR: Region ID not valide" << std::endl;
			return 0;
		}
		return this->possibleNeigbourChanseFaktor[region];
	}

	Season<Propability>* getAllSeasonDependantTempVariation(int tempZohne);

	double getSeasonDependantTempVariation(int seasonId, int tempZohne);

	int getRegionId() {
		return this->regionId;
	}

	bool hasEnoughNeigboursWithSameRegion(int neigbours) {
		return neigbours < this->minNeigboursWithSameRegion ? false : true;
	}

	double getMaxPlants() {
		return this->maxPlants;
	}
	double  getMaxFreshWater() {
		return this->maxFreshWater;
	}
	Season<double>* getTauFreshWater() {
		return &this->TauFreshWater;
	}
	Season<double>* getTauPlants() {
		return &this->TauPlants;
	}


	static std::string getRegionName(int i) {
		
		switch (i)
		{
		case 0:
			return "Ocean";
		case 1:
			return "Lake";
		case 2:
			return "Meadow";
		case 3:
			return "Forest";
		case 4:
			return "Steppe";
		case 5:
			return "Desert";
		case 6:
			return "Moor";
		case 7:
			return "Tropical forest";
		case 8:
			return "Ice dessert";
		case 9:
			return "heightland";
		case 10:
			return "Mountain";
		default:
			return "Default";
			break;
		}			
	}

	//Alle Werte sin in 1/Tau wobei nach Tau Sekunden die resource von Null voll aufgefüllt ist
	static Season<double> getRegionBasedWaterRegeneration(int ID) {
		switch (ID)
		{
		case 0://ocean
			return 	Season<double>();
		case 1://Lake
			return 	Season<double>(1, 1, 1, 1);
		case 2://Medow
			return 	Season<double>(1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (4.0 *		_QUARTER_YEAR_IN_S));
		case 3://Forest
			return 	Season<double>(1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (4.0 *		_QUARTER_YEAR_IN_S));
		case 4://Steppe	
			return 	Season<double>(1.0 / (0.1 *		_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S));
		case 5://Desert
			return 	Season<double>(1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S));
		case 6://Moor
			return 	Season<double>(1.0 / (0.1 *		_QUARTER_YEAR_IN_S), 1.0 / (0.1 *		_QUARTER_YEAR_IN_S), 1.0 / (0.1 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S));
		case 7://tropical Forest
			return 	Season<double>(1.0 / (0.1 *		_QUARTER_YEAR_IN_S), 1.0 / (0.2 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S));
		case 8://Ice desert
			return 	Season<double>(1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S));
		case 9://highlands
			return 	Season<double>(1.0 / (0.5 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (10.0 *	_QUARTER_YEAR_IN_S), 1.0 / (10.0 *	_QUARTER_YEAR_IN_S));
		case 10://mountain
			return 	Season<double>(1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (20.0 *	_QUARTER_YEAR_IN_S), 1.0 / (40.0 *	_QUARTER_YEAR_IN_S));
		default:
			return 	Season<double>(1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S));
			break;
		}
	}
	//Alle Werte sin in 1/Tau wobei nach Tau Sekunden die resource von Null voll aufgefüllt ist
	static Season<double> getRegionBasedPlantGrowth(int ID) {
		switch (ID)
		{
		case 0://ocean
			return 	Season<double>(1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (10.0 *	_QUARTER_YEAR_IN_S));
		case 1://Lake
			return 	Season<double>(1.0 / (5.0 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-0.1 * 	_QUARTER_YEAR_IN_S));
		case 2://Medow
			return 	Season<double>(1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (10.0 *	_QUARTER_YEAR_IN_S), 1.0 / (0.5 *		_QUARTER_YEAR_IN_S), 1.0 / (-0.1 *	_QUARTER_YEAR_IN_S));
		case 3://Forest
			return 	Season<double>(1.0 / (30.0 *	_QUARTER_YEAR_IN_S), 1.0 / (8.0 *		_QUARTER_YEAR_IN_S), 1.0 / (0.8 *		_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S));
		case 4://Steppe
			return 	Season<double>(1.0 / (0.05 *	_QUARTER_YEAR_IN_S), 1.0 / (-0.05 *	_QUARTER_YEAR_IN_S), 1.0 / (-0.01 *	_QUARTER_YEAR_IN_S), 1.0 / (-0.01 *	_QUARTER_YEAR_IN_S));
		case 5://Desert	
			return 	Season<double>(1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-2.0 *	_QUARTER_YEAR_IN_S), 1.0 / (100.0 *	_QUARTER_YEAR_IN_S));
		case 6://Moor
			return 	Season<double>(1.0 / (80.0 *	_QUARTER_YEAR_IN_S), 1.0 / (4.0 *		_QUARTER_YEAR_IN_S), 1.0 / (4.0 *		_QUARTER_YEAR_IN_S), 1.0 / (-0.1 *	_QUARTER_YEAR_IN_S));
		case 7://tropical Forest	
			return 	Season<double>(1.0 / (0.3 *		_QUARTER_YEAR_IN_S), 1.0 / (0.3 *		_QUARTER_YEAR_IN_S), 1.0 / (0.3 *		_QUARTER_YEAR_IN_S), 1.0 / (0.3 *		_QUARTER_YEAR_IN_S));
		case 8://Ice desert
			return 	Season<double>(1.0 / (1000.0 *	_QUARTER_YEAR_IN_S), 1.0 / (10.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-100.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-0.1 *	_QUARTER_YEAR_IN_S));
		case 9://highlands		
			return 	Season<double>(1.0 / (60 *		_QUARTER_YEAR_IN_S), 1.0 / (16.0 *	_QUARTER_YEAR_IN_S), 1.0 / (16.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S));
		case 10://mountain
			return 	Season<double>(1.0 / (120 *		_QUARTER_YEAR_IN_S), 1.0 / (32.5 *	_QUARTER_YEAR_IN_S), 1.0 / (-17.0 *	_QUARTER_YEAR_IN_S), 1.0 / (-1.0 *	_QUARTER_YEAR_IN_S));
		default:
			return 	Season<double>(1.0 / (2 *		_QUARTER_YEAR_IN_S), 1.0 / (1.0 *		_QUARTER_YEAR_IN_S), 1.0 / (2.0 *		_QUARTER_YEAR_IN_S), 1.0 / (-4.0 *	_QUARTER_YEAR_IN_S));
			break;
		}
	}
	static double getOptimalTempForGrowth(int ID) {
		switch (ID)
		{
		case 0://ocean
			return 20.0;
		case 1://Lake
			return 	15.0;
		case 2://Medow
			return 	25.0;
		case 3://Forest
			return 	25.0;
		case 4://Steppe
			return 	30.0;
		case 5://Desert
			return 	35.0;
		case 6://Moor
			return 	20.0;
		case 7://tropical Forest
			return 	 25.0;
		case 8://Ice desert
			return 	10.0;
		case 9://highlands
			return 	20.0;
		case 10://mountain
			return 	20.0;
		default:
			return 	25.0;
			break;
		}
	}

	static double desideTauDependancy(double one_div_tau_temp, double one_div_tau_season, int ID) {
		bool temp_wachstum = (one_div_tau_temp < 0) ? false : true;
		bool region_season_wachstum = (one_div_tau_season < 0) ? false : true;

		switch (ID)
		{
		case 4://Steppe
			return 	one_div_tau_season;
		case 5://Desert
			return 	one_div_tau_season;
		case 7://tropical Forest
			return 	one_div_tau_season;
		default://ocean//Lake//Medow//Forest//Moor//Ice desert//highlands//mountain//default
			if (temp_wachstum == region_season_wachstum) {
				//beide 1/Tau gehen in die gleiche Richtung
				return (one_div_tau_temp*0.2 + one_div_tau_season*0.8);//todo factor anpassen?
			}
			else {//Die Temperatur entscheidet wie das Wachstum ausieht
				return one_div_tau_temp;
			}
		}
	}

	GroundProperties* getGroundProperties();
};

#endif // !_REGION_

