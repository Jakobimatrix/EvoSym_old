#ifndef _FUNCTIONS_
#define _FUNCTIONS_
#define NOMINMAX

#include <random>
#include <ctime>
#include <limits>
#include <Windows.h> 
#include <math.h>
#include <sys/types.h>
#include "Point2d.h"


//a - a primary array(which we need to stretch / compress) pointer.
//oldw - primary width
//oldh - primary height
//b - a secondary array(which we get after compressing / stretching) pointer
//neww - secondary width
//newh - seconday height
void resample(void *a, void *b, int oldw, int oldh, int neww, int newh);

// \brief uniform_int_dist(int min, int max);
// Rückgabe:Integer Zufallsvariable einer Gleichverteilung von min und max

//[min]	Anfang der Gleichverteilung
//[max] Ende der Gleichverteilung
int uniform_int_dist(int min, int max);

// \brief randTrueFalse();
// Rückgabe: zufällig true oder false (50/50)
bool randTrueFalse();

// \brief uniform_double_dist(double max, double min);
// Rückgabe:double Zufallsvariable einer Gleichverteilung von min und max

//[min]	Anfang der Gleichverteilung
//[max] Ende der Gleichverteilung
double uniform_double_dist(double max, double min);

// \brief randGaus(double mean, double variance);
// Rückgabe:double Zufallsvariable einer Normalverteilung N(mean,variance^2)

//[mean]		Erwartungswert der Normalverteilten Zufallsvariable
//[variance]	Varianz der Normalverteilten Zufallsvariable
double randGaus(double mean, double variance);

// \brief smallerValue(T a, T b);
// Vergleicht zwei Zahlenwerte (Typ egal aber gleich!)
// Rückgabe: EingabeTyp Kleinere von beiden Werten

//[a]	Zahl a
//[b]	Zahl b
template <typename T>
inline T smallerValue(T a, T b)
{
	if (a<b) {
		return a;
	}
	return b;
}

// \brief largerValue(T a, T b);
// Vergleicht zwei Zahlenwerte (Typ egal aber gleich!)
// Rückgabe: EingabeTyp Größere von beiden Werten

//[a]	Zahl a
//[b]	Zahl b
template <typename T>
inline T largerValue(T a, T b)
{
	if (a>b)
	{
		return a;
	}
	return b;
}

// \brief Phytagoras(T a, T b);
// a^2+b^2=c^2
// Rückgabe: EingabeTyp c^2

//[a]	Zahl a
//[b]	Zahl b
template <typename T>
inline T Phytagoras(T a, T b)
{
	return std::sqrt(a*a + b*b);
}

// \brief Rad2Deg(double rad);
// Konvertierung von Rad in Grad

// [rad] Zahl in Rad
template <typename T>
inline double Rad2Deg(T rad)
{
	//return (180 * (rad / 3.14159));
	return rad*57.29577951;
}

// \brief Deg2Rad(double deg);
// Konvertierung von Grad in Rad

// [deg] Zahl in Grad
template <typename T>
inline double Deg2Rad(T deg)
{
	//return (3.14159 * (deg / 180));
	return deg*0.01745329252;
}


// \brief IsNaN(T x);
// Überprüft ob eine Zahl definiert ist z.B. 0/0 oder inf/inf sind undefiniert

// [x] zu überprüfende Zahl
template <typename T>
inline bool IsNaN(T x)
{
	return (x != x);
}

//// \brief SetMaxLinit(T& x);
//// Überschreibt die übergebene Variable mit dem maximalen Wert, den der Speichertyp annehmen kann.
//template <typename T>
//inline void SetMaxLinit(T& val) {
//	val = std::numeric_limits<T>::max();
//}
//
//// \brief SetMinLinit(T& x);
//// Überschreibt die übergebene Variable mit dem minimalen Wert, den der Speichertyp annehmen kann.
//template <typename T>
//inline void SetMinLinit(T& val) {
//	val = std::numeric_limits<T>::min();
//}

/* Function to get no of set bits in binary
representation of passed binary no. 
http://www.geeksforgeeks.org/count-set-bits-in-an-integer/
*/

template <typename T>
inline unsigned int countSetBits(T n)
{
	unsigned int count = 0;
	while (n)
	{
		n &= (n - 1);
		count++;
	}
	return count;
}

/*
Since true is everything != 0 it returns 1 for true and 0 for false
*/
inline int TrueFalseTo10(bool b) {
	if (b) {
		return 1;
	}
	return 0;
}

//if color is bigger than 255 it will be set to 255, if color is negativ it will be set to 0
template <typename T>
inline void normalizeColor(T &color) {
	if (color > 255) {
		color = 255;
	}
	else if (color < 0) {
		color = 0;
	}
}
/**
* @ double PT1_T_Discrete(double dt, double pt1_T_continous) 
* @brief: Calculates the discrete value for the T constant of a pt1 behaviour.
* @param[in] dt: The time passed since the last update. This should not exceed 5 Days.
* @param[in] pt1_T_continous: The pt1_t constant for a continous system.
**/
inline double PT1_T_Discrete(double dt, double pt1_T_continous) {
	return 1.0 / (pt1_T_continous / dt + 1); //see wikipedia pt1 for time discrete form
}


inline double ReLU(double input) {
	if (input>0)
	{
		return 0.0;
	}
	return input;
}

/**
* @function double getMinDistanceLine(Point2d& LinieBegin, Point2d& LineEnd, Point2d& Position);
* @brief Returns the minimal distance of a point to a Line (segment).
* @param[in] Line:		segment to which the shortest distance shall be calculated
* @param[in] Position:	point from which the shortest distance shall be calculated
* @retval double:	the calculated distance
**/
double getMinDistanceLine(Point2d& LinieBegin, Point2d& LinieEnd, Point2d& Position);

/**
* @function getMinDistanceInfLine(oslam::CLine &Line, SVector2d &Position);
* @brief Returns the minimal distance of a point to a Line (infinite long).
* @param[in] Line:		segment to which the shortest distance shall be calculated
* @param[in] Position:	point from which the shortest distance shall be calculated
* @retval bool:			returns true if x is a valid number
**/
double getMinDistanceInfLine(Point2d& LineBegin, Point2d& LineEnd, Point2d& Position);


/**
* @function bool isPointCinLineAB(oslam::CLine &Line, SVector2d &C, double tollerance);
* @brief Validates if a Point C lies within a tollerance on a line.
* @param[in] Line:	segment on which the point might lie
* @param[in] C:		point which might lie on a Line
* @retval bool:		returns true if the point lies within tollerance to the line
**/
bool isPointCinLineAB(Point2d& A, Point2d& B, Point2d& C, double tollerance);

/**
* @function bool isLineABcrossedByLineCD(oslam::CLine &line1, oslam::CLine &line2, double tollerance);
* @brief Validates if two lines intersect with each other.
* @param[in] line1:			segment which might be intersected by segment2
* @param[in] line2:			segment which might be intersected by segment1
* @param[in] tollerance:	tolleranc for the intersection
* @retval bool:				returns true if both lines intersect witch eachother
**/
bool isLineABcrossedByLineCD(Point2d& A, Point2d& B, Point2d& C, Point2d& D, double tollerance);

/**
* @function Point2d getSchnittpunkt(Point2d& A, Point2d& B, Point2d& C, Point2d& D);
* @brief Calculates the point of intersection of 2 given lines which are interpreted as infinit long lines.
* @param[in] line1:			line1 cuts line2 somewhere
* @param[in] line2:			line2 cuts line1 somewhere
* @retval Point2d:		returns the point of intersection
**/
Point2d getSchnittpunkt(Point2d& A, Point2d& B, Point2d& C, Point2d& D);

/**
* @function double signed_scalar(Point2d &A, Point2d &B)
* @brief A scala multiplication, but the result will be negative if the angle between two vectors exceed 90°.
* @param[in] Point2d &A: First vector for scalar product.
* @param[in] Point2d &B: Second vector for scalar product.
* @retval double: signed scalar product result.
**/
double signed_scalar(Point2d &A, Point2d &B)
{
	double r = A.getR()*B.getR();
	double angle = std::abs(A.getArg() - B.getArg());
	return r * cos(angle);
}

#endif // !_FUNCTIONS_

