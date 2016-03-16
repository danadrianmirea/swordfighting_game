#pragma once
#include "Player.h"
#include "State.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>

std::string exeName;
SDL_Window *win; //pointer to the SDL_Window
SDL_Renderer *ren; //pointer to the SDL_Renderer
SDL_Surface *surface; //pointer to the SDL_Surface
SDL_Texture *playerTex; //pointer to the SDL_Texture
SDL_Texture *floorTex;

SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

Player player1 = Player();
Player player2 = Player();

State idle = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);

State stabHigh = State("stabHigh", 5, 10, 15, 10, 10, 0, 0, 0, 0);
State stabMid = State("stabMid", 5, 10, 15, 10, 10, 0, 0, 0, 0);
State stabLow = State("stabLow" ,5, 10, 15, 10, 10, 0, 0, 0, 0);

State slashHigh = State("slashHigh", 10, 20, 25, 20, 20, 0, 0, 0, 0);
State slashMid = State("slashMid", 10, 20, 25, 20, 20, 0, 0, 0, 0);
State slashLow = State("slashLow", 10, 20, 25, 20, 20, 0, 0, 0, 0);

State blockHigh = State("blockHigh", 2, 0, 0, 0, 0, 0, 1, 100, 25);
State blockMid = State("blockMid", 2, 0, 0, 0, 0, 0, 1, 100, 25);
State blockLow = State("blockLow", 2, 0, 0, 0, 0, 0, 1, 100, 25);

State parryHigh = State("parryHigh", 5, 0, 0, 15, 0, 10, 0, 100, 100);
State parryMid = State("parryMid", 5, 0, 0, 15, 0, 10, 0, 100, 100);
State parryLow = State("parryLow", 5, 0, 0, 15, 0, 10, 0, 100, 100);

bool done = false;

int staminaDelay = 0;
int staminaDrain = 0;
int animTime = 0;

void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-up or key-down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-down and key-up the same within a frame, and we still want something to happen (e.g. jump)
	//  - the alternative is to Poll the current state with SDL_GetKeyboardState

	SDL_Event event; //somewhere to store an event

	//NOTE: there may be multiple events per frame
	while (SDL_PollEvent(&event)) //loop until SDL_PollEvent returns 0 (meaning no more events)
	{
		switch (event.type)
		{
		case SDL_QUIT:
			done = true; //set donecreate remote branch flag if SDL wants to quit (i.e. if the OS has triggered a close event,
							//  - such as window close, or SIGINT
			break;

			//keydown handling - we should to the opposite on key-up for direction controls (generally)
		case SDL_KEYDOWN:
			//Keydown can fire repeatable if key-repeat is on.
			//  - the repeat flag is set on the keyboard event, if this is a repeat event
			//  - in our case, we're going to ignore repeat events
			//  - https://wiki.libsdl.org/SDL_KeyboardEvent
			if (!event.key.repeat)
				switch (event.key.keysym.sym)
				{
					//Player.hitCtrl escape to exit
					case SDLK_ESCAPE: done = true;
						break;
						// player 1 controls
						// TODO: add in all controls and direction keys for attack height
					case SDLK_q: if (player1.stamina >= stabHigh._stamina &&
						player1.state._name == "idle") {
						player1.state = stabHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_w: if (player1.stamina >= slashHigh._stamina &&
						player1.state._name == "idle") {
						player1.state = slashHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_e: if (player1.stamina >= parryHigh._stamina &&
						player1.state._name == "idle") {
						player1.state = parryHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_r: if (player1.stamina >= blockHigh._stamina &&
						player1.state._name == "idle") {
						player1.state = blockHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
						
						// player 2 controls
					case SDLK_p: if (player2.stamina >= stabHigh._stamina &&
						player2.state._name == "idle") {
						player2.state = stabHigh;
						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
					case SDLK_u: if (player2.stamina >= blockHigh._stamina &&
						player2.state._name == "idle") {
						player2.state = blockHigh;
						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
				}
			break;
		case SDL_KEYUP:
			event.key.repeat = true;
			if (event.key.repeat)
				switch (event.key.keysym.sym)
				{
				case SDLK_u: player2.state = idle;
					break;
				}
			break;
		}
	}
}
// end::handleInput[]

void stateCompare()
{
	// idle states
	if (player1.state._name == "stabHigh" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "stabHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "slashHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}
	//attacking states
	if (player1.state._name == "stabHigh" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}


	//blocking states
	if (player1.state._name == "slashHigh" &&
		player2.state._name == "blockHigh" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= (player1.state._damage/4) * 3;
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "blockHigh" &&
		player1.stateTime == player1.state._actionEnd) {
		player1.state = idle; //TODO add playback to idle state from current animation
		// mostly visual change, should have no real game impact
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= (player2.state._damage / 4) * 3;
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "stabHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player2.state = idle;
	}
	
	//TODO: finalise parry system

	//if (player1.state._name == "stabHigh" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashHigh" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}
}
// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{

	staminaDelay++;
	staminaDrain++;
	if (staminaDelay > 20) {
		staminaDelay = 0;
		player1.stamina += player1.staminaGain;
		player2.stamina += player2.staminaGain;
	}
	if (staminaDrain > 5) {
		staminaDrain = 0;
		player1.stamina -= player1.state._staminaDrain;
		player2.stamina -= player2.state._staminaDrain;
	}

	if (player1.stamina >= 50) {
		player1.stamina = 50;
	}
	if (player2.stamina >= 50) {
		player2.stamina = 50;
	}
	if (player1.stamina < 0) {
		player1.stamina = 0;
	}
	if (player2.stamina < 0) {
		player2.stamina = 0;
	}

	player1.inState("stabHigh");
	player1.inState("slashHigh");

	player2.inState("stabHigh");
	stateCompare();
	player1.animUpdate();
	player2.animUpdate();
}

void render()
{
		//First clear the renderer
		SDL_RenderClear(ren);
		//Draw the texture
		SDL_Rect srcPlayer;
		SDL_Rect dstPlayer;

		SDL_Rect srcPlayer2;
		SDL_Rect dstPlayer2;

		srcPlayer.x = player1.xSpriteIndex;
		srcPlayer.y = player1.ySpriteIndex;
		srcPlayer.w = 100;
		srcPlayer.h = 100;

		dstPlayer.x = 200;
		dstPlayer.y = 200;
		dstPlayer.w = 100;
		dstPlayer.h = 100;

		srcPlayer2.x = player2.xSpriteIndex;
		srcPlayer2.y = player2.ySpriteIndex;
		srcPlayer2.w = 100;
		srcPlayer2.h = 100;

		dstPlayer2.x = 50;
		dstPlayer2.y = 200;
		dstPlayer2.w = 100;
		dstPlayer2.h = 100;

		SDL_RenderCopy(ren, playerTex, &srcPlayer, &dstPlayer);
		SDL_RenderCopyEx(ren, playerTex, &srcPlayer2, &dstPlayer2, 0, 0, flip);

		//Update the screen
		SDL_RenderPresent(ren);
}

// based on http://www.willusher.io/sdl2%20tutorials/2013/08/17/lesson-1-hello-world/
int main(int argc, char* args[])
{
	player1.state = idle;
	player2.state = idle;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	std::cout << "SDL initialised OK!\n";

	//create window
	win = SDL_CreateWindow("SDL Hello World!", 100, 100, 384, 384, SDL_WINDOW_SHOWN);

	//error handling
	if (win == nullptr)
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		exit(1);
	}
	std::cout << "SDL CreatedWindow OK!\n";

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr)
	{
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	std::string imagePath = "assets/sprites/stabHigh.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	playerTex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (playerTex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	imagePath = "assets/sprites/floor.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	floorTex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (floorTex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	while (!done) //loop until done flag is set)
	{
		cout << player1.stamina << "         " << player2.stamina << "                   " << player1.health << "         " << player2.health << endl;

		handleInput(); // this should ONLY SET VARIABLES

		updateSimulation(); // this should ONLY SET VARIABLES according to simulation

		render(); // this should render the world state according to VARIABLES

		SDL_Delay(20); // unless vsync is on??
	}


	SDL_DestroyTexture(playerTex);
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
