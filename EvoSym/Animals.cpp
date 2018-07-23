#include "Animals.h"
Animals::Animals(){

}

Animals::~Animals(){

}

void Animals::Update(World& W) {
	BOOST_FOREACH(Animal animal, this->animals) {
		animal.simulate(W.time);
	}
}