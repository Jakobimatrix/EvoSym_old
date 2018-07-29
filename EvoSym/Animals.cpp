#include "Animals.h"
Animals::Animals(){

}

Animals::~Animals(){

}

void Animals::Update(World& w) {

	int animal_decayed = 0;
	BOOST_FOREACH(Animal animal, this->animals) {
		animal.simulate(w.time);
		if (animal.isZero()) {
			animal_decayed++;
		}
	}
	std::vector<Animal>::iterator it = animals.begin();
	if (animal_decayed > 0) {
		while (it != animals.end()) {//delete animals which died and have zero nutritiant value
			if (it->isZero()) {
				it = animals.erase(it);
				animal_decayed--;
				if (animal_decayed == 0) {
					break;
				}
			}
			else ++it;
		}
	}
}