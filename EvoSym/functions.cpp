#include "functions.h"




////////////////////////
// GLOBAL VARIABLES
////////////////////////
std::random_device rd;
std::default_random_engine generator(rd());



int uniform_int_dist(int min, int max)
{
	if (min > max) {
		std::uniform_int_distribution<int> distribution(max, min);
		return distribution(generator);
	}
	else {
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(generator);
	}		
}
bool randTrueFalse()
{
	std::uniform_int_distribution<int> distribution(0, 1);
	if (distribution(generator) == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
double uniform_double_dist(double min, double max)
{
	if (min > max) {
		std::uniform_real_distribution<> distribution(0.0f, 1.0f);
		return max + distribution(generator)*(min - max);
	}
	else {
		std::uniform_real_distribution<> distribution(0.0f, 1.0f);
		return min + distribution(generator)*(max - min);
	}

}

double randGaus(double mean, double variance)
{
	std::normal_distribution<double> distribution(mean, variance);
	return distribution(generator);
}

double expDist(double lambda, double dt)
{
	std::exponential_distribution<> distribution(lambda);
	return distribution(generator)*dt;
}

//https://stackoverflow.com/questions/9570895/image-downscaling-algorithm
//scales a image with bilinear interpolaton
void resample(void *a, void *b, int oldw, int oldh, int neww, int newh)
{
	int i;
	int j;
	int l;
	int c;
	float t;
	float u;
	float tmp;
	float d1, d2, d3, d4;
	u_int p1, p2, p3, p4; /* nearby pixels */
	u_char red, green, blue;

	for (i = 0; i < newh; i++) {
		for (j = 0; j < neww; j++) {

			tmp = (float)(i) / (float)(newh - 1) * (oldh - 1);
			l = (int)floor(tmp);
			if (l < 0) {
				l = 0;
			}
			else {
				if (l >= oldh - 1) {
					l = oldh - 2;
				}
			}

			u = tmp - l;
			tmp = (float)(j) / (float)(neww - 1) * (oldw - 1);
			c = (int)floor(tmp);
			if (c < 0) {
				c = 0;
			}
			else {
				if (c >= oldw - 1) {
					c = oldw - 2;
				}
			}
			t = tmp - c;

			/* coefficients */
			d1 = (1 - t) * (1 - u);
			d2 = t * (1 - u);
			d3 = t * u;
			d4 = (1 - t) * u;

			/* nearby pixels: a[i][j] */
			p1 = *((u_int*)a + (l * oldw) + c);
			p2 = *((u_int*)a + (l * oldw) + c + 1);
			p3 = *((u_int*)a + ((l + 1)* oldw) + c + 1);
			p4 = *((u_int*)a + ((l + 1)* oldw) + c);

			/* color components */
			blue = (u_char)((u_char)p1 * d1 + (u_char)p2 * d2 + (u_char)p3 * d3 + (u_char)p4 * d4);
			green = (u_char)((u_char)(p1 >> 8) * d1 + (u_char)(p2 >> 8) * d2 + (u_char)(p3 >> 8) * d3 + (u_char)(p4 >> 8) * d4);
			red = (u_char)((u_char)(p1 >> 16) * d1 + (u_char)(p2 >> 16) * d2 + (u_char)(p3 >> 16) * d3 + (u_char)(p4 >> 16) * d4);

			/* new pixel R G B  */
			//*((u_int*)b + (i * neww) + j) = (red << 16) | (green << 8) | (blue);

			/* new pixel GREY  */
			*((u_char*)b + (i * neww) + j) = (u_char) std::floor((red + green + blue) / 3.0);
		}
	}
}

double getMinDistanceLine(Point2d& LineBegin, Point2d& LineEnd, Point2d& Position)
{
	//berechnte über Normale d. Hesseschen Normalform den kleinsten Abstand
	//Vorher wird geschaut ob dieser "Kleinste Abstand" auch zwischen dem Anfangs und Endpunkt liegt
	//dazu muss entweder LineBegin.x < Position.x < LineEnd.x sein oder das Gleiche bei y


	bool PointIsInbetween = false;
	if (smallerValue(LineBegin.x, LineEnd.x) < Position.x && largerValue(LineBegin.x, LineEnd.x) > Position.x || smallerValue(LineBegin.y, LineEnd.y) < Position.y && largerValue(LineBegin.y, LineEnd.y) > Position.y)
	{
		PointIsInbetween = true;
	}
	if (PointIsInbetween)
	{
		double x1 = LineBegin.x;
		double y1 = LineBegin.y;
		double x2 = LineEnd.x;
		double y2 = LineEnd.y;
		double x3 = Position.x;
		double y3 = Position.y;
		double nx = -1 * (y1 - y2);
		double ny = (x1 - x2);
		double norm_n = 1 / std::sqrt(nx*nx + ny*ny);
		return std::abs(norm_n*((nx*x3 + ny*y3) - (nx*x1 + ny*y1)));
	}
	else//einer der Endpunkte der Linie sind die nächsten Punkte
	{
		Point2d PDistance1 = LineBegin - Position;
		Point2d PDistance2 = LineEnd - Position;
		double distance1 = std::abs(PDistance1.getR());
		double distance2 = std::abs(PDistance2.getR());
		return smallerValue(distance1, distance2);
	}
}

double getMinDistanceInfLine(Point2d& LineBegin, Point2d& LineEnd, Point2d& Position) {
	//berechnte über Normale d. Hesseschen Normalform den kleinsten Abstand
	double x1 = LineBegin.x;
	double y1 = LineBegin.y;
	double x2 = LineEnd.x;
	double y2 = LineEnd.y;
	double x3 = Position.x;
	double y3 = Position.y;
	double nx = -1 * (y1 - y2);
	double ny = (x1 - x2);
	double norm_n = 1 / std::sqrt(nx*nx + ny*ny);
	return std::abs(norm_n*((nx*x3 + ny*y3) - (nx*x1 + ny*y1)));
}

bool isPointCinLineAB(Point2d& A, Point2d& B, Point2d& C, double tollerance)
{
	if (tollerance<getMinDistanceLine(A, B, C))
	{
		return false;
	}
	return true;
}

bool isLineABcrossedByLineCD(Point2d& A, Point2d& B, Point2d& C, Point2d& D, double tollerance)
{
	Point2d SchnittP = getSchnittpunkt(A, B, C, D);
	if (isPointCinLineAB(A, B, SchnittP, tollerance) && isPointCinLineAB(C, D, SchnittP, tollerance))//Liegt der Schnittpunkt in beiden Linien mit Tolleranz von minimalem Sicherheitsabstand der Drohne
	{
		return true;
	}
	return false;
}

Point2d getSchnittpunkt(Point2d& A, Point2d& B, Point2d& C, Point2d& D)
{
	/*
	In 2D gibt es einen tollen Trick. Du kannst sowohl Geraden als auch Punkte mit 3D-Vektoren beschreiben und ganz einfach mit diesen Vektoren rechnen. Stell Dir dazu folgendes in 3D vor:
	Code:
	/|
	/ |
	(0;0;0)         |  |
	*               |  |
	(Ursprung)      |  |
	| /
	|/(Ebene "z=1")

	Die 2D-Ebene, in der Du die Punkte und Linien beschreibst, ist die "z=1"-Ebene.

	Einen Punkt repräsentierst Du durch einen 3D-Vektor. Stell Dir vor, wie er vom Ursprung ausgehend, in eine Richtung zeigt und damit eine Gerade in 3D definiert. Diese Gerade schneidet die z=1 Ebene in dem Punkt, den du beschreiben willst. Der zum Punkt (x;y) gehörige 3D-Vektor ergibt sich einfach durch anhängen einer 1: (x;y;1.0).

	Eine Gerade repräsentierst Du auch durch einen 3D-Vektor. Stell Dir dazu eine zweite Ebene vor, die durch den Ursprung geht und die z=1 Ebene schneidet. Der Schnitt zweier Ebenen ist die Gerade. Diese zweite Ebene kannst Du auch mit einem 3D-Vektor beschreiben, nämlich den Normalvektor, also der, der senkrecht auf der Ebene steht.

	Der Trick ist nun der, dass das Kreuzprodukt dazu benutzt werden kann, aus 2 Punkten eine Linie zu erzeugen und aus 2 Linien einen Schnittpunkt zu berechnen:
	Code:
	linie1   = cross( punkt1, punkt2 );
	linie2   = cross( punkt3, punkt4 );
	schnittp = cross( linie1, linie2 );

	wobei punkt1..4 und linie1..2 3D-Vektoren sind (Homogene Koordinaten) und "cross" das Kreuzprodukt berechnet. Warum funktioniert das? Naja, die Punkte werden durch 3D-Richtungsvektoren repräsentiert. Es Ergibt sich ein Dreieck mit den Punkten Ursprung, Punkt1 und Punkt2. Mit dem Kreuzprodukt kann der Normalvektor dieses Dreiecks (der Ebene) bestimmt werden. Weiter kannst Du über das Kreuzprodukt zweier Normalvektoren von Ebenen einen 3. Vektor bestimmen, der in beiden Ebenen enthalten ist und den "Schnittpunkt" repräsentiert. In der projektiven Geometrie spricht man hier von einem Dualismus zwischen Punkt und Gerade.

	Die letzte Komponente von schnittp wird nicht mehr 1 sein. Um wieder auf zwei Komponenten zu kommen teilst Du durch die letzte Komponente:

	//https://www.c-plusplus.net/forum/258648-full
	*/

	double p1[3] = { A.x,A.y,1.0 };
	double p2[3] = { B.x,B.y,1.0 };
	double p3[3] = { C.x,C.y,1.0 };
	double p4[3] = { D.x,D.y,1.0 };

	double l1[3], l2[3], s[3];

	l1[0] = p1[1] * p2[2] - p1[2] * p2[1];
	l1[1] = p1[2] * p2[0] - p1[0] * p2[2];
	l1[2] = p1[0] * p2[1] - p1[1] * p2[0];
	l2[0] = p3[1] * p4[2] - p3[2] * p4[1];
	l2[1] = p3[2] * p4[0] - p3[0] * p4[2];
	l2[2] = p3[0] * p4[1] - p3[1] * p4[0];
	s[0] = l1[1] * l2[2] - l1[2] * l2[1];
	s[1] = l1[2] * l2[0] - l1[0] * l2[2];
	s[2] = l1[0] * l2[1] - l1[1] * l2[0];
	double schX = s[0] / s[2];
	double schY = s[1] / s[2];
	return Point2d(schX, schY);
}