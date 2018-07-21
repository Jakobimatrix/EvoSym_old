#include "WorldView.h"

void WorldView::Update(World& W, bool forceUpdate) {
	if (this->window.isOpen()) {
		bool mouse_over_menue = false;
		sf::Event event;
		sf::Vector2i MousePos = sf::Mouse::getPosition(this->window);
		sf::Vector2i MousePos_menue = sf::Mouse::getPosition(this->window);
		MousePos.x = (int)(MousePos.x*this->one_div_ratio);
		sf::Vector2f graphic_to_windowsize_ratio = sf::Vector2f((float)_WORLD_DIAMETER_IN_PIXEL / (float)this->window.getSize().x, (float)_WORLD_DIAMETER_IN_PIXEL / (float)this->window.getSize().y);
		while (this->window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) {
				this->window.close();
			}else if (event.type == sf::Event::GainedFocus) {
				this->hasFocus = true;
			}
			else if (event.type == sf::Event::LostFocus) {
				this->hasFocus = false;
			}
			else if (event.type == sf::Event::Resized)
			{
				//neuer ratio
				//this->ratio = (float)this->window.getSize().y / (float) this->window.getSize().x;
				//this->one_div_ratio = 1.0f / this->ratio;
			}
		}
		if (this->hasFocus) {

			//Menüe hover
			bool hover_over_menue_appearance = false;
			bool hover_over_menue_Simulation = false;
			if (this->menu_visible) {
				hover_over_menue_appearance = this->MenueWorldAppearance.hover(MousePos_menue);
				hover_over_menue_Simulation = this->MenueSimulation.hover(MousePos_menue);
			}
			this->ButtonShowMenue.hover(MousePos_menue);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {//verschieben
				int deltaX = MousePos.x - this->LastMousePos.x;
				int deltaY = MousePos.y - this->LastMousePos.y;
				this->left_upper_corner_of_view.x += (int)((float)deltaX*graphic_to_windowsize_ratio.x*this->zoom);
				this->left_upper_corner_of_view.y += (int)((float)deltaY*graphic_to_windowsize_ratio.y*this->zoom);
				this->majorChange = true;
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {//anklicken
				//appearance Menue
				if (this->menu_visible) {
					if (hover_over_menue_appearance) {
						int buttonNr = this->MenueWorldAppearance.click(MousePos_menue);//change world appearance
						if (buttonNr > -1) {
							if (buttonNr > 0) {
								this->SetDeltaWorldColor(buttonNr);
							}						
						}
						mouse_over_menue = true;
					}
					else if (hover_over_menue_Simulation) {
						int buttonNr = this->MenueSimulation.click(MousePos_menue);//simulation Menü
						if (buttonNr > -1) {
							if (buttonNr > 0) {
								this->menueSimulationAction(buttonNr, W);
								this->majorChange = true;
							}
						}
						mouse_over_menue = true;
					}					
				}
				//showHideMenue
				if (this->ButtonShowMenue.click(MousePos_menue)) {
					this->menu_visible = !this->menu_visible;
					this->majorChange = true;
					mouse_over_menue = true;
				}

				//TileInformation
				if (!mouse_over_menue) {//mouse_over_menue
					int hover_over_tile_x = (int)(((float) MousePos.x * graphic_to_windowsize_ratio.x*this->zoom - (float)this->left_upper_corner_of_view.x)/ (float)_TILE_RESULUTION);
					int hover_over_tile_y = (int)(((float) MousePos.y * graphic_to_windowsize_ratio.y*this->zoom - (float)this->left_upper_corner_of_view.y)/ (float)_TILE_RESULUTION);

					int dw_element = (hover_over_tile_x)*_WORLD_DIMENSION + hover_over_tile_y;
					if (dw_element > -1 && dw_element < _AMOUNT_DELTA_WORLDS) {
						this->focussed_delta_world = dw_element;
					}
					else {
						this->focussed_delta_world = -1;
					}
				}
			}
			//zoom
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				float ratio = 0.9f;

				float _zoome_before = this->zoom;
				this->zoom = this->zoom *  ratio;
				float _zoome_after = this->zoom;

				float Verschiebung = _zoome_after - _zoome_before;
				Verschiebung = Verschiebung * (float)_WORLD_DIAMETER_IN_PIXEL;
				Verschiebung = Verschiebung / 2;

				this->left_upper_corner_of_view = sf::Vector2i(this->left_upper_corner_of_view.x + (int)Verschiebung, this->left_upper_corner_of_view.y + (int)Verschiebung);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				float ratio = 1.1f;

				float _zoome_before =  this->zoom;
				this->zoom = this->zoom *  ratio;
				float _zoome_after =  this->zoom;

				float Verschiebung = _zoome_after - _zoome_before;
				Verschiebung = Verschiebung * (float)_WORLD_DIAMETER_IN_PIXEL;
				Verschiebung = Verschiebung / 2;

				this->left_upper_corner_of_view = sf::Vector2i(this->left_upper_corner_of_view.x + (int)Verschiebung, this->left_upper_corner_of_view.y + (int)Verschiebung);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
				if (this->delta_wheel_tick != event.mouseWheelScroll.delta) {
					float delta = event.mouseWheelScroll.delta - this->delta_wheel_tick;
					this->delta_wheel_tick = event.mouseWheelScroll.delta;

					float ratio = 1.1f;
					if (delta > 0) {
						ratio = 0.9f;
					}
					float _zoome_before = this->zoom;
					this->zoom = this->zoom *  ratio;
					float _zoome_after = this->zoom;

					float Verschiebung = _zoome_after - _zoome_before;
					Verschiebung = Verschiebung * (float)_WORLD_DIAMETER_IN_PIXEL;
					Verschiebung = Verschiebung / 2;

					this->left_upper_corner_of_view = sf::Vector2i(this->left_upper_corner_of_view.x + (int)Verschiebung, this->left_upper_corner_of_view.y + (int)Verschiebung);
				}		
			}
		}

		this->TextTime.setString(W.GetTimeReadable(true, true, true, true, true));


		this->tilemap_view.reset(sf::FloatRect((float)-this->left_upper_corner_of_view.x, (float)-this->left_upper_corner_of_view.y, zoom * (float)_WORLD_DIMENSION * (float)_TILE_RESULUTION * this->one_div_ratio, zoom * _WORLD_DIMENSION * _TILE_RESULUTION));

		this->tilemap_view.setViewport(sf::FloatRect(0.f, 0.f, 1, this->tliemap_height_ratio));
		this->window.setView(this->tilemap_view);

		this->window.clear();

		//DeltaWorldDetail highlight tile
		if (this->focussed_delta_world > -1) {
			W.world_parts[this->focussed_delta_world].setShowInfo(true);
		}
		//int count = 0;
		if (this->majorChange) {
			//redraw the full map (slow)
			this->map.loadAll(W, this->DeltaWorldColoring);
			this->majorChange = false;
			//count = _Amount_Delta_Worlds;
		}
		else {
			//only draw the tiles which changed; 
			//draw only visible Tiles //TODO doesnt work
			//float visible_World_Dimension = std::ceil(zoom*_World_Dimension);

			//int drawFromX = (int)std::ceil((float)this->centerOfView.x / (float)TileResulution);
			//int drawToX = (int)std::ceil(visible_World_Dimension + (float)this->centerOfView.x / (float)TileResulution);
			//int drawFromY = (int)std::ceil((float)this->centerOfView.y / (float)TileResulution);
			//int drawToY = (int)std::ceil(visible_World_Dimension + (float)this->centerOfView.y / (float)TileResulution);

			for (int xi = 0; xi < _WORLD_DIMENSION; xi++) {
				for (int yi = 0; yi < _WORLD_DIMENSION; yi++) {
					//if (drawFromX < xi && xi < drawToX && drawFromY < yi && yi < drawToY) {//draw only visible Tiles //TODO doesnt work
						int i = xi*_WORLD_DIMENSION + yi;
						if (W.world_parts[i].hadChangeInAppearance(this->DeltaWorldColoring)) {
							this->map.updateOne(xi, yi, &W.world_parts[i], this->DeltaWorldColoring);	
						}
					//}
				}
			}
		}
		//reset the highlight of the choosenDeltaWorld
		if (this->focussed_delta_world > -1) {
			W.world_parts[this->focussed_delta_world].setShowInfo(false);
		}
		window.draw(map);
		//set the view to defaut, that teh buttons and menues always stay at the same position

			
		window.setView(this->menue_view);

		//Menue
		if (this->menu_visible) {
			this->MenueWorldAppearance.display(this->window);
			this->MenueSimulation.display(this->window);
		}

		window.draw(this->Bottom);
		window.draw(this->ButtonShowMenue.shape);
		window.draw(this->ButtonShowMenue.text);
		window.draw(this->TextTime);
		window.draw(this->TextVersion);

		//Infobox
		if (this->focussed_delta_world > -1) {
			W.world_parts[this->focussed_delta_world].setShowInfo(false);
			this->info_text.setString(W.world_parts[this->focussed_delta_world].getInfoString());
			window.draw(this->info_box);
			window.draw(this->info_text);
		}

		this->window.display();
		this->colorChanged = false;

				
		this->LastMousePos = MousePos;
	}
};
