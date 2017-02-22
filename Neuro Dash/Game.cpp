#include "Game.h"
#include <iostream>

void Game::setPopulation(NEAT::Population* _population, std::list<PlayerActor*>* _actors)
{
	population = _population;
	actors = _actors;
	for (auto& i : (*actors))
	{
		i->setPlayer(new Player(currentLevel.getSpawnpoint()));
	}
	if (manualPlayer != nullptr)
		delete manualPlayer;
	manualPlayer = new Player(currentLevel.getSpawnpoint());
}

void Game::update()
{
	if (restartNextUpdate)
	{
		restartLevel(true);
		restartNextUpdate = false;
	}
	bool allDead = true, oneFinished = false;
	updateManualPlayer();
	if (!manualPlayer->isDead())
		allDead = false;
	if (manualPlayer->isFinished())
		allDead = true;
	for (auto &i : (*actors))
	{
		if (i->hasNetwork())
		{
			i->setInput(currentLevel.getMapViewFromPos(i->getPlayer()->getPlayerPos()));
			i->evaluate();
			if (i->getOutput())
				i->getPlayer()->jump();
			i->getPlayer()->update(&currentLevel);
			if (!i->getPlayer()->isDead())
			{
				allDead = false;
			}
			if (i->getPlayer()->isFinished())
			{
				oneFinished = true;
			}
		}
	}

	//camera
	int lastX = -1;
	int farRightPlayer = -2;
	int counter = 0;
	for (auto &i : (*actors))
	{
		if (i->getPlayer()->getPlayerPos().x > lastX && !i->getPlayer()->isDead())
		{
			lastX = int(i->getPlayer()->getPlayerPos().x + 0.5);
			farRightPlayer = counter;
		}
		counter++;
	}
	if (manualPlayer->getPlayerPos().x > lastX)
	{
		lastX = int(manualPlayer->getPlayerPos().x + 0.5);
		farRightPlayer = -1;
	}
	if (farRightPlayer == -1)
	{
		playerView.setCenter(manualPlayer->getPlayerPos().x, manualPlayer->getPlayerPos().y);
	}
	else if (farRightPlayer != -2)
	{
		std::list<PlayerActor*>::iterator it = actors->begin();
		std::advance(it, farRightPlayer);
		playerView.setCenter((*it)->getPlayer()->getPlayerPos().x, (*it)->getPlayer()->getPlayerPos().y);
	}

	if (allDead)
	{
		restartLevel(true);
	}
	if (oneFinished)
	{
		done = true;
	}
}

void Game::draw(sf::RenderWindow& window)
{
	playerView.setSize(window.getDefaultView().getSize());
	playerView.setViewport(window.getDefaultView().getViewport());
	window.setView(playerView);
	currentLevel.draw(window);
	for (auto &i : (*actors))
		i->getPlayer()->draw(window);
	manualPlayer->draw(window);
}

void Game::restartLevel(bool evolveNetwork)
{
	if (evolveNetwork)
		population->stepGeneration(false);
	for (auto& i : (*actors))
	{
		i->setPlayer(new Player(currentLevel.getSpawnpoint()));
	}
	if (manualPlayer != nullptr)
		delete manualPlayer;
	manualPlayer = new Player(currentLevel.getSpawnpoint());
}

void Game::updateManualPlayer()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		manualPlayer->jump();
	manualPlayer->update(&currentLevel);
}

void Game::loadLevel(std::string file)
{
	currentLevel.loadLevel(file);
	for (auto& i : (*actors))
	{
		i->setPlayer(new Player(currentLevel.getSpawnpoint()));
	}
	if (manualPlayer != nullptr)
		delete manualPlayer;
	manualPlayer = new Player(currentLevel.getSpawnpoint());
}