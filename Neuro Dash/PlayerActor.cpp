#include "PlayerActor.h"

PlayerActor::PlayerActor()
{
}

PlayerActor::~PlayerActor()
{
	delete player;
}

float PlayerActor::getFitness()
{
	if (player == nullptr)
	{
		fitness = 0.f;
	}
	else
	{
		fitness = player->getPlayerPos().x;
		if (player->isFinished())
			fitness += 1000;
	}
	return fitness;
}

void PlayerActor::reset()
{
	fitness = 0.f;
	if (player != nullptr)
		delete player;
}

void PlayerActor::setPlayer(Player* newPlayer)
{
	player = newPlayer;
}

void PlayerActor::setInput(std::vector<std::vector<Level::TileType>>& playerView)
{
	int counter = 0;
	for (int i = 0; i < Level::viewHeight; i++)
	{
		for (int j = 0; j < Level::viewWidth; j++)
		{
			float input = 0.f;
			if (playerView[j][i] == Level::AIR)
				input = 0.f;
			else if (playerView[j][i] == Level::SOLID)
				input = 1.f;
			else if (playerView[j][i] == Level::SPIKE)
				input = -1.f;
			else if (playerView[j][i] == Level::GOAL)
				input = 2.f;
			network->setInput(counter, input);
		}
	}
}

bool PlayerActor::getOutput()
{
	return network->getOutput(0) > 0.f;
}

void PlayerActor::evaluate()
{
	int netSteps = 1;
	for (int i = 0; i < netSteps; i++)
		network->step();
}

Player* PlayerActor::getPlayer()
{
	return player;
}