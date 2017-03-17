#pragma once
#include "Player.h"
#include "Level.h"
#include "neat\Population.h"
#include "PlayerActor.h"

class Game
{
public:
	Game() {};
	~Game() {};

	void setPopulation(NEAT::Population*, std::list<PlayerActor*>*);
	void update();
	void draw(sf::RenderWindow&);
	void restartLevel(bool);
	bool isDone() { return done; };
	void setDone(bool _done) { done = _done; restartNextUpdate = true; };
	void updateManualPlayer();
	void loadLevel(std::string);
	int getGeneration() { return generation; };
private:
	Level currentLevel;
	sf::View playerView;
	NEAT::Population* population;
	std::list<PlayerActor*>* actors;
	bool done = false;
	Player* manualPlayer;
	bool restartNextUpdate = false;
	int generation = 0;
};