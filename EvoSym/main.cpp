/**
* @file mein.cpp
* @brief: Main File for the Evolution Symulator. 
* 
1. Tag:
Am Anfang schuf ich die ganze Welt: World* MyWorld = new World(). Aber dort war alles dunkel und leer. 
Noch lebten keine Menschen, Tiere oder Pflanzen dort. Aber ich war da.
Und ich sprach: "Es soll hell werden!" Da wurde es hell. "Und lass hell bleiben", 
denn Tag und Nachtzyklen zu simulieren fand ich doof. Ich freute mich über die Vereinfachung.
Und Ich sah dass es gut war.
So ging der erste Tag zu Ende.
2. Tag:
Am nächsten Tag sagte ich: „Über der Erde müsste sich eigendlich der blaue Himmel wölben. 
Von der Erde sollte Wasserdampf aufsteigen und sich in den Wolken sammeln.“
Doch regionale Klima schienen mir nicht sinnvoll und würden den Rahmen der Simulation sprengen.
So freute ich mich nichts tun zu müssen.
Und ich sah dass es gut war.
So ging der zweite Tag zu Ende.
3. Tag:
Am nächsten Tag sagte ich: „Es soll eine ausgewogenes Verhähltnis von Land und Wasser sein.“ 
Ich nannte das Land Erde und das wasser Meer. Dann befahl ich: 
Auf der Erde sollen Büsche, Bäume, Blumen und Gräser wachsen, und die Erde grün und bunt machen. 
Beziehungsweise, lass einfach nur "Vegetation Wachsen", dass sollte für den Zwech dienlich sein.
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
Am siebten Tag wollte ich mich eignendlich ausruhen und die Symulation laufen lassen. 
Doch es gab Bugs zu beheben und Finetuning durchzuführen. Eine Welt ist eben nicht in 7 Tagen erschaffen.
* @date 10.04.2017
* @author Jakob Wandel
* @version 1.0
**/

#include "globals.h"
#include "World.h"
#include "WorldView.h"

int main() {

	World SimulatedWorld = World(_DEFINED_MAP);

	if (!SimulatedWorld.getIsReady()) {
		return 0;
	}

	WorldView worldview;

	while (worldview.window.isOpen())
	{
		worldview.Update(SimulatedWorld, true);
		SimulatedWorld.Update();
	}

	return 0;
}