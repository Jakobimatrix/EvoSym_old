/**
* @file SimulatedUnit.h
* @brief: All things which change over the time within the simulationprogram and thus have to be simulated are a child class of this class.
* To simplify everything and allow multi tasking: All Units within the simulation time delta t are independent of eache other.
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _SIMULATEDUNIT_
#define _SIMULATEDUNIT_
#include "structs.h"
#include "Point2d.h"

class SimulatedUnit{
protected:
	double t0;				//[s] Start time since this unit was created.
	double t_last_update;	//[s] Time when this unit was last updated.
	double delta_t_update;	//[s] After this intervall this unit needs an update.
	Point2d position;		//[m,m] 2 D position of this unit.
	bool has_updated;		//for visualization->if true the unit needs a visual update

public:

	/**
	* @function SimulatedUnit()
	* @brief: default constructor
	**/
	SimulatedUnit() {
		this->delta_t_update = 1.0;
		this->t_last_update = 0.0;
		this->has_updated = true;
	}
	~SimulatedUnit() {}

	/**
	* @function void simulate(double tnow)
	* @brief: To run one simulation step from a child class run this function.
	* @param[in] tnow: The time difference since the simulation started.
	**/
	void simulate(double tnow) {
		if (this->runSimulationStep(tnow)) {
			this->update(tnow);
			this->t_last_update = tnow;
		}
	}

	/**
	* @function getPosition()()
	* @brief: Return the reference of the position of the simulated Unit.
	* @retval Point2d&:	return the reference of the position
	**/
	Point2d& getPosition() { return this->position; }

	/**
	* @function getAge_s(double tnow = -1)
	* @brief: calculates the age of the simulated unit.
	* @param[in] tnow: The time difference since the simulation started.
	* @retval double: age of simulated unit
	**/
	double getAge_s(double tnow = -1) {
		if (tnow < 0) {//if tnow is not avaiable
			return this->t_last_update - this->t0;
		}
		else {
			return tnow - this->t0;
		}
	}

	/**
	* @function wasUpdated()
	* @brief: Checks, if this simulated unit was updated since the last time asked. E.g. for redrawing purpose.
	* @retval bool: True if the unit was updated.
	**/
	bool wasUpdated() {
		if (this->has_updated) {
			this->has_updated = false;
			return true;
		}
		return false;
	}

protected:

	/**
	* @function runSimulationStep(double tnow)
	* @brief: Since different simulated units need to be updated in different time intervalls this function checks wheather this unit needs a update or not.
	* @param[in] tnow: The time difference since the simulation started.
	* @retval bool: Returns true, if this unit needs to be updated.
	**/
	bool runSimulationStep(double tnow) {
		if (delta_t_update > tnow - t_last_update)
		{
			return false;
		}
		else 
		{
			this->has_updated = true;
			return true;
		}
	}

	/**
	* @function virtual update ()
	* @brief: Every child class which inherits from this class needs this update function, which will be called when
	*		 this simulated unit needs to be updated.
	* @param[in] The time difference since the simulation started.
	**/
	virtual void update(double tnow) {};
};
#endif // !_SIMULATEDUNIT_

