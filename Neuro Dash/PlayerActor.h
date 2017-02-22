#pragma once
#include "neat\Actor.h"
#include "Player.h"

class PlayerActor : public Neural::Actor
{
public:
	PlayerActor();
	~PlayerActor();

	int getSensors() { return Level::viewWidth*Level::viewHeight; }
	int getActions() { return 1; }

	float getFitness();
	void reset();
	void setPlayer(Player*);
	Player* getPlayer();
	void setInput(std::vector<std::vector<Level::TileType>>&);
	void evaluate();
	bool getOutput();

private:
	float fitness = 0.f;
	Player* player;
};