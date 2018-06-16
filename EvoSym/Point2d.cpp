#include "Point2d.h"


double Point2d::getR()
{
	return std::sqrt(this->x*this->x + this->y*this->y);
}

double Point2d::getArg()
{
	return std::atan2(this->y, this->x);
}



void Point2d::rotateAroundPoint(Point2d *RotateAround, double PhiRot)
{
	//setze RotateAround rellativ zu Ursprung
	Point2d P = *this-*RotateAround;

	//inPolarkoordinaten
	double R	= P.getR();
	double Phi	= P.getArg();
	//Rotation + setze Punkt wieder zu rellativ zur Ursprungsposition
	this->x = R*std::cos(Phi + PhiRot) + RotateAround->x;
	this->y = R*std::sin(Phi + PhiRot) + RotateAround->y;
}