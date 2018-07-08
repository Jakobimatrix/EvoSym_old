#ifndef _TEMPERATEZONE_
#define _TEMPERATEZONE_

#include <string.h>
#include "functions.h"
#include "structs.h"
#include "globals.h"

class TemperateZone 
{
private:
	GlobalSingleton* _G_;
	MinMax			latitude;					//Grenzen in denen dieses Klima vorkommt.
	double			OverlapNextTemperateZone;	//Wie viele Breitengrade diese Klimazone in die Nachbarklimazohnen hineinragt.
	double			_negative_1_divided_OverlapNextTemperateZone;//for performance reasons
	Season<double>	MeanTemp;					//mittlere Temperatur Frühling, Sommer, Herbst, Winter.
	Propability		VariationInTemp;			//Abweichung von MeanTemp.
	int				TempZoneID;				//int, welche Zone
	std::string TempZoneName; 

public:
	TemperateZone() {
		this->_G_ = &this->_G_->getInstance();
	}
	TemperateZone(double latitude) {
		this->_G_ = &this->_G_->getInstance();
		if (_G_->_POLAR_latitude.isWithin(latitude)){
			this->setToPolar();
		}else if(_G_->_MODERATE_latitude.isWithin(latitude)){
			this->setToModerate();
		}
		else if (_G_->_SUBTROPE_latitude.isWithin(latitude)) {
			this->setToSubtropical();
		}
		else {
			this->setToTropical();
		}
		this->_negative_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}


	TemperateZone(int num) {
		if (num == 0) {
			this->setToPolar();
		}
		else if (num == 1) {
			this->setToModerate();
		}
		else if (num == 2) {
			this->setToSubtropical();
		}
		else {
			this->setToTropical();
		}
		this->_negative_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}

	std::string getZoneName() {
		return this->TempZoneName;
	}
	int getZoneInt() {
		return this->TempZoneID;
	}
	Season<double>* getSeasonDependentTemp() {
		return &this->MeanTemp;
	}

	Propability* getProbability() {
		return &this->VariationInTemp;
	}
	double getNewYearTempVariation() {
		return this->VariationInTemp.getRand();
	}

	/**
	* @function getTempZoneInfluence(double latitude)
	* @brief returns how much this Temperature zone influences things on the given latitude.
	* @param[in] latitude: Latitude for which the influence schould be given back
	* @retval double: [0,1] 1: latitude is within the temperate zone, 0<retval<1: latitude not within the temperate zone but still has some influence, 0: no influence
	**/
	double getTempZoneInfluence(double latitude) {
		if (this->latitude.isWithin(latitude)) {
			return 1.0;
		}
		double difference;
		if (this->latitude.max < latitude) {
			difference = latitude - this->latitude.max;
		}
		else {
			difference = this->latitude.min - latitude;
		}
		if (difference > this->OverlapNextTemperateZone) {
			return 0.0;
		}
		else {
			return this->_negative_1_divided_OverlapNextTemperateZone*difference + 1;
		}
	}

	void setNextNeigbour(double latitude) {
		if (this->TempZoneID == 0) {
			this->setToModerate();
		}
		else if (this->TempZoneID == 1) {
			if (latitude - this->_G_->_POLAR_latitude.max < this->_G_->_SUBTROPE_latitude.min - latitude) {
				this->setToPolar();
			}
			else {
				this->setToSubtropical();
			}
		}
		else if (this->TempZoneID == 2) {
			if (latitude - this->_G_->_MODERATE_latitude.max < this->_G_->_TROPICAL_latitude.min - latitude) {
				this->setToModerate();
			}
			else {
				this->setToTropical();
			}
		}
		else {
			this->setToSubtropical();
		}
	}

	/**
	* @function getAllTempZoneInfluence(double(&influence)[4], double latitude)
	* @brief returns how much all Temperature zones influence things on the given latitude.
	* @param[in] latitude: Latitude for which the influence schould be given back
	* @param[out] influence: normed values for all 4 zones
	**/
	static void getAllTempZoneInfluence(double(&influence)[4], double latitude) {
		TemperateZone T;
		double Total_Influence = 0;
		for (int i = 0; i < 4; i++) {
			T = TemperateZone(i);
			influence[i] = T.getTempZoneInfluence(latitude);
			Total_Influence += influence[i];
		}
		/*if (Total_Influence < 1) {
			std::cout << "ERROR <TemperateZone.h>: Total_Influence of all TemperateZone is smaller than 1." << std::endl;
		}*/
		if (Total_Influence != 1.0) {
			Total_Influence = 1.0 / Total_Influence;//Normierung
			for (int i = 0; i < 4; i++) {
				influence[i] = influence[i]*Total_Influence;
			}
		}
	}

private:
	void setToPolar() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_POLAR_latitude;
		this->OverlapNextTemperateZone = 8.0;
		//this->MeanTemp = Season<double>(-18.0, -10.0, -15.0, -20.0);//https://de.wikipedia.org/wiki/Datei:Klimadiagramm-Wostok-Antarktis_(Russland)-metrisch-deutsch.png
		this->MeanTemp = Season<double>(-40.0, -30.0, -40.0, -50.0);//https://de.wikipedia.org/wiki/Datei:Klimadiagramm-Wostok-Antarktis_(Russland)-metrisch-deutsch.png
		this->VariationInTemp = Propability(0.0, 1.6);//N(0,5)
		this->TempZoneName = "polar";
		this->TempZoneID = 0;
	}
	void setToModerate() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_MODERATE_latitude;
		this->OverlapNextTemperateZone = 4.0;
		this->MeanTemp = Season<double>(7.0, 25.0, 13.0, -6.0);
		this->VariationInTemp = Propability(0.0, 4.5);
		this->TempZoneName = "moderate";
		this->TempZoneID = 1;
	}
	void setToSubtropical() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_SUBTROPE_latitude;
		this->OverlapNextTemperateZone = 8.0;
		this->MeanTemp = Season<double>(20.0, 30.0, 20.0, 15.0);//sydney https://de.climate-data.org/location/24/
		this->VariationInTemp = Propability(0.0, 2.1);
		this->TempZoneName = "subtropical";
		this->TempZoneID = 2;
	}
	void setToTropical() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_TROPICAL_latitude;
		this->OverlapNextTemperateZone = 4.0;
		this->MeanTemp = Season<double>(30.0, 30.0, 30.0, 30.0);
		this->VariationInTemp = Propability(0, 1.3);
		this->TempZoneName = "tropical";
		this->TempZoneID = 3;
	}
};

#endif // !_TEMPERATEZONE_

