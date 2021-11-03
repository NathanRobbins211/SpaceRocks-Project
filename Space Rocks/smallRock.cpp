#include "smallRock.h"

#include <iostream>
#include <cstdlib>
using namespace std;

// CONSTRUCTOR / DECONSTRUCTOR
SmallRock::SmallRock(SDL_Renderer* renderer, float x, float y)
{
	// Create variables
	int w, h;

	// Set defaults
	active = false;
	speed = 400.0;
	rockAngle = 0;

	// Create image
	SDL_Surface* surface = IMG_Load("./Assets/SmallRock.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;
	posRect.h = h;

	// Postiton for movement
	posRect.x = x;
	posRect.y = y;
	pos_X = x;
	pos_Y = y;

	// Move on screen
	xDir = 0;
	yDir = 0;

	// Center of rock sprite
	rockCenter.x = posRect.w / 2;
	rockCenter.y = posRect.h / 2;
}

SmallRock::~SmallRock()
{
	// Destroy texture
	//SDL_DestroyTexture(texture);
}

// Update by passing in deltaTime
void SmallRock::Update(float deltaTime)
{
	if (active)
	{
		// Get new position
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;
		// Adjust for precision loss
		posRect.x = (int)(pos_X + 0.5F);
		posRect.y = (int)(pos_Y + 0.5F);

		rockAngle += 0.1;

		// Wrapping
		if (posRect.x < (0 - posRect.w))		// left of screen
		{
			posRect.x = 1024; pos_X = posRect.x;
		}
		if (posRect.x > 1024)					// right of screen
		{
			posRect.x = (0 - posRect.w); pos_X = posRect.x;
		}
		if (posRect.y < (0 - posRect.w))		// top of screen
		{
			posRect.y = 768; pos_Y = posRect.y;
		}
		if (posRect.y > 768)					// bottom of screen
		{
			posRect.y = (0 - posRect.w); pos_Y = posRect.y;
		}
	}
}

// Draw enemy
void SmallRock::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);
}

void SmallRock::Reposition(float x, float y)
{
	// Set defaults
	active = true;

	// Update position
	posRect.x = x;
	posRect.y = y;
	pos_X = x;
	pos_Y = y;

	// Random speed
	speed = rand() % 2 + 3;
	speed *= 100;

	// Generate direction of movement
	int upOrDown = rand() % 2 + 1;
	int leftOrRight = rand() % 2 + 1;

	if (upOrDown == 1)	// up
	{
		if (leftOrRight == 1)	// move up and left
		{
			xDir = -1; yDir = -1;
		}
		else					// move up and right
		{
			xDir = 1; yDir = -1;
		}
	}
	else				// down
	{
		if (leftOrRight == 1)	// move down and left
		{
			xDir = -1; yDir = 1;
		}
		else					// move down and right
		{
			xDir = 1; yDir = 1;
		}
	}
}

void SmallRock::Deactivate()
{
	// Set defaults
	active = false;
	posRect.x = -2000;
	posRect.y = -2000;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}