#ifndef _WORLDVIEW_
#define _WORLDVIEW_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "DeltaWorld.h"
#include "World.h"
#include "Button.h"
#include "GlobalDraw.h"
#include "Menue.h"
#include "TileMap.h"


class WorldView
{
public:
	sf::RenderWindow window;
	sf::View tilemap_view;
	TileMap map;

	sf::Text info_text;
	sf::RectangleShape info_box;

	sf::Vector2i left_upper_corner_of_view;
	float zoom;
	float ratio;
	float one_div_ratio;
	float tliemap_height_ratio;
	sf::Vector2i LastMousePos;
	float delta_wheel_tick;

	bool hasFocus;
	bool majorChange;


	sf::RectangleShape Bottom;
	sf::Text TextVersion;
	sf::Text TextTime;

	int DeltaWorldColoring;
	bool colorChanged;
	int focussed_delta_world;

	GlobalDrawSingleton* _G_;

	int MenueWidth;
	sf::View menue_view;
	bool menu_visible;
	Button ButtonShowMenue;
	Menue MenueWorldAppearance;
	Menue MenueSimulation;
	int charactersize;



	WorldView() {
		this->delta_wheel_tick = 0.0f;
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		int width = sf::VideoMode::getDesktopMode().width;
		int height = sf::VideoMode::getDesktopMode().height;
		this->ratio = (float)height / (float)width;
		this->one_div_ratio = 1.0f / this->ratio;

		this->focussed_delta_world = -1;
		this->menu_visible = true;
		this->majorChange = true;
		

		this->_G_ = &this->_G_->getInstance();

		//this->window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "EvoSym", sf::Style::Default);
		this->window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "EvoSym", sf::Style::Default);


		this->window.setVerticalSyncEnabled(true); // refresh rate synchronized with the vertical frequency of the monitor
		this->window.setPosition(sf::Vector2i(0, 0));

		this->zoom = 1.0;
		this->left_upper_corner_of_view = sf::Vector2i(0,0);
		this->tilemap_view.reset(sf::FloatRect(0,0, (float)height, (float)height));//left,top/width/height
		this->tliemap_height_ratio = (sf::VideoMode::getDesktopMode().height - this->Bottom.getSize().y) / sf::VideoMode::getDesktopMode().height;
		this->tilemap_view.setViewport(sf::FloatRect(0.f, 0.f, 1.0f, this->tliemap_height_ratio));
		this->menue_view.reset(sf::FloatRect(0.0f, 0.0f, (float)width, (float)height));//left,top/width/height
		this->menue_view.setViewport(sf::FloatRect(0.f, 0.f, 1.0f, 1.0f));

		//Menue
		this->charactersize = 10;
		float menueWidth = 150.0f;
		this->MenueWorldAppearance = Menue(menueWidth,0.0f,0.0f,std::string("Aussehen"));
		this->MenueWorldAppearance.addButton(std::string("Region"));
		this->MenueWorldAppearance.addButton(std::string("Höhe"));
		this->MenueWorldAppearance.addButton(std::string("Temperatur"));		
		this->MenueWorldAppearance.addButton(std::string("Jahreszeiten"));
		this->MenueWorldAppearance.addButton(std::string("Klimazone"));
		this->MenueWorldAppearance.addButton(std::string("Ressourcen Pflanzen"));

		this->MenueSimulation = Menue(menueWidth, menueWidth, 0.0f, std::string("Simulation"));
		this->MenueSimulation.addButton(std::string("new World"));
		this->MenueSimulation.addButton(std::string("exit"));

		//set default WorldAppearance
		this->hasFocus = true;
		this->DeltaWorldColoring = 1;//1=region/2=height/3=temp/4=season/5=tempZone/6=resources


		//Bottom
		float Bottomheight = 20.f;

		this->Bottom.setFillColor(sf::Color(0, 0, 0));
		
		this->Bottom.setPosition(sf::Vector2f(0.0f, (float)height - Bottomheight));
		this->Bottom.setSize(sf::Vector2f((float)width, Bottomheight));

		this->ButtonShowMenue.shape.setSize(sf::Vector2f(Bottomheight * 3, Bottomheight));
		this->ButtonShowMenue.shape.setPosition(sf::Vector2f((float)width*0.5f -Bottomheight * 3.0f *0.5f, (float)(height - Bottomheight)));
		this->ButtonShowMenue.setText(std::string("Menue"), this->charactersize);


		this->TextVersion.setString("version 1.3 - Jakob Wandel");
		this->TextVersion.setFillColor(sf::Color(255, 255, 255));
		this->TextVersion.setFont(this->_G_->arial);
		this->TextVersion.setCharacterSize(this->charactersize);
		this->TextVersion.setOrigin(this->TextVersion.getGlobalBounds().width, this->TextVersion.getGlobalBounds().height);
		this->TextVersion.setPosition(sf::Vector2f((float)width,(float)height - this->TextVersion.getGlobalBounds().height));

		this->TextTime.setString("0:00:000:0");
		this->TextTime.setFillColor(sf::Color(255, 255, 255));
		this->TextTime.setFont(this->_G_->arial);
		this->TextTime.setCharacterSize(this->charactersize);
		this->TextTime.setOrigin(0, this->TextTime.getGlobalBounds().height);
		this->TextTime.setPosition(sf::Vector2f(0, height - this->TextVersion.getGlobalBounds().height));

		//infobox
		this->info_box.setFillColor(sf::Color(0, 0, 0));
		this->info_box.setPosition(sf::Vector2f((float)width*0.5f, 0.0f));
		this->info_box.setSize(sf::Vector2f((float)width*0.5f, height - Bottomheight));

		this->info_text.setFillColor(sf::Color(255, 255, 255));
		this->info_text.setFont(this->_G_->arial);
		this->info_text.setCharacterSize(this->charactersize);
		this->info_text.setPosition(this->info_box.getPosition().x + 10, this->info_box.getPosition().y + 10);

	}
	~WorldView() {

	}

	void Update(World& W, bool forceUpdate = false);

	//menue actions
	void SetDeltaWorldColor(int i) {
		if (i != this->DeltaWorldColoring) {
			if (-1 < i) {
				this->DeltaWorldColoring = i;
				this->majorChange = true;
			}

		}
	}
	void menueSimulationAction(int i,World& w) {
		switch (i)
		{
		case 1:
			w.reset();
			break;
		case 2:
			this->window.close();
		default:
			break;
		}
	}
};

#endif // !_WORLDVIEW_

