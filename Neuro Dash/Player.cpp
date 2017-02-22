#include "Player.h"

Player::Player(sf::Vector2f _position)
{
	position = _position;
	playerRect.setSize(sf::Vector2f(diameter, diameter));
	playerRect.setFillColor(sf::Color(0, 100, 0, 125));
}

sf::Vector2f Player::getPlayerPos()
{
	return position;
}

void Player::draw(sf::RenderWindow& window)
{
	window.draw(playerRect);
}

void Player::update(Level* level)
{
	
	if (!dead && !finished)
	{
		position.x += xVelocity;
		Level::TileType collisionTypeX = level->collides(getPlayerPos());
		if (collisionTypeX == Level::GOAL)
		{
			win();
			return;
		}
		else if (collisionTypeX == Level::SOLID || collisionTypeX == Level::SPIKE)
		{
			kill();
			return;
		}
		position.y += yVelocity;
		Level::TileType collisionTypeY = level->collides(getPlayerPos());
		if (collisionTypeX == Level::GOAL)
		{
			win();
			return;
		}
		else if (collisionTypeY == Level::SPIKE)
		{
			kill();
		}
		else if (collisionTypeY == Level::SOLID)
		{
			if (yVelocity < 0.f)
			{
				kill();
				return;
			}
			else
			{
				position.y = static_cast<float>(static_cast<int>(position.y / level->getTileHeight())*level->getTileHeight());
				setGrounded(true);
			}
		}
		else
		{
			yVelocity += gravity;
			setGrounded(false);
		}
		playerRect.setPosition(position);
	}
}

void Player::setGrounded(bool _grounded)
{
	grounded = _grounded;
	if(grounded)
		yVelocity = 0.f;
}

void Player::kill()
{
	playerRect.setFillColor(sf::Color(100, 0, 0, 125));
	dead = true;
	playerRect.setPosition(position);
}

void Player::jump()
{
	if (grounded)
		yVelocity = jumpVelocity;
}

void Player::win()
{
	playerRect.setFillColor(sf::Color(0, 0, 100, 125));
	finished = true;
	playerRect.setPosition(position);
}