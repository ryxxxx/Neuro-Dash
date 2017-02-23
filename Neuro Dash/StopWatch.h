#pragma once
#include <SFML\System.hpp>

class StopWatch
{
public:
	StopWatch() {};
	~StopWatch() {};

	void stop();
	void start();
	void reset();
	float getTime();
	bool isPaused() { return paused; };
private:
	sf::Clock clock;
	float timeBuffer = 0.f;
	bool paused = true;
};