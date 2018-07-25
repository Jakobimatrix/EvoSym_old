#include "Animals.h"
Animals::Animals(){

}

Animals::~Animals(){

}

void Animals::Update(World& w) {
	BOOST_FOREACH(Animal animal, this->animals) {
		animal.simulate(w.time);
	}
}