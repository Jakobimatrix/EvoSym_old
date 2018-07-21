#ifndef _TileMap_
#define _TileMap_

#include <SFML\Graphics.hpp>
#include "globals.h"
#include "World.h"

class TileMap : public sf::Drawable, public sf::Transformable
{
private:
	GlobalDrawSingleton* _G_DRAW_;
	GlobalSingleton* _G_;
public:

	TileMap() {
		m_tileset.loadFromFile(_SRC_TILE_IMAGE);
		this->_G_DRAW_ = &this->_G_DRAW_->getInstance();
		this->_G_ = &this->_G_->getInstance();
		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		m_vertices.resize(_WORLD_DIMENSION * _WORLD_DIMENSION * 4);
	}

	bool loadAll(World &W, int representation)
	{
		// populate the vertex array, with one quad per tile
		
		for (unsigned int x = 0; x < _WORLD_DIMENSION; x++){
			for (unsigned int y = 0; y < _WORLD_DIMENSION; y++)
			{
				this->updateOne(x, y, &W.WorldParts[y + x * _WORLD_DIMENSION], representation);
			}
		}

		return true;
	}


	void updateOne(int x, int y, DeltaWorld* DW, int representation) {
		//representation: //1=region/2=height/3=temp/4=season/5=tempZone
		// populate the vertex array, with one quad per tile

		// get a pointer to the current tile's quad
		sf::Vertex* quad = &m_vertices[(x + y * _WORLD_DIMENSION) * 4]; 

		// define its 4 corners
		quad[0].position = sf::Vector2f((float)(x * _TILE_RESULUTION), y * (float)(_TILE_RESULUTION));
		quad[1].position = sf::Vector2f((float)((x + 1) * _TILE_RESULUTION), y * (float)(_TILE_RESULUTION));
		quad[2].position = sf::Vector2f((float)((x + 1) * _TILE_RESULUTION), (float)((y + 1) * _TILE_RESULUTION));
		quad[3].position = sf::Vector2f((float)(x * _TILE_RESULUTION), (float)((y + 1) * _TILE_RESULUTION));

		if (representation == 1) {
			// get the current tile number
			int tileNumber = DW->getRegionId();
			if (DW->GroundFrozen()) {
				tileNumber += 11;
			}

			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / _TILE_RESULUTION);
			int tv = tileNumber / (m_tileset.getSize().x / _TILE_RESULUTION);

			// define its 4 texture coordinates

			quad[0].texCoords = sf::Vector2f((float)(tu * _TILE_RESULUTION), (float)(tv * _TILE_RESULUTION));
			quad[1].texCoords = sf::Vector2f((float)((tu + 1) * _TILE_RESULUTION), (float)(tv * _TILE_RESULUTION));
			quad[2].texCoords = sf::Vector2f((float)((tu + 1) * _TILE_RESULUTION), (float)((tv + 1) * _TILE_RESULUTION));
			quad[3].texCoords = sf::Vector2f((float)(tu * _TILE_RESULUTION), (float)((tv + 1) * _TILE_RESULUTION));

			quad[0].color = sf::Color(255, 255, 255, (sf::Uint8) 255);
			if (DW->showInfo()) {
				quad[0].color = sf::Color(0, 0, 0, (sf::Uint8) 0);
			}
			quad[1].color = quad[0].color;
			quad[2].color = quad[0].color;
			quad[3].color = quad[0].color;
		}
		else {
			int tileNumber = 23;
			// find its position in the tileset texture
			int tu = tileNumber % (m_tileset.getSize().x / _TILE_RESULUTION);
			int tv = tileNumber / (m_tileset.getSize().x / _TILE_RESULUTION);

			// define its 4 texture coordinates

			quad[0].texCoords = sf::Vector2f((float)(tu * _TILE_RESULUTION), (float)(tv * _TILE_RESULUTION));
			quad[1].texCoords = sf::Vector2f((float)((tu + 1) * _TILE_RESULUTION), (float)(tv * _TILE_RESULUTION));
			quad[2].texCoords = sf::Vector2f((float)((tu + 1) * _TILE_RESULUTION), (float)((tv + 1) * _TILE_RESULUTION));
			quad[3].texCoords = sf::Vector2f((float)(tu * _TILE_RESULUTION), (float)((tv + 1) * _TILE_RESULUTION));

		
			if (representation == 2) {
				double RGB_grey = DW->getHeight() + std::abs(_MIN_TERRANE_HEIGHT);//0-Min+Max
				RGB_grey = RGB_grey / (_MAX_TERRANE_HEIGHT - _MIN_TERRANE_HEIGHT);//0-1
				RGB_grey = RGB_grey * 255.0;//0-255
				if (-100 < DW->getHeight() && DW->getHeight() < 100) {//markiere nulllinie
					quad[0].color = sf::Color(255, 0, 0);
				}
				else {
					quad[0].color = sf::Color((sf::Uint8)RGB_grey, (sf::Uint8)RGB_grey, (sf::Uint8) RGB_grey);
				}			
				if (DW->showInfo()) {
					quad[0].color = sf::Color(0, 255, 0, (sf::Uint8) 255);
				}
				quad[1].color = quad[0].color;
				quad[2].color = quad[0].color;
				quad[3].color = quad[0].color;
			}
			else if (representation == 3) {
				double temp = DW->getTemp();
				double R = temp*this->_G_DRAW_->GradientRot + this->_G_DRAW_->RotOffset;
				double G = 0.0;
				double B = temp*this->_G_DRAW_->GradientBlau + this->_G_DRAW_->BlauOffset;

				if (temp > -2 && temp < 2) {
					G = 255.0;
				}
				normalizeColor(R);
				normalizeColor(B);
				quad[0].color = sf::Color((sf::Uint8)R, (sf::Uint8)G, (sf::Uint8)B);
				if (DW->showInfo()) {
					quad[0].color = sf::Color((sf::Uint8)0, 0, 0, (sf::Uint8) 0);
				}
				quad[1].color = quad[0].color;
				quad[2].color = quad[0].color;
				quad[3].color = quad[0].color;
			}
			else if (representation == 4) {
				switch (this->_G_->dominantSeason)
				{
				case 0:
					quad[0].color = sf::Color(102, 255, 51);//spring
					break;
				case 1:
					quad[0].color = sf::Color(0, 153, 0);//summer
					break;
				case 2:
					quad[0].color = sf::Color(255, 102, 0);//autumn
					break;
				case 3:
					quad[0].color = sf::Color(0, 204, 255);//winter
					break;
				default:
					quad[0].color = sf::Color(255, 255, 255);
					break;
				}
				if (DW->showInfo()) {
					quad[0].color = sf::Color(0, 0, 0, (sf::Uint8) 0);
				}
				quad[1].color = quad[0].color;
				quad[2].color = quad[0].color;
				quad[3].color = quad[0].color;
			}
			else if (representation == 5)
			{
				sf::Color Zone[4];
				Zone[0] = sf::Color(255, 255, 255);
				Zone[1] = sf::Color(10, 204, 0);
				Zone[2] = sf::Color(255, 255, 102);
				Zone[3] = sf::Color(255, 0, 0);

				double R = 0;
				double G = 0;
				double B = 0;

				for (int i = 0; i < 4; i++) {
					double influence = DW->getTempZoneInfluence(i);
					R += Zone[i].r*influence;
					G += Zone[i].g*influence;
					B += Zone[i].b*influence;
				}
				quad[0].color = sf::Color((sf::Uint8)R, (sf::Uint8)G, (sf::Uint8)B);
				if (DW->showInfo()) {
					quad[0].color = sf::Color(0, 0, 0, (sf::Uint8) 0);
				}
				quad[1].color = quad[0].color;
				quad[2].color = quad[0].color;
				quad[3].color = quad[0].color;
			}
			else if (representation == 6)
			{
				double plant = DW->getIsPlant();
				double R = 0.0;
				double G = plant*this->_G_DRAW_->resource_green_gradient + this->_G_DRAW_->resource_green_offset;
				double B = 0.0;


				normalizeColor(G);
				quad[0].color = sf::Color((sf::Uint8)R, (sf::Uint8)G, (sf::Uint8)B);
				if (DW->showInfo()) {
					quad[0].color = sf::Color((sf::Uint8)255, 0, 0, (sf::Uint8) 255);
				}
				quad[1].color = quad[0].color;
				quad[2].color = quad[0].color;
				quad[3].color = quad[0].color;
			}
		}
	}

private:
	
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &m_tileset;

		// draw the vertex array
		target.draw(m_vertices, states);
	}

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};
#endif