// SimplexNoise1234 https://cmaher.github.io/posts/working-with-simplex-noise/
// Copyright © 2003-2011, Stefan Gustavson
//
// Contact: stegu@itn.liu.se
//
// This library is public domain software, released by the author
// into the public domain in February 2011. You may do anything
// you like with it. You may even remove all attributions,
// but of course I'd appreciate it if you kept my name somewhere.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

/** \file
		\brief Declares the SimplexNoise1234 class for producing Perlin simplex noise.
		\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
 * This is a clean, fast, modern and free Perlin Simplex noise class in C++.
 * Being a stand-alone class with no external dependencies, it is
 * highly reusable without source code modifications.
 *
 *
 * Note:
 * Replacing the "float" type with "double" can actually make this run faster
 * on some platforms. A templatized version of SimplexNoise1234 could be useful.
 */

 /*
 Explination:
 The various implementations provide a noise function that takes a coordinate and returns a floating-point noise value between -1 and 1,
 inclusive, where coordinates that are close to each other have similar values.

 example:
 scale = .007 -> a scale of 1 produces a white noise like pattern
 for(i = 0; i < M; ++i):
 for(j = 0; j < N; ++j):
 luminance[i][j] = (simplex_noise(i * scale, j * scale) + 1) / 2.0 * 255.0
 */

#ifndef _SIMPLEXNOISE_
#define _SIMPLEXNOISE_


#include "../functions.h"

class SimplexNoise1234 {

  public:
	  SimplexNoise1234(bool b = false) {
		  if (b) {//generate Rand Pattern
			  for (int i = 0; i < 512; i++) {
				  if (i < 256) {
					  this->permR[i] = uniform_int_dist(0, 255);
				  }
				  else {
					  this->permR[i] = this->permR[i - 256];
				  }			  
			  }
		  }
		else{ //use predefinedPattern
			for (int i = 0; i < 512; i++) {
				this->permR[i] = perm[i];
			}
		}

	}
    ~SimplexNoise1234() {}

/** 1D, 2D, 3D and 4D float Perlin noise
 */
    double SimplexNoise1234::noise( double x );
    double SimplexNoise1234::noise( double x, double y );
    double SimplexNoise1234::noise( double x, double y, double z );
    double SimplexNoise1234::noise( double x, double y, double z, double w );

/** 1D, 2D, 3D and 4D double Perlin noise, with a specified integer period
 */
    double SimplexNoise1234::pnoise( double x, int px );
    double SimplexNoise1234::pnoise( double x, double y, int px, int py );
    double SimplexNoise1234::pnoise( double x, double y, double z, int px, int py, int pz );
    double SimplexNoise1234::pnoise( double x, double y, double z, double w,
                              int px, int py, int pz, int pw );

  private:
	unsigned char SimplexNoise1234::permR[512];
	static  unsigned char SimplexNoise1234::perm[];
    double  SimplexNoise1234::grad( int hash, double x );
    double  SimplexNoise1234::grad( int hash, double x, double y );
    double  SimplexNoise1234::grad( int hash, double x, double y , double z );
    double  SimplexNoise1234::grad( int hash, double x, double y, double z, double t );

};
#endif // !_SIMPLEXNOISE_