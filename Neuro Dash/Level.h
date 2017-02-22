#pragma once
#include <SFML\Graphics.hpp>
#include "pugixml\pugixml.hpp"

class Level
{
public:
	Level() {};
	~Level() {};

	void loadLevel(std::string);
	void draw(sf::RenderWindow&);
	int getTileHeight() { return tileHeight; };
	enum TileType {
		AIR,
		SOLID,
		SPIKE,
		GOAL
	};
	TileType collides(sf::Vector2f);
	sf::Vector2f getSpawnpoint() { return spawnpoint; };
	std::vector<std::vector<TileType>> getMapViewFromPos(sf::Vector2f);
	static const int viewWidth = 15, viewHeight = 8;
private:
	sf::Vector2f spawnpoint;
	std::vector<TileType> tiles;
	sf::VertexArray tileVertices, overlay;
	sf::Texture tileSet;
	int tileWidth, tileHeight, tileCount;
	int mapWidth, mapHeight;

	const int viewUpMidIncluded = 5, viewDownMidExcluded = viewHeight - viewUpMidIncluded;
};