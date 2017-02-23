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
	void loadPopulation(std::string);
	void updateGame();
	void draw();
	void stop();
	void loadExperiment(std::string);
	void createExperiment(
		std::string file,
		int populationSize,
		float weightMutationRate,
		float weightMutationIntensity,
		float weightDisableRate,
		float addWeightMutationRate,
		float addNodeMutationRate,
		float c1,
		float c3,
		float speciationDifference,
		std::string levelFile,
		bool endOnLevelFinish,
		//only if false
		int stopAfterGeneration,
		bool saveFinalPopulation,
		bool saveAverageFitnessCourse,
		bool saveChampionFitnessCourse,
		std::string resultFile);

		char imguiSavePopulation[256] = "";
		char imguiLoadPopulation[256] = "";
		char imguiLoadLevel[256] = "";
		char imguiLoadExperiment[256] = "";
		int imguiResetPopulation = 0;
		char imguiCreateExperiment[256] = "";
		int imguiPopulationSize = 0;
		float imguiWeightMutationRate = 0.f;
		float imguiWeightMutationIntensity = 0.f;
		float imguiWeightDisableRate = 0.f;
		float imguiAddWeightMutationRate = 0.f;
		float imguiAddNodeMutationRate = 0.f;
		float imguiC1 = 0.f;
		float imguiC3 = 0.f;
		float imguiSpeciationDifference = 0.f;
		char imguiLevelFile[256] = "";
		bool imguiEndOnLevelFinish = false;
		int imguiStopAfterGeneration = 0;
		bool imguiSaveFinalPopulation = false;
		bool imguiSaveAverageFitnessCourse = false;
		bool imguiSaveChampionFitnessCourse = false;
		char imguiResultFile[256] = "";

	bool done = false;
};

