#pragma once
#include <SFML\Graphics.hpp>
#include "Level.h"

class Player
{
public:
	Player(sf::Vector2f);
	Player& operator = (const Player& origin) { return Player{ sf::Vector2f{} }; };
	~Player() {};

	void draw(sf::RenderWindow&);
	void update(Level*);

	sf::Vector2f getPlayerPos();
	void setGrounded(bool);
	void kill();
	void jump();
	bool isDead() { return dead; };
	void win();
	bool isFinished() { return finished; }

	static const int diameter = 32;
private:
	sf::Vector2f position = sf::Vector2f{ 48,48 };
	sf::RectangleShape playerRect;
	const float xVelocity = 3.5f;
	const float jumpVelocity = -10.f;
	const float gravity = 0.6f;
	float yVelocity = 0.f;
	bool grounded = false;
	bool dead = false;
	bool finished = false;
};