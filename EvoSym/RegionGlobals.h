#ifndef _RegionGlobals_
#define _RegionGlobals_
#include "globals.h"
#include "Region.h"
#include "TemperateZone.h"

class RegionGlobals {

private:
	Region Regions[_AMOUNT_REGIONS];
	Region uninitilized;
	TemperateZone temperate_zone[_AMOUNT_TEMPERATE_ZONES];

	RegionGlobals() {
		for (int i = 0; i < _AMOUNT_REGIONS; i++) {
			this->Regions[i].setTo(i);
		}	
		uninitilized.setToDefault();
		for (int i = 0; i < _AMOUNT_TEMPERATE_ZONES; i++) {
			this->temperate_zone[i] = TemperateZone(i);
		}
	}
	// Stop the compiler generating methods of copy the object
	RegionGlobals(RegionGlobals const& copy);            // Not Implemented
	RegionGlobals& operator=(RegionGlobals const& copy); // Not Implemented

public:
	Region* getUninitilized() {
		return &this->uninitilized;
	}

	Region* getRegion(int i) {
		return &this->Regions[i]; //NOT FAILSAVE
	}
	Region* getDefaultRegion(double height = 1.0, bool polar = false) {
		if (height > 0) {
			if (!polar) {
				return &this->Regions[2];//Default meaddow
			}
			else {
				return &this->Regions[8];//Default polar
			}
		}
		else {
			return &this->Regions[0];//Default ocean
		}
		
	}
	TemperateZone* getTemperatureZone(int temp_zone) {
		return &this->temperate_zone[temp_zone]; //NOT FAILSAVE
	}
	static int getDefaultRegionID(double height = 1.0, bool polar = false) {
		if (height > 0) {
			if (!polar) {	
				return 2;//Default meaddow
			}
			else {
				return 8;//Default polar
			}			
		}
		else {
			return 0;//Default ocean
		}
	}
	static RegionGlobals& getInstance()
	{
		// The only instance
		// Guaranteed to be lazy initialized
		// Guaranteed that it will be destroyed correctly
		static RegionGlobals instance;
		return instance;
	}
};
#endif // !_RegionGlobals_
