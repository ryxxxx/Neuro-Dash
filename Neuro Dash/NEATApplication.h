#pragma once
#include "Game.h"
#include "neat\Population.h"
#include "StopWatch.h"
#include "PlayerActor.h"
#include <imgui.h>
#include <imgui-SFML.h>

class NEATApplication
{
public:
	NEATApplication();
	~NEATApplication() {};

	void pauseGame(bool);
	void run();
	void createPopulation(int);
private:
	bool paused = true;
	bool runsFast = true;
	bool pauseOnFinish = true;
	int fpsInSlowMode = 60;
	int drawCallsPerSecond = 30;
	StopWatch timeSinceExperimentStarted;

	sf::Clock drawingTimeClock;
	sf::Clock deltaTimeClock;
	sf::Clock slowModeTimeClock;

	Game game;
	NEAT::Population* population;
	std::list<PlayerActor*> actors;
	sf::RenderWindow window;

	void handleEvents();
	void handleGui();
	void deletePopulation();
	void updateGame();
	void draw();
	void stop();

	char savePopulationString[256] = "";
	char loadPopulationString[256] = "";
	char loadLevelString[256] = "";
	int resetPopulationInt = 0;

	bool done = false;
};

