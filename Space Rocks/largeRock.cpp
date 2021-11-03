#include "largeRock.h"

#include <iostream>
#include <cstdlib>
using namespace std;

// CONSTRUCTOR / DECONSTRUCTOR
LargeRock::LargeRock(SDL_Renderer* renderer, float x, float y)
{
	// Create variables
	int w, h;

	// Set defaults
	active = false;
	speed = 200.0;
	rockAngle = 0;

	// Create image
	SDL_Surface* surface = IMG_Load("./Assets/BigRock.png");
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

LargeRock::~LargeRock()
{
	// Destroy texture
	//SDL_DestroyTexture(texture);
}

// Update by passing in deltaTime
void LargeRock::Update(float deltaTime)
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
void LargeRock::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopyEx(renderer, texture, NULL, &posRect, rockAngle, &rockCenter, SDL_FLIP_NONE);
}

void LargeRock::Reposition()
{
	// Set defaults
	active = true;

	// Generate location on screen
	int location = rand() % 4 + 1;
	// Generate direction of movement
	int direction = rand() % 2 + 1;

	if (location == 1)		// left
	{
		// Position off screen
		posRect.x = -posRect.w;
		pos_X = posRect.x;

		// Generate Y location
		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_Y = posRect.y;

		if (direction == 1)	// move right and up
		{
			xDir = 1; yDir = -1;
		}
		else				// move right and down
		{
			xDir = 1; yDir = 1;
		}
	}
	else if (location == 2)	// right
	{
		// Position off screen
		posRect.x = 1024;
		pos_X = posRect.x;

		// Generate Y location
		int bottomOfScreen = 768 - (posRect.h * 2);
		posRect.y = rand() % bottomOfScreen + posRect.h;
		pos_Y = posRect.y;

		if (direction == 1)	// move left and up
		{
			xDir = -1; yDir = -1;
		}
		else				// move left and down
		{
			xDir = -1; yDir = 1;
		}
	}
	else if (location == 3)	// top
	{
		// Position off screen
		posRect.y = -posRect.h;
		pos_Y = posRect.y;

		// Generate X location
		int sideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % sideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == 1)	// move down and left
		{
			xDir = -1; yDir = 1;
		}
		else				// move down and right
		{
			xDir = 1; yDir = 1;
		}
	}
	else if (location == 4)	// bottom
	{
		// Position off screen
		posRect.y = 768;
		pos_Y = posRect.y;

		// Generate X location
		int sideOfScreen = 1024 - (posRect.w * 2);
		posRect.x = rand() % sideOfScreen + posRect.w;
		pos_X = posRect.x;

		if (direction == 1)	// move up and left
		{
			xDir = -1; yDir = -1;
		}
		else				// move up and right
		{
			xDir = 1; yDir = -1;
		}
	}
}

void LargeRock::Deactivate()
{
	// Set defaults
	active = false;
	posRect.x = -2000;
	posRect.y = -2000;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}