/**
* @file Skills.h
* @brief: This class file contains all Skills which an animal can have. 
* @date 10.07.2017
* @author Jakob Wandel
* @version 0.0 experimental
**/
#ifndef _SKILLS_
#define _SKILLS_

class Skills {
public:

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	Skills() {
		this->energyconsumption = 0;
		this->difficulty = 0;
	}

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	Skills(double energyconsumption, double difficulty) {
		this->energyconsumption = energyconsumption;
		this->difficulty = difficulty;
	}

protected:
	double energyconsumption;
	double difficulty;


};

class NoSkill : public Skills{

};

class Climb : public Skills {

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool possible(double strength, double weight) {
		if (strength*difficulty > weight) {
			return true;
		}
		return false;
	}
};

class Dig : public Skills {

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool possible(double strength, bool frozen) {//frozen how thic the ice is
		if (frozen) {
			return false;
		}
		else {
			return true;
		}
	}
};

class Swim : public Skills {

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool possible(double weight, double size) {
		//dichte Wasser 1 g/cm³ == 1000 kg/m^3
		//approx Volumen animal: Zylinder: size*pi*r^2 mit r=1/10 size
		if (size*3.14*(0.07*size)*(0.07*size) * 1000 > weight) {
			return true;
		}
		else {
			return false;
		}
	}
};

class Dive : public Skills {

};

class Fly : public Skills {

	/**
	* @function int f()
	* @brief
	* @param[in] name:
	* @param[out] name:
	* @retval int:
	**/
	bool possible(double weight, double size) {
		//dichte Wasser 1 g/cm³ == 1000 kg/m^3
		//approx Volumen animal: Zylinder: size*pi*r^2 mit r=7/100*size
		if (size*3.14*(0.07*size)*(0.07*size) * 50 > weight) {
			return true;
		}
		else {
			return false;
		}
	}
};


#endif // _SKILLS_

