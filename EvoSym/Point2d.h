#ifndef _POINT2D_
#define _POINT2D_

#include <cmath>
#include <SFML\Graphics.hpp>

class Point2d
{
public:
	double x;
	double y;

	Point2d(double x, double y, bool polar = false)
	{
		if (polar) {
			this->x = x*std::cos(y);
			this->y = x*std::sin(y);
		}
		else {
			this->x = x;
			this->y = y;
		}
	}

	Point2d()
	{
		this->x = 0;
		this->y = 0;
	}
	~Point2d(){}
	
	// \brief getR();
	// calculates the radius (poolar coordinate) of this Point
	double Point2d::getR();

	// \brief getArg();
	// calculates the argument (poolar coordinate) of this Point in RAD
	//		90°
	//180°	 |
	//  -----------0°
	//-180°  |
	//		-90°
	//
	double Point2d::getArg();

	bool operator==(Point2d &Pcompare)
	{
		if (Pcompare.x == this->x && Pcompare.y == this->y)
		{
			return true;
		}
		return false;
	}

	bool operator==(const Point2d &Pcompare)
	{
		if (Pcompare.x == this->x && Pcompare.y == this->y)
		{
			return true;
		}
		return false;
	}

	bool operator!=(Point2d &Pcompare)
	{
		return !(*this == Pcompare);
	}

	bool operator!=(const Point2d &Pcompare)
	{
		return !(*this == Pcompare);
	}
	Point2d operator-(Point2d &other)
	{
		Point2d returnVal = *this;
		returnVal.x = returnVal.x - other.x;
		returnVal.y = returnVal.y - other.y;
		return returnVal;
	}

	Point2d operator-(const Point2d &other)
	{
		Point2d returnVal = *this;
		returnVal.x = returnVal.x - other.x;
		returnVal.y = returnVal.y - other.y;
		return returnVal;
	}
	Point2d operator+(Point2d &other)
	{
		Point2d returnVal = *this;
		returnVal.x = returnVal.x + other.x;
		returnVal.y = returnVal.y + other.y;
		return returnVal;
	}

	Point2d operator+(const Point2d &other)
	{
		Point2d returnVal = *this;
		returnVal.x = returnVal.x + other.x;
		returnVal.y = returnVal.y + other.y;
		return returnVal;
	}

	//scalar product
	double operator*(Point2d &other)
	{
		double r = this->getR()*other.getR();
		double angle = std::abs(this->getArg() - other.getArg());
		return r * std::abs(cos(angle));
	}

	//scalar stretch vector
	Point2d operator*(double stretch)
	{
		Point2d returnVal = *this;
		x = x*stretch;
		y = y*stretch;
		return returnVal;
	}

	// \brief rotateAroundPoint(Point2d *RotateAround, double Phi);
	// Rotiert this->Class Punkt um "Phi" um den Punkt "*RotateAround"

	// [*RotateAround]	Punkt um den rotiert werden soll
	// [Phi]			Winkel um den rotiert werden soll. In Rad.
	void Point2d::rotateAroundPoint(Point2d *RotateAround, double Phi);

	// \breif PointToPointDistance(Point *p1, Point *p2
	// Berechnet den Abstand zweier Punkte [*p1] und [*p2]
	static double PointToPointDistance(Point2d *p1, Point2d *p2)
	{
		double x = p2->x - p1->x;
		double y = p2->y - p1->y;
		return std::sqrt(x*x + y*y);
	}
	sf::Vector2f getSFVectorf() {
		return sf::Vector2f((float) this->x, (float) this->y);
	}

};


#endif // !_POINT2D_

