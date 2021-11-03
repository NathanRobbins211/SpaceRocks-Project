#include "bullet.h"

// CONSTRUCTOR / DECONSTRUCTOR
Bullet::Bullet(SDL_Renderer* renderer, float x, float y)
{
	// Create variables
	int w, h;

	// Set defaults
	active = false;
	speed = 800.0;

	// Create image
	SDL_Surface* surface = IMG_Load("./Assets/Projectile.png");
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// Error message if unable to load image
	if (texture == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", "./Assets/Projectile.png", IMG_GetError());
	}

	// Free surface
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
}

Bullet::~Bullet()
{
	// Destroy texture
	//SDL_DestroyTexture(texture);
}

// Reset bullet
void Bullet::Reset()
{
	active = false;

	// Reset position
	posRect.x = -1000;
	posRect.y = -1000;

	pos_X = posRect.x;
	pos_Y = posRect.y;
}

// Update bullet by passing in deltaTime
void Bullet::Update(float deltaTime)
{
	if (active)
	{
		// Move on screen
		pos_X += (speed * xDir) * deltaTime;
		pos_Y += (speed * yDir) * deltaTime;
		posRect.x = (int)(pos_X + 0.5F);
		posRect.y = (int)(pos_Y + 0.5F);

		// If off screen
		if (posRect.x < 0 || posRect.x > 1024 || posRect.y < 0 || posRect.y > 768)
		{
			// Reset position
			Deactivate();
		}
	}
}

// Draw bullet
void Bullet::Draw(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

void Bullet::Reposition()
{
	if (xDir == 1&& yDir == 0)
	{
		posRect.x += 45;
		posRect.y += 11;
	}
	else if(xDir == -1 && yDir == 0)
	{
		posRect.x -= 8;
		posRect.y += 11;
	}
	else if (xDir == 0 && yDir == 1)
	{
		posRect.x += 20;
		posRect.y += 38;
	}
	else if (xDir == 0 && yDir == -1)
	{
		posRect.x += 18;
		posRect.y -= 16;
	}
	else if (xDir == 1 && yDir == -1)
	{
		posRect.x += 40;
		posRect.y -= 10;
	}
	else if (xDir == -1 && yDir == -1)
	{
		posRect.x -= 6;
		posRect.y += 10;
	}
	else if (xDir == 1 && yDir == 1)
	{
		posRect.x += 40;
		posRect.y += 32;
	}
	else if (xDir == -1 && yDir == 1)
	{
		posRect.x -= 2;
		posRect.y += 32;
	}

	pos_X = posRect.x;
	pos_Y = posRect.y;
}

void Bullet::Deactivate()
{
	// Set defaults
	active = false;
	posRect.x = -3000;
	posRect.y = -3000;
	pos_X = posRect.x;
	pos_Y = posRect.y;
}