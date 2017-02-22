#include "StopWatch.h"

StopWatch::StopWatch()
{
}

StopWatch::~StopWatch()
{
}

float StopWatch::getTime()
{
	if (paused)
		return timeBuffer;
	else
		return timeBuffer + clock.getElapsedTime().asSeconds();
}

void StopWatch::stop()
{
	if (!paused)
	{
		paused = true;
		timeBuffer += clock.getElapsedTime().asSeconds();
	}
}

void StopWatch::start()
{
	if (paused)
	{
		paused = false;
		clock.restart();
	}
}

void StopWatch::reset()
{
	paused = true;
	timeBuffer = 0.f;
}