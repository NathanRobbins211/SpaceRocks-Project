#pragma once
#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
using namespace std;

class Bullet
{
	// Create class variables
public:
	bool active;
	SDL_Texture* texture;
	SDL_Rect posRect;
	float speed;
	float xDir, yDir;
	float pos_X, pos_Y;
	SDL_Rect playerPos;
	SDL_Point bulletCenter;
	SDL_Point playerCenter;

	// CONSTRUCTOR / DECONSTRUCTOR
	Bullet(SDL_Renderer* renderer, float x, float y);
	~Bullet();

	// Reset bullet
	void Reset();

	// Update bullet by passing in deltaTime
	void Update(float deltaTime);

	// Draw bullet
	void Draw(SDL_Renderer* renderer);

	void Reposition();

	void Deactivate();

};