/**
* @file mein.cpp
* @brief Main File for the Evolution Symulator. 
* 
1. Tag:
Am Anfang schuf ich die ganze Welt: World* MyWorld = new World(). Aber dort war alles dunkel und leer. Noch lebten keine Menschen, Tiere oder Pflanzen dort. Aber ich war da.
Und ich sprach: "Es soll hell werden!" Da wurde es hell. "Und lass hell bleiben", denn Tag und Nachtzyklen zu simulieren fand ich doof. Ich freute mich über die Vereinfachung.
Und Ich sah dass es gut war.
So ging der erste Tag zu Ende.
2. Tag:
Am nächsten Tag sagte ich: „Über der Erde müsste sich eigendlich der blaue Himmel wölben. Von der Erde sollte Wasserdampf aufsteigen und sich in den Wolken sammeln.“
Doch regionale Klima schienen mir nicht sinnvoll und würden den Rahmen der Simulation sprengen.
So freute ich mich nichts tun zu müssen.
Und ich sah dass es gut war.
So ging der zweite Tag zu Ende.
3. Tag:
Am nächsten Tag sagte ich: „Es soll eine ausgewogenes Verhähltnis von Land und Wasser sein.“ Ich nannte das Land Erde und das wasser Meer. Dann befahl ich: Auf der Erde sollen Büsche, Bäume, Blumen und Gräser wachsen, und die Erde grün und bunt machen. Beziehungsweise, lass einfach nur "Vegetation Wachsen", dass sollte für den Zwech dienlich sein.
Ich freute sich über die bunte Erde und das Wasser.
Und ich sah dass es gut war.
So ging der dritte Tag zu Ende.
4. Tag:
Am nächsten Tag sagte ich: „Den Himmel zu simulieren bringt nix"
Und ich sah dass es gut war.
So ging der vierte Tag zu Ende.
5. und 6. Tag:
Am nächsten Tag sagte Ich: „Im Wasser und in er Luft sollen Tiere leben.“ Und so schuf ich die ersten Prototypen.
Ich freute sich über die Tiere im Wasser und in der Luft. Ich sprach zu ihnen: „Let the evolution begin!“.
Und ich sah dass es gut war.
Dann sagte ich: „Den Menschen lasse ich mal aus der Sym raus. Der macht sowieso nur alles kaputt."
7. Tag:
Am siebten Tag wollte ich mich eignendlich ausruhen und die Symulation laufen lassen. Doch es gab Bugs zu beheben und Finetuning durchzuführen. Eine Welt ist eben nicht in 7 Tagen erschaffen.
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/
#include <iostream> 

#include <string>
#include <math.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include "World.h"
#include "globals.h"
#include "WorldView.h"
#include "TileMap.h"
#include "functions.h"
//#include "Timer.h"



int main()
{
	/*

	Multithreading einzelne funktionen
	#include <boost/thread.hpp>
	#include <boost/bind.hpp>

	boost::thread_group TG;
	TG.create_thread(boost::bind(1,2,3,4...));
	//1 == Referenz auf die Function z.B. &myFunc
	//2 - n == alle variablen, die der Funktion übergeben werden sollen, auch die, die schon als default vordefiniert wurden
	//1 == wenn eine Methode einer klasse aufgerufen werden soll, dann: &class::methode
	//2 == Referenz auf die Classeninstance also
	//class myClass;
	//2 == &myClass
	//3 - n == alle variablen, die der Funktion übergeben werden sollen, auch die, die schon als default vordefiniert wurden

	zum schluss!!

	TG.join_all(); //warte bis alle thread erledigt sind -> alle threads sollten etwa gleich lange dauern und !!! nicht voneinander abhängen

	Multithreading using forloop
	#include <boost/foreach.hpp>

	FOREACH(A,B){
		A.doThings();
	}
	A == Typ und name
	B == Pointer auf erstes Element in /Array/vector etc


	example 1
	std::list<int> list_int(  );
	BOOST_REVERSE_FOREACH(int i, list_int)
	{
		// do something with i
	}

	example 2
	extern std::vector<float> get_vector_float();
	BOOST_FOREACH( float f, get_vector_float() )
	{
	// Note: get_vector_float() will be called exactly once
	}

	exmple 3
	BOOST_FOREACH(DeltaWorld* WorldPart, this->WorldParts) {
		WorldPart->setSeason(this->_G_->_SeasonShift + WorldPart->getPosition().getArg());
		WorldPart->simulate(this->time);
	}
	more examples http://www.boost.org/doc/libs/1_61_0/doc/html/foreach.html
	*/

	//double max = 100;
	//double ist[3] = {0.0, 50.0, 100.0};
	//double Tau[2] = { 10.0, -10.0 };
	//double interpol[4] = { 0.1,0.4,0.5,0.9 };
	//double interpol2[4] = { 0.9,0.6,0.5,0.1 };
	//double dt = 0.1;
	////for (int i = 0; i < 3; i++) {
	////	for (int ii = 0; ii < 2; ii++) {
	////		double t = inverse_cosResurceGeneration(max, Tau[ii], ist[i]);
	////		double newRes = cosResurceGeneration(max, Tau[ii], t + dt);
	////		std::cout << "ist: " << ist[i] << " Tau: " << Tau[ii] << " t + dt: " << t + dt << std::endl;
	////		std::cout << "soll: " << newRes << std::endl << "--------------------" << std::endl;
	////	}
	////}
	////std::getchar();
	////return 0;
	//for (int i = 0; i < 3; i++) {
	//	double _tau = 0;
	//	for (int a = 0; a < 4; a++) {
	//		_tau = 1.0/Tau[0] * interpol[a] + 1.0/Tau[1] * interpol2[a];
	//		_tau = 1.0 / _tau;
	//		double t = 0;
	//		double newRes = ist[i];
	//		if (IsFiniteNumber(_tau)) {
	//			t = inverse_cosResurceGeneration(max, _tau, ist[i]);
	//			newRes = cosResurceGeneration(max, _tau, t + dt);
	//		}
	//				
	//				std::cout << "ist: " << ist[i] << " Tau: " << _tau << " t + dt: " << t + dt <<" verhaltnis: "<< interpol[a]<<":"<<interpol2[a] << std::endl;
	//				std::cout << "soll: " << newRes << std::endl << "--------------------" << std::endl;
	//	}
	//}
	//std::getchar();
	//return 0;
	
	World SimulatedWorld = World(_DEFINED_MAP);

	if (!SimulatedWorld.getIsReady()) {
		return 0;
	}
	WorldView worldview;

	while (worldview.window.isOpen())
	{
		worldview.Update(SimulatedWorld,true);
		SimulatedWorld.Update();
	}
	return 0;
}