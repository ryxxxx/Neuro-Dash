#include "Level.h"
#include <algorithm>
#include <iostream>
#include "Player.h"

void Level::loadLevel(std::string fileLocation)
{
	pugi::xml_document xmlFile;
	xmlFile.load_file(fileLocation.c_str());
	tileWidth = xmlFile.child("map").child("tileset").attribute("tilewidth").as_int();
	tileHeight = xmlFile.child("map").child("tileset").attribute("tileheight").as_int();
	tileCount = xmlFile.child("map").child("tileset").attribute("tilecount").as_int();
	mapWidth = xmlFile.child("map").attribute("width").as_int();
	mapHeight = xmlFile.child("map").attribute("height").as_int();
	std::string textureSource = xmlFile.child("map").child("tileset").child("image").attribute("source").as_string();
	tileSet.loadFromFile(textureSource);
	std::string rawMapString = xmlFile.child("map").child("layer").child("data").text().as_string();
	rawMapString.erase(std::remove(rawMapString.begin(), rawMapString.end(), ','), rawMapString.end());
	rawMapString.erase(std::remove(rawMapString.begin(), rawMapString.end(), '\n'), rawMapString.end());
	tiles.clear();
	tiles.resize(mapWidth*mapHeight);
	tileVertices.setPrimitiveType(sf::Quads);
	tileVertices.resize(mapWidth*mapHeight * 4);
	spawnpoint = sf::Vector2f{ 
		xmlFile.child("map").child("objectgroup").child("object").attribute("x").as_float(),
		xmlFile.child("map").child("objectgroup").child("object").attribute("y").as_float() };
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			sf::Vertex* tile = &tileVertices[(i*mapWidth + j) * 4];
			tile[0].position = sf::Vector2f(j * tileWidth, i * tileHeight);
			tile[1].position = sf::Vector2f((j + 1) * tileWidth, i * tileHeight);
			tile[2].position = sf::Vector2f((j + 1) * tileWidth, (i + 1) * tileHeight);
			tile[3].position = sf::Vector2f(j  * tileWidth, (i + 1) * tileHeight);
			switch (rawMapString[i*mapWidth + j])
			{
			case '1':
				tiles[i*mapWidth + j] = TileType::AIR;
				tile[0].texCoords = sf::Vector2f(0, 0);
				tile[1].texCoords = sf::Vector2f(tileWidth, 0);
				tile[2].texCoords = sf::Vector2f(tileWidth, tileHeight);
				tile[3].texCoords = sf::Vector2f(0, tileHeight);
				break;
			case '2':
				tiles[i*mapWidth + j] = TileType::SOLID;
				tile[0].texCoords = sf::Vector2f(tileWidth, 0);
				tile[1].texCoords = sf::Vector2f(tileWidth*2, 0);
				tile[2].texCoords = sf::Vector2f(tileWidth*2, tileHeight);
				tile[3].texCoords = sf::Vector2f(tileWidth, tileHeight);
				break;
			case '3':
				tiles[i*mapWidth + j] = TileType::SPIKE;
				tile[0].texCoords = sf::Vector2f(tileWidth*2, 0);
				tile[1].texCoords = sf::Vector2f(tileWidth*3, 0);
				tile[2].texCoords = sf::Vector2f(tileWidth*3, tileHeight);
				tile[3].texCoords = sf::Vector2f(tileWidth*2, tileHeight);
				break;
			case '4':
				tiles[i*mapWidth + j] = TileType::GOAL;
				tile[0].texCoords = sf::Vector2f(tileWidth*3, 0);
				tile[1].texCoords = sf::Vector2f(tileWidth * 4, 0);
				tile[2].texCoords = sf::Vector2f(tileWidth * 4, tileHeight);
				tile[3].texCoords = sf::Vector2f(tileWidth*3, tileHeight);
				break;
			}
		}
	}
}

void Level::draw(sf::RenderWindow& window)
{
	window.draw(tileVertices, &tileSet);
	window.draw(overlay);
}

Level::TileType Level::collides(sf::Vector2f position)
{
	bool goal = false;
	bool spike = false;
	bool ground = false;
	sf::FloatRect player = sf::FloatRect(position, sf::Vector2f(static_cast<float>(Player::diameter), static_cast<float>(Player::diameter)));
	for (int i = 0; i < mapWidth; i++)
	{
		for (int j = 0; j < mapHeight; j++)
		{
			
			sf::FloatRect currentTile = sf::FloatRect(static_cast<float>(i*tileWidth), static_cast<float>(j*tileHeight), static_cast<float>(tileWidth), static_cast<float>(tileHeight));
			if (tiles[j*mapWidth + i] == SPIKE)
			{
				currentTile.top += currentTile.height / 2;
				currentTile.height /= 2;
			}
			if (player.intersects(currentTile))
				if (tiles[j*mapWidth + i] == GOAL)
					goal = true;
				else if (tiles[j*mapWidth + i] == SPIKE)
					spike = true;
				else if (tiles[j*mapWidth + i] == SOLID)
					ground = true;
		}
	}
	if (goal)
		return GOAL;
	if (spike)
		return SPIKE;
	if (ground)
		return SOLID;
	return AIR;
}

std::vector<std::vector<Level::TileType>> Level::getMapViewFromPos(sf::Vector2f position)
{
	std::vector<std::vector<Level::TileType>> view;
	for (int i = 0; i < viewWidth; i++)
	{
		std::vector<Level::TileType> column;
		for (int j = 0; j < viewHeight; j++)
		{
			column.push_back(TileType::AIR);
		}
		view.push_back(column);
	}

	int xTileIndex = int(position.x / tileWidth), yTileIndex = int(position.y/tileHeight+0.5);
	int left = std::max(0, xTileIndex), right = std::min(mapWidth, xTileIndex + viewWidth);
	int top = std::max(0, yTileIndex - viewUpMidIncluded), bottom = std::min(mapHeight, yTileIndex + viewDownMidExcluded);
	for (int i = left; i < right; i++)
	{
		for (int j = top; j < bottom; j++)
		{
			view[i - left][j - top] = tiles[i + j*mapWidth];
		}
	}

	overlay.clear();
	overlay.resize(4);
	overlay.setPrimitiveType(sf::PrimitiveType::Quads);
	overlay[0].position = sf::Vector2f(left*tileWidth, top*tileHeight);
	overlay[1].position = sf::Vector2f(right*tileWidth, top*tileHeight);
	overlay[2].position = sf::Vector2f(right*tileWidth, bottom*tileHeight);
	overlay[3].position = sf::Vector2f(left*tileWidth, bottom*tileHeight);
	overlay[0].color = sf::Color(100, 100, 100, 100);
	overlay[1].color = sf::Color(100, 100, 100, 100);
	overlay[2].color = sf::Color(100, 100, 100, 100);
	overlay[3].color = sf::Color(100, 100, 100, 100);
	
	/*for (int i = 0; i < viewHeight;i++)
	{
		for (int j = 0; j < viewWidth;j++)
		{
			std::cout << view[j][i];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/
	
	return view;
}