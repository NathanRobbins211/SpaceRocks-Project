#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

class LargeRock
{
	// Create class variables
public:
	bool active;
	SDL_Texture* texture;
	SDL_Rect posRect;
	float xDir, yDir;
	float speed;
	float pos_X, pos_Y;
	SDL_Point rockCenter;
	float rockAngle;

	// CONSTRUCTOR / DECONSTRUCTOR
	LargeRock(SDL_Renderer* renderer, float x, float y);
	~LargeRock();

	// Update enemy by passing in deltaTime
	void Update(float deltaTime);

	// Draw enemy
	void Draw(SDL_Renderer* renderer);

	void Reposition();

	void Deactivate();
};