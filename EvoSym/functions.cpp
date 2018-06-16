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