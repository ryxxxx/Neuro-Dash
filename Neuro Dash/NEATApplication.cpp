#include "NEATApplication.h"

NEATApplication::NEATApplication()
{
}

void NEATApplication::run()
{
	createPopulation(20);
	game.setPopulation(population, &actors);

	game.loadLevel("testmap.tmx");
	window.create(sf::VideoMode(980, 532), "Neuro Dash", sf::Style::Close);

	ImGui::SFML::Init(window);

	while (!done)
	{
		if ((runsFast || slowModeTimeClock.getElapsedTime().asSeconds() > 1.f / fpsInSlowMode) && !paused)
		{
			updateGame();
			slowModeTimeClock.restart();
		}

		handleEvents();

		if (drawingTimeClock.getElapsedTime().asSeconds() > 1.f / drawCallsPerSecond)
		{
			handleGui();
			draw();

			drawingTimeClock.restart();
		}
	}
#
	ImGui::SFML::Shutdown();
	deletePopulation();
}

void NEATApplication::pauseGame(bool shouldBePaused)
{
	paused = shouldBePaused;
	if (paused)
		timeSinceExperimentStarted.stop();
	else
		timeSinceExperimentStarted.start();
}

void NEATApplication::createPopulation(int size)
{
	deletePopulation();
	for (int i = 0; i < size; i++)
		actors.push_back(new PlayerActor());
	population = new NEAT::Population((std::list<Neural::Actor*>*)(&actors));

	population->weightMutationRate = 0.80f;
	population->weightMutationIntensity = 0.5f;
	population->addWeightMutationRate = 0.5f;
	population->addNodeMutationRate = 0.3f;
	population->c1 = 1.0f;
	population->c3 = 0.4f;
	population->speciationDifference = 1.0f;
}

void NEATApplication::deletePopulation()
{
	if (population != nullptr)
		delete population;
	for (auto& i : actors)
	{
		if (i != nullptr)
			delete i;
	}
	actors.clear();
}

void NEATApplication::handleEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			stop();
		ImGui::SFML::ProcessEvent(event);
	}
}

void NEATApplication::updateGame()
{
	game.update();
	if (game.isDone())
	{
		if (pauseOnFinish)
		{
			pauseGame(true);
		}
		else
		{
			game.restartLevel(true);
		}
		game.setDone(false);
	}
}

void NEATApplication::handleGui()
{
	ImGui::SFML::Update(window, deltaTimeClock.restart());
	ImGui::Begin("Controls:");
	std::string timeDisplay = "Current time in seconds: " + std::to_string(timeSinceExperimentStarted.getTime());
	ImGui::Text(timeDisplay.c_str());
	std::string maxChampFitness = "Current max champ Fitness: " + std::to_string(population->getMaxChampFitness());
	ImGui::Text(maxChampFitness.c_str());
	std::string maxPopFitness = "Last population Fitness (may update slower than the game): " + std::to_string(population->getPopulationFitness());
	ImGui::Text(maxPopFitness.c_str());
	std::string speciesCount = "Current species count: " + std::to_string(population->getSpeciesCount());
	ImGui::Text(speciesCount.c_str());
	ImGui::Checkbox("Pause on finish", &pauseOnFinish);
	ImGui::DragInt("Draw calls per second", &drawCallsPerSecond, 1.f, 10, 120);
	if (paused)
	{
		if (ImGui::Button("Start"))
		{
			pauseGame(false);
		}
	}
	else
	{
		if (ImGui::Button("Stop"))
		{
			pauseGame(true);
		}
	}
	if (runsFast)
	{
		if (ImGui::Button("Slow"))
		{
			runsFast = !runsFast;
		}
	}

	else
	{
		if (ImGui::Button("Fast"))
		{
			runsFast = !runsFast;
		}
		ImGui::DragInt("Updates per second", &fpsInSlowMode, 1.f, 1, 120);
	}
	if (ImGui::Button("Save population"))
	{
		ImGui::OpenPopup("Save population");
	}
	if (ImGui::BeginPopup("Save population"))
	{
		ImGui::InputText("File Name", imguiSavePopulation, 256);
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			strcpy_s(imguiSavePopulation, "");
		}
		ImGui::SameLine();
		if (ImGui::Button("Save"))
		{
			population->saveNetworks(imguiSavePopulation);
			ImGui::CloseCurrentPopup();
			strcpy_s(imguiSavePopulation, "");
		}

		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load population"))
	{
		ImGui::OpenPopup("Load population");
	}
	if (ImGui::BeginPopup("Load population"))
	{
		ImGui::InputText("File Name", imguiLoadPopulation, 256);
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			strcpy_s(imguiLoadPopulation, "");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			loadPopulation(imguiLoadPopulation);
			ImGui::CloseCurrentPopup();
			strcpy_s(imguiLoadPopulation, "");
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset population"))
	{
		ImGui::OpenPopup("Reset population");
	}
	if (ImGui::BeginPopup("Reset population"))
	{
		ImGui::InputInt("Population size", &imguiResetPopulation);
		if (imguiResetPopulation <= 3)
			imguiResetPopulation = 4;
		if (imguiResetPopulation >= 1000)
			imguiResetPopulation = 999;
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			imguiResetPopulation = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			createPopulation(imguiResetPopulation);
			game.setPopulation(population, &actors);
			game.restartLevel(false);
			ImGui::CloseCurrentPopup();
			timeSinceExperimentStarted.reset();
			if (!paused)
				timeSinceExperimentStarted.start();
			imguiResetPopulation = 0;
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Load level"))
	{
		ImGui::OpenPopup("Load level");
	}
	if (ImGui::BeginPopup("Load level"))
	{
		ImGui::InputText("File Name", imguiLoadLevel, 256);
		if (ImGui::Button("Cancel"))
		{
			strcpy_s(imguiLoadLevel, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			game.loadLevel(imguiLoadLevel);
			strcpy_s(imguiLoadLevel, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	if (ImGui::Button("Create experiment"))
	{
		ImGui::OpenPopup("Create experiment");
	}
	if (ImGui::BeginPopup("Create experiment"))
	{
		ImGui::InputText("File Name", imguiCreateExperiment, 256);
		ImGui::InputInt("Population size", &imguiPopulationSize);
		ImGui::InputFloat("Weight mutation rate", &imguiWeightMutationRate);
		ImGui::InputFloat("Weight mutation intensity", &imguiWeightMutationIntensity);
		ImGui::InputFloat("Weight disable rate", &imguiWeightDisableRate);
		ImGui::InputFloat("Add weight mutation rate", &imguiAddWeightMutationRate);
		ImGui::InputFloat("Add node mutation rate", &imguiAddNodeMutationRate);
		ImGui::InputFloat("c1", &imguiC1);
		ImGui::InputFloat("c3", &imguiC3);
		ImGui::InputFloat("Speciation difference", &imguiSpeciationDifference);
		ImGui::InputText("Level file", imguiLevelFile, 256);
		ImGui::Checkbox("End experiment on level finish", &imguiEndOnLevelFinish);
		if (!imguiEndOnLevelFinish)
			ImGui::InputInt("Stop after generation number", &imguiStopAfterGeneration);
		ImGui::Checkbox("Save final population", &imguiSaveFinalPopulation);
		ImGui::Checkbox("Save course of average fitness", &imguiSaveAverageFitnessCourse);
		ImGui::Checkbox("Save course of champion fitness", &imguiSaveChampionFitnessCourse);
		ImGui::InputText("Result file", imguiResultFile, 256);

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Create"))
		{
			createExperiment(
				imguiCreateExperiment,
				imguiPopulationSize,
				imguiWeightMutationRate,
				imguiWeightMutationIntensity,
				imguiWeightDisableRate,
				imguiAddWeightMutationRate,
				imguiAddNodeMutationRate,
				imguiC1,
				imguiC3,
				imguiSpeciationDifference,
				imguiLevelFile,
				imguiEndOnLevelFinish,
				imguiStopAfterGeneration,
				imguiSaveFinalPopulation,
				imguiSaveAverageFitnessCourse,
				imguiSaveChampionFitnessCourse,
				imguiResultFile);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::SameLine();
	if (ImGui::Button("Load experiment"))
	{
		ImGui::OpenPopup("Load experiment");
	}
	if (ImGui::BeginPopup("Load experiment"))
	{
		ImGui::InputText("File Name", imguiLoadExperiment, 256);
		if (ImGui::Button("Cancel"))
		{
			strcpy_s(imguiLoadExperiment, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			loadExperiment(imguiLoadExperiment);
			strcpy_s(imguiLoadExperiment, "");
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
	ImGui::End();
}

void NEATApplication::draw()
{
	window.clear(sf::Color::White);
	game.draw(window);
	ImGui::Render();
	window.display();
}

void NEATApplication::stop()
{
	done = true;
	window.close();
}

void NEATApplication::loadPopulation(std::string file)
{
	pugi::xml_document doc;
	doc.load_file(file.c_str());
	createPopulation(doc.child("population").attribute("nnetworkCount").as_int());
	population->loadNetworks(file);
	game.setPopulation(population, &actors);
	game.restartLevel(false);
}

void NEATApplication::loadExperiment(std::string file)
{

}

void NEATApplication::createExperiment(
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
	std::string resultFile)
{

}