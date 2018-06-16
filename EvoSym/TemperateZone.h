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
	double			_1_divided_OverlapNextTemperateZone;//for performance reasons
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
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
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
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
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


	double getTempZoneInfluence(double latitude) {
		if (this->latitude.isWithin(latitude)) {
			return 1.0;
		}
		else {
			double difference = smallerValue(abs(latitude - this->latitude.max), abs(this->latitude.min -latitude));
			if (difference > this->OverlapNextTemperateZone) {
				return 0.0;
			}
			else {
				return this->_1_divided_OverlapNextTemperateZone*difference + 1;
			}
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

	static void getAllTempZoneInfluence(double(&influence)[4], double latitude) {
		TemperateZone T;
		double Total_Influence = 0;
		for (int i = 0; i < 4; i++) {
			T = TemperateZone(i);
			influence[i] = T.getTempZoneInfluence(latitude);
			Total_Influence += influence[i];
		}
		Total_Influence = 1/Total_Influence;
		if (Total_Influence != 1) {
			for (int i = 0; i < 4; i++) {
				influence[i] = influence[i]*Total_Influence;
			}
		}
	}

private:
	void setToPolar() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_POLAR_latitude;
		this->OverlapNextTemperateZone = 10.0;
		//this->MeanTemp = Season<double>(-18.0, -10.0, -15.0, -20.0);//https://de.wikipedia.org/wiki/Datei:Klimadiagramm-Wostok-Antarktis_(Russland)-metrisch-deutsch.png
		this->MeanTemp = Season<double>(-40.0, -25.0, -40.0, -50.0);//https://de.wikipedia.org/wiki/Datei:Klimadiagramm-Wostok-Antarktis_(Russland)-metrisch-deutsch.png
		this->VariationInTemp = Propability(0.0, 10.0);
		this->TempZoneName = "polar";
		this->TempZoneID = 0;
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}
	void setToModerate() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_MODERATE_latitude;
		this->OverlapNextTemperateZone = 15.0;
		this->MeanTemp = Season<double>(6.0, 18.0, 7.0, -5.0);//Berlin
		this->VariationInTemp = Propability(0.0,5.0);
		this->TempZoneName = "moderate";
		this->TempZoneID = 1;
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}
	void setToSubtropical() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_SUBTROPE_latitude;
		this->OverlapNextTemperateZone = 10.0;
		this->MeanTemp = Season<double>(19.0, 24.0, 22.0, 18.0);//sydney https://de.climate-data.org/location/24/
		this->VariationInTemp = Propability(0.0, 3.0);
		this->TempZoneName = "subtropical";
		this->TempZoneID = 2;
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}
	void setToTropical() {
		this->_G_ = &this->_G_->getInstance();
		this->latitude = this->_G_->_TROPICAL_latitude;
		this->OverlapNextTemperateZone = 20.0;
		this->MeanTemp = Season<double>(30.0, 30.0, 30.0, 30.0);
		this->VariationInTemp = Propability(0, 1.5);
		this->TempZoneName = "tropical";
		this->TempZoneID = 3;
		this->_1_divided_OverlapNextTemperateZone = -1 / OverlapNextTemperateZone;
	}
};

#endif // !_TEMPERATEZONE_

