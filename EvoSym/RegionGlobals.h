#ifndef _RegionGlobals_
#define _RegionGlobals_
#include "globals.h"
#include "Region.h"
class RegionGlobals {

private:
	Region Regions[_amountRegions];
	RegionGlobals() {
		for (int i = 0; i < _amountRegions; i++) {
			this->Regions[i].setTo(i);
		}		
	}
	// Stop the compiler generating methods of copy the object
	RegionGlobals(RegionGlobals const& copy);            // Not Implemented
	RegionGlobals& operator=(RegionGlobals const& copy); // Not Implemented

public:
	Region* getRegion(int i) {
		if (i < _amountRegions) {
			return &this->Regions[i];
		}
		else {
			return this->getDefaultRegion();//Default meaddow
		}
	}
	Region* getDefaultRegion(double height = 1.0, bool polar = false) {
		if (height > 0) {
			if (polar) {
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
