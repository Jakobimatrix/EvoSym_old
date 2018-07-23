#ifndef _ANIMALS_
#define _ANIMALS_

#include <boost/foreach.hpp>
#include <vector>
#include "World.h"
#include "Animal.h"

class Animals
{
private:
	std::vector<Animal> animals;
public:
	Animals();
	~Animals();
	void Update(World& W);


};


#endif // !_ANIMALS_
