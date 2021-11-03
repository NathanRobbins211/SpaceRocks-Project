#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <time.h>
using namespace std;

// Boolean for game loop
bool quit = false;

// Init deltaTime (for frame rate)
float deltaTime = 0.0F;
int thisTime = 0;
int lastTime = 0;

// Create movement variables
float x, y, playerAngle, oldAngle;	// angles
float pos_X, pos_Y;					// precision loss
float xDir, xDirOld;				// direction
float yDir, yDirOld;

// Variables for player
float  playerSpeed = 400.0F;	// speed
SDL_Rect playerPos;				// position
SDL_Point center;				// player center

// Declare audio and font
Mix_Chunk* laser;
Mix_Chunk* explosion;
Mix_Music* bgm;
TTF_Font* font;
SDL_Color colorP1 = { 0, 255, 0, 255 };

// Bullet info
#include "bullet.h"
vector<Bullet> bulletList;

void CreateBullet()
{
	for (int i = 0; i < bulletList.size(); i++)
	{
		// If bullet is inactive (able to be used)
		if (bulletList[i].active == false)
		{
			// Play sound
			Mix_PlayChannel(-1, laser, 0);
			// Activate bullet, set position
			bulletList[i].active = true;
			bulletList[i].posRect.x = pos_X; bulletList[i].posRect.y = pos_Y;
			bulletList[i].pos_X = pos_X; bulletList[i].pos_Y = pos_Y;
			bulletList[i].xDir = xDirOld; bulletList[i].yDir = yDirOld;
			bulletList[i].Reposition();
			break;
		}
	}
}

// LargeRock info
#include "largeRock.h"
vector<LargeRock> largeRockList;

// SmallRock info
#include "smallRock.h"
vector<SmallRock> smallRockList;

int main(int argc, char* argv[]) {

	// Seed random numbers
	srand(time(NULL));

	// Declare a pointer, declare a renderer var, initialize SDL2
	SDL_Window* window;
	SDL_Renderer* renderer = NULL;
	SDL_Init(SDL_INIT_EVERYTHING);

	// Create an application window (with the following settings:)
	window = SDL_CreateWindow(
		"Space Rocks",				// window title
		SDL_WINDOWPOS_UNDEFINED,	// initial x pos
		SDL_WINDOWPOS_UNDEFINED,	// initial y pos
		1024,						// width (in pixels)
		768,						// height (in pixels)
		SDL_WINDOW_OPENGL			// flags - see below
	);

	// Check if the window was successfully created
	if (window == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// --- Create background image:
	SDL_Surface* surface = IMG_Load("./Assets/Background.png");	// create an SDL surface
	SDL_Texture* bkgd;											// create background texture
	bkgd = SDL_CreateTextureFromSurface(renderer, surface);		// place surface into the texture
	SDL_FreeSurface(surface);
	// Create rectangle for the background position and set vars
	SDL_Rect bkgdPos;
	bkgdPos.x = 0;
	bkgdPos.y = 0;
	bkgdPos.w = 1024;
	bkgdPos.h = 768;

	//--- Create player image:
	surface = IMG_Load("./Assets/Player.png");					// create an SDL surface
	SDL_Texture* player;										// create background texture
	player = SDL_CreateTextureFromSurface(renderer, surface);	// place surface into the texture
	SDL_FreeSurface(surface);
	// Set rectangle for the player position and set vars
	playerPos.x = 1024 / 2;
	playerPos.y = 768 / 2;
	playerPos.w = 49;	//100
	playerPos.h = 32;	//61

	// Center of player sprite, move direction (player graphic points right, so xDirOld = 1 for bullets to go correct dir)
	center.x = playerPos.w / 2; center.y = playerPos.h / 2;
	pos_X = playerPos.x; pos_Y = playerPos.y;
	xDir = 1; yDir = 0;
	xDirOld = 1; yDirOld = 0;

	// SDL event to handle input
	SDL_Event event;

	// Create Bullets then place in list
	for (int i = 0; i < 10; i++)
	{
		Bullet tempBullet(renderer, -1000.0F, -1000.0F);	// create object
		bulletList.push_back(tempBullet);					// add to list
	}
	//// Create LargeRock then place in list
	//for (int i = 0; i < 1; i++)
	//{
	//	LargeRock tempLargeRock(renderer, -1000.0F, -1000.0F);	// create object
	//	largeRockList.push_back(tempLargeRock);					// add to list
	//}
	//// Create SmallRock then place in list
	//for (int i = 0; i < 2; i++)
	//{
	//	SmallRock tempSmallRock(renderer, -1000.0F, -1000.0F);	// create object
	//	smallRockList.push_back(tempSmallRock);					// add to list
	//}

	// Initialize audio playback, load sounds
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	laser = Mix_LoadWAV("./Assets/Laser.wav");
	explosion = Mix_LoadWAV("./Assets/Explosion.aiff");

	// --- LEVEL UP (defaults)
	int level = 1;
	int playerScore = 0;
	int numberOfLargeRocks = 1;
	int numberOfSmallRocks = 2;
	int totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;
	int totalRocksDestroyed = 0;

	// Create LargeRock then place in list
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		LargeRock tempLargeRock(renderer, -1000.0F, -1000.0F);	// create object
		largeRockList.push_back(tempLargeRock);					// add to list
	}
	// Create SmallRock then place in list
	for (int i = 0; i < numberOfSmallRocks; i++)
	{
		SmallRock tempSmallRock(renderer, -1000.0F, -1000.0F);	// create object
		smallRockList.push_back(tempSmallRock);					// add to list
	}

	// Activate rocks for level 1
	for (int i = 0; i < numberOfLargeRocks; i++)
	{
		largeRockList[i].Reposition();
	}
	cout << "You have reached Level " << level << endl;

	// START GAME LOOP
	while (!quit)
	{
		// Create deltatime
		thisTime = SDL_GetTicks();
		deltaTime = (float)(thisTime - lastTime) / 1000;
		lastTime = thisTime;

		// Check for input
		if (SDL_PollEvent(&event))
		{
			// Close window by the window exit (X) button
			if (event.type == SDL_QUIT)
			{
				quit = true; break;
			}

			switch (event.type)
			{
				// Look for a keypress and release / type
			case SDL_KEYUP:
				switch (event.key.keysym.sym) {
					// Check for the space bar
				case SDLK_SPACE:
					CreateBullet();

					break;

					// Check for the z key (respawn LargeROck)
				/*case SDLK_z:
					largeRockList[0].Reposition();
					break;*/

				//	// Check for the s key (place SmallRocks where LargeRock is, delete LargeRocks)
				//case SDLK_s:
				//	for (int i = 0; i < 2; i++)
				//	{
				//		if (smallRockList[i].active == false)
				//		{
				//			smallRockList[i].Reposition(largeRockList[0].posRect.x, largeRockList[0].posRect.y);
				//		}
				//	}
				//	largeRockList.clear();
				//	break;

					// Check for the a key (reset)
				//case SDLK_a:
				//	smallRockList.clear();
				//	largeRockList.clear();
				//	// Create LargeRock then place in list
				//	for (int i = 0; i < 1; i++)
				//	{
				//		LargeRock tempLargeRock(renderer, -1000.0F, -1000.0F);	// create object
				//		largeRockList.push_back(tempLargeRock);					// add to list
				//	}
				//	// Create SmallRock then place in list
				//	for (int i = 0; i < 2; i++)
				//	{
				//		SmallRock tempSmallRock(renderer, -1000.0F, -1000.0F);	// create object
				//		smallRockList.push_back(tempSmallRock);					// add to list
				//	}
				//	break;

				default:
					break;
				}
			}
		}

		// --- PLAYER MOVEMENT
			// Get keyboard state, check which arrows are pressed for xDir, check which arrows are pressed for yDir
		const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
		if (currentKeyStates[SDL_SCANCODE_LEFT])
		{
			xDir = -1.0F;
		}
		else if (currentKeyStates[SDL_SCANCODE_RIGHT])
		{
			xDir = 1.0F;
		}
		else
		{
			xDir = 0.0F;
		}
		if (currentKeyStates[SDL_SCANCODE_UP])
		{
			yDir = -1.0F;
		}
		else if (currentKeyStates[SDL_SCANCODE_DOWN])
		{
			yDir = 1.0F;
		}
		else
		{
			yDir = 0.0F;
		}

		// --- UPDATE
			// Player update if moving
		if (xDir != 0 || yDir != 0)
		{
			x = playerPos.x - xDir; y = playerPos.y - yDir; // get X and Y
			playerAngle = atan2(yDir, xDir) * 180 / 3.14;	// calculate angle
			oldAngle = playerAngle;							// update old direction
			xDirOld = xDir; yDirOld = yDir;
		}
		else
		{
			// Update old angle
			oldAngle = playerAngle;
		}

		// Get player's new position, adjust for precision loss
		pos_X += (playerSpeed * xDir) * deltaTime;
		pos_Y += (playerSpeed * yDir) * deltaTime;
		playerPos.x = (int)(pos_X + 0.5F);
		playerPos.y = (int)(pos_Y + 0.5F);

		// Player wrapping
		if (playerPos.x < (0 - playerPos.w))	// left of screen
		{
			playerPos.x = 1024; pos_X = playerPos.x;
		}
		if (playerPos.x > 1024)					// right of screen
		{
			playerPos.x = (0 - playerPos.w); pos_X = playerPos.x;
		}
		if (playerPos.y < (0 - playerPos.w))	// top of screen
		{
			playerPos.y = 768; pos_Y = playerPos.y;
		}
		if (playerPos.y > 768)					// bottom of screen
		{
			playerPos.y = (0 - playerPos.w); pos_Y = playerPos.y;
		}

		// Update bulletList
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Update(deltaTime);
			}
		}
		// Update largeRockList
		for (int i = 0; i < largeRockList.size(); i++)
		{
			if (largeRockList[i].active == true)
			{
				largeRockList[i].Update(deltaTime);
			}
		}
		// Update smallRockList
		for (int i = 0; i < smallRockList.size(); i++)
		{
			if (smallRockList[i].active == true)
			{
				smallRockList[i].Update(deltaTime);
			}
		}

		// --- COLLISION
			// Check for collision with bullets and LargeRocks
			// Scroll through all the player's bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				// Check all LargeRocks against active bullet
				for (int j = 0; j < largeRockList.size(); j++)
				{
					if (SDL_HasIntersection(&bulletList[i].posRect, &largeRockList[j].posRect))
					{
						Mix_PlayChannel(-1, explosion, 0);
						int smallRockCounter = 0; playerScore += 50;
						cout << "You have " << playerScore << " points" << endl;

						// Create 2 SmallRocks and reset LargeRock and Bullet
						for (int i = 0; i < smallRockList.size(); i++)
						{
							if (smallRockList[i].active == false)
							{
								smallRockList[i].Reposition(largeRockList[j].posRect.x, largeRockList[j].posRect.y);
								smallRockCounter++;
							}

							// If 2 inactive SmallRocks exist, exit loop
							if (smallRockCounter == 2)
							{
								break;
							}
						}
						largeRockList[j].Deactivate(); bulletList[i].Deactivate();
						totalRocksDestroyed++;

						/*cout << "Total: " << totalNumberOfRocks << endl;
						cout << "Destroyed: " << totalRocksDestroyed << endl;
						cout << "-- Large: " << numberOfLargeRocks << endl;
						cout << "-- Small: " << numberOfSmallRocks << endl << endl;*/
					}
				}
			}
		}

		// Check for collision with bullets and SmallRocks
			// Scroll through all the player's bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				// Check all SmallRocks against active bullet
				for (int j = 0; j < smallRockList.size(); j++)
				{
					if (SDL_HasIntersection(&bulletList[i].posRect, &smallRockList[j].posRect))
					{
						Mix_PlayChannel(-1, explosion, 0);
						smallRockList[j].Deactivate();
						bulletList[i].Deactivate();
						totalRocksDestroyed++; playerScore += 100;
						cout << "You have " << playerScore << " points" << endl;

						// Check if all rocks are destroyed (increase rocks for next level)
						if (totalRocksDestroyed >= totalNumberOfRocks)
						{
							level++;
							totalRocksDestroyed = 0;
							numberOfLargeRocks++; numberOfSmallRocks += 2;
							totalNumberOfRocks = numberOfLargeRocks + numberOfSmallRocks;
							largeRockList.clear();
							smallRockList.clear();
							cout << "You have reached Level " << level << endl;
							cout << "You have " << playerScore << " points" << endl;

							// Create LargeRock then place in list
							for (int i = 0; i < numberOfLargeRocks; i++)
							{
								LargeRock tempLargeRock(renderer, -1000.0F, -1000.0F);	// create object
								largeRockList.push_back(tempLargeRock);					// add to list
							}
							// Create SmallRock then place in list
							for (int i = 0; i < numberOfSmallRocks; i++)
							{
								SmallRock tempSmallRock(renderer, -1000.0F, -1000.0F);	// create object
								smallRockList.push_back(tempSmallRock);					// add to list
							}

							// Activate all LargeRocks
							for (int i = 0; i < numberOfLargeRocks; i++)
							{
								largeRockList[i].Reposition();
							}
						}
					}
				}
			}
		}

		// --- DRAW
		SDL_RenderClear(renderer);						// clear the old buffer
		SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);	// draw bkgd

		// Draw Bullets
		for (int i = 0; i < bulletList.size(); i++)
		{
			if (bulletList[i].active == true)
			{
				bulletList[i].Draw(renderer);
			}
		}
		// Draw LargeRocks
		for (int i = 0; i < largeRockList.size(); i++)
		{
			if (largeRockList[i].active == true)
			{
				largeRockList[i].Draw(renderer);
			}
		}
		// Draw SmallRocks
		for (int i = 0; i < smallRockList.size(); i++)
		{
			if (smallRockList[i].active == true)
			{
				smallRockList[i].Draw(renderer);
			}
		}

		SDL_RenderCopyEx(renderer, player, NULL, &playerPos, playerAngle, &center, SDL_FLIP_NONE);	// draw player

		// Draw new info to the screen
		SDL_RenderPresent(renderer);

	} // END GAME LOOP

	// Close and destroy window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();

	return 0;
}