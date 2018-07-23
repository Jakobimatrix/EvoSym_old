/**
* @file WorldView.h
* @brief: This class Manages the Window which displays the world.
* @date 10.07.2017
* @author Jakob Wandel
* @version 1.0
**/
#ifndef _WORLDVIEW_
#define _WORLDVIEW_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <chrono>
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
	sf::RenderWindow window; //the sfml::renderWindow
	sf::View tilemap_view;	 
	TileMap map;

	sf::Text info_text;
	sf::RectangleShape info_box;
	std::chrono::time_point<std::chrono::system_clock> lastClick = std::chrono::system_clock::now();

	sf::Vector2i left_upper_corner_of_view;
	float zoom;
	float ratio;
	float one_div_ratio;
	float tliemap_height_ratio;
	sf::Vector2i last_moude_position;
	float delta_wheel_tick;

	bool has_focus;
	bool major_change;


	sf::RectangleShape bottom;
	sf::Text text_version;
	sf::Text text_time;

	int delta_world_coloring;
	bool color_changed;
	int focussed_delta_world;

	GlobalDrawSingleton* _G_;

	int menue_width;
	sf::View menue_view;
	bool menu_visible;
	Button button_show_menue;
	Menue menue_world_appearance;
	Menue menue_simulation;
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
		this->major_change = true;
		

		this->_G_ = &this->_G_->getInstance();

		//this->window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "EvoSym", sf::Style::Default);
		this->window.create(sf::VideoMode(width, height, desktop.bitsPerPixel), "EvoSym", sf::Style::Fullscreen);


		this->window.setVerticalSyncEnabled(true); // refresh rate synchronized with the vertical frequency of the monitor
		this->window.setPosition(sf::Vector2i(0, 0));

		this->zoom = 1.0;
		this->left_upper_corner_of_view = sf::Vector2i(0,0);
		this->tilemap_view.reset(sf::FloatRect(0,0, (float)height, (float)height));//left,top/width/height
		this->tliemap_height_ratio = (sf::VideoMode::getDesktopMode().height - this->bottom.getSize().y) / sf::VideoMode::getDesktopMode().height;
		this->tilemap_view.setViewport(sf::FloatRect(0.f, 0.f, 1.0f, this->tliemap_height_ratio));
		this->menue_view.reset(sf::FloatRect(0.0f, 0.0f, (float)width, (float)height));//left,top/width/height
		this->menue_view.setViewport(sf::FloatRect(0.f, 0.f, 1.0f, 1.0f));

		//Menue
		this->charactersize = 10;
		float menueWidth = 150.0f;
		this->menue_world_appearance = Menue(menueWidth,0.0f,0.0f,std::string("Aussehen"));
		this->menue_world_appearance.addButton(std::string("Region"));
		this->menue_world_appearance.addButton(std::string("Höhe"));
		this->menue_world_appearance.addButton(std::string("Temperatur"));		
		this->menue_world_appearance.addButton(std::string("Jahreszeiten"));
		this->menue_world_appearance.addButton(std::string("Klimazone"));
		this->menue_world_appearance.addButton(std::string("Ressourcen Pflanzen"));

		this->menue_simulation = Menue(menueWidth, menueWidth, 0.0f, std::string("Simulation"));
		this->menue_simulation.addButton(std::string("new World"));
		this->menue_simulation.addButton(std::string("exit"));

		//set default WorldAppearance
		this->has_focus = true;
		this->delta_world_coloring = 1;//1=region/2=height/3=temp/4=season/5=tempZone/6=resources


		//Bottom
		float Bottomheight = 20.f;

		this->bottom.setFillColor(sf::Color(0, 0, 0));
		
		this->bottom.setPosition(sf::Vector2f(0.0f, (float)height - Bottomheight));
		this->bottom.setSize(sf::Vector2f((float)width, Bottomheight));

		this->button_show_menue.shape.setSize(sf::Vector2f(Bottomheight * 3, Bottomheight));
		this->button_show_menue.shape.setPosition(sf::Vector2f((float)width*0.5f -Bottomheight * 3.0f *0.5f, (float)(height - Bottomheight)));
		this->button_show_menue.setText(std::string("Menue"), this->charactersize);


		this->text_version.setString("version 1.3 - Jakob Wandel");
		this->text_version.setFillColor(sf::Color(255, 255, 255));
		this->text_version.setFont(this->_G_->arial);
		this->text_version.setCharacterSize(this->charactersize);
		this->text_version.setOrigin(this->text_version.getGlobalBounds().width, this->text_version.getGlobalBounds().height);
		this->text_version.setPosition(sf::Vector2f((float)width,(float)height - this->text_version.getGlobalBounds().height));

		this->text_time.setString("0:00:000:0");
		this->text_time.setFillColor(sf::Color(255, 255, 255));
		this->text_time.setFont(this->_G_->arial);
		this->text_time.setCharacterSize(this->charactersize);
		this->text_time.setOrigin(0, this->text_time.getGlobalBounds().height);
		this->text_time.setPosition(sf::Vector2f(0, height - this->text_version.getGlobalBounds().height));

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

	void Update(World& W);

	//menue actions
	void SetDeltaWorldColor(int i) {
		if (i != this->delta_world_coloring) {
			if (-1 < i) {
				this->delta_world_coloring = i;
				this->major_change = true;
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

