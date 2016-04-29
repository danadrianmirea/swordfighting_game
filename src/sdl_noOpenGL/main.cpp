#pragma once
#include "Player.h"
#include "State.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <time.h>
#include <dos.h>
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
SDL_Texture *staminaTex;
SDL_Texture *healthTex;

SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;
//dafuq dis shit
Player player1 = Player();
Player player2 = Player();
State stabHigh = State("stabHigh", 5, 10, 36, 40, 20, 0, 5, 0, 0, 0, 1600, 7000);
State stabMid = State("stabMid", 5, 10, 36, 40, 20, 0, 5, 0, 0, 0, 2000, 7000);
State stabLow = State("stabLow", 5, 10, 36, 40, 20, 0, 5, 0, 0, 0, 2400, 7000);

State slashHigh = State("slashHigh", 10, 20, 50, 80, 40, 0, 10, 0, 0, 0, 400, 10000);
State slashMid = State("slashMid", 10, 20, 50, 80, 40, 0, 10, 0, 0, 0, 800, 10000);
State slashLow = State("slashLow", 10, 20, 50, 80, 40, 0, 10, 0, 0, 0, 1200, 10000);

State blockHigh = State("blockHigh", 2, 0, 2, 0, 0, 0, 2, 100, 25, 1000, 2800, 1000);
State blockMid = State("blockMid", 2, 0, 2, 0, 0, 0, 2, 100, 25, 1000, 3200, 1000);
State blockLow = State("blockLow", 2, 0, 2, 0, 0, 0, 2, 100, 25, 1000, 3600, 1000);

State parryHigh = State("parryHigh", 5, 0, 0, 45, 0, 10, 0, 100, 100, 0, 4000, 5000);
State parryMid = State("parryMid", 5, 0, 0, 45, 0, 10, 0, 100, 100, 0, 4400, 5000);
State parryLow = State("parryLow", 5, 0, 0, 45, 0, 10, 0, 100, 100, 0, 4800, 5000);
State idle = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7000);

void aiResponse(State success, float successRate);
void aiAttack(State success, float successRate);

bool done = false;

bool p1Up = false;
bool p1Down = false;
bool p2Up = false;
bool p2Down = false;

int staminaDelay = 0;
int animTime = 0;
int floorCount = 0;
int floorTimer = 0;

int random = 0;
int aiStateHold = 0;

int width = 1000;
int height = 375;


// TODO: add title screen
// TODO: add control screen/ on screen controls
// TODO: death end state
// TODO: victory animations
// TODO: finalise parry system
// TODO: Change the Version to 1.0 once the game is complete
 


void handleInput()
{
	//Event-based input handling
	//The underlying OS is event-based, so **each** key-p1Up or key-p1Down (for example)
	//generates an event.
	//  - https://wiki.libsdl.org/SDL_PollEvent
	//In some scenarios we want to catch **ALL** the events, not just to present state
	//  - for instance, if taking keyboard input the user might key-p1Down two keys during a frame
	//    - we want to catch based, and know the order
	//  - or the user might key-p1Down and key-p1Up the same within a frame, and we still want something to happen (e.g. jump)
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

			//keydown handling - we should to the opposite on key-p1Up for direction controls (generally)
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
						
					case SDLK_q: if (player1.stamina >= stabHigh._stamina &&
						player1.state._name == "idle") {
						player1.animReset();

						if (p1Up == true) {
							player1.state = stabHigh;
							aiResponse(blockHigh, player2.blockHighChance);
						}
						else if (p1Down == true) {
							player1.state = stabLow;
							aiResponse(blockLow, player2.blockLowChance);
						}
						else {
							player1.state = stabMid;
							aiResponse(blockMid, player2.blockMidChance);
					}

						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_w: if (player1.stamina >= slashHigh._stamina &&
						player1.state._name == "idle") {
						player1.animReset();

						if (p1Up == true)
							player1.state = slashHigh;
						else if (p1Down == true)
							player1.state = slashLow;
						else
							player1.state = slashMid;

						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_e: if (player1.stamina >= parryHigh._stamina &&
						player1.state._name == "idle") {
						player1.animReset();
						player1.state = parryHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_r: if (player1.stamina >= blockHigh._stamina &&
						player1.state._name == "idle") {
						player1.animReset();

						if (p1Up == true)
							player1.state = blockHigh;
						else if (p1Down == true)
							player1.state = blockLow;
						else
							player1.state = blockMid;

						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
						
						// player 2 controls
					case SDLK_p: if (player2.stamina >= stabHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						
						if (p2Up == true) {
							player2.state = stabHigh;
						}
						else if (p2Down == true) {
							player2.state = stabLow;
						}
						else {
							player2.state = stabMid;
						}

						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
					case SDLK_o: if (player2.stamina >= slashHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						
						if (p2Up == true)
							player2.state = slashHigh;
						else if (p2Down == true)
							player2.state = slashLow;
						else
							player2.state = slashMid;

						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
					case SDLK_u: if (player2.stamina >= blockHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						
						if (p2Up == true)
							player2.state = blockHigh;
						else if (p2Down == true)
							player2.state = blockLow;
						else
							player2.state = blockMid;

						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
					case SDLK_x: p1Up = true;
						break;
					case SDLK_z: p1Down = true;
						break;
					case SDLK_UP: p2Up = true;
						break;
					case SDLK_DOWN: p2Down = true;
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
				case SDLK_r: player1.state = idle;
					break;
				case SDLK_x: p1Up = false;
					break;
				case SDLK_z: p1Down = false;
					break;
				case SDLK_UP: p2Up = false;
					break;
				case SDLK_DOWN: p2Down = false;
					break;
				}
			break;
		}
	}
}
// end::handleInput[]

void aiResponse(State successState, float successRate) {
	random = rand() % 100 + 1;

	if (random <= successRate)
	{
		player2.state = successState;
		player2.successRateUpdate(successState);
		aiStateHold = 40;
	}
	else
	{
		player2.state = idle;
		player2.successRateUpdate(successState);
	}
}

void aiAttack(State successState, float successRate) {
	int random = rand() % 100 + 1;

	if (random <= successRate)
	{
		player2.state = successState;
		player2.successRateUpdate(successState);
	}
	else
	{
		player2.state = idle;
		player2.successRateUpdate(successState);
	}
}

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

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "slashHigh") {
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
		player2.stamina -= player1.state._staminaDrain;
		player2.health -= (player1.state._damage/4) * 3;
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "blockHigh" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player1.state = idle; 
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.health -= (player2.state._damage / 4) * 3;
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "stabHigh" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player2.state = idle;
	}
	
	

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




	///////MID STATES/////////////

	// idle states
	if (player1.state._name == "stabMid" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "stabMid" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "slashMid" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}
	//attacking states
	if (player1.state._name == "stabMid" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	//blocking states
	if (player1.state._name == "slashMid" &&
		player2.state._name == "blockMid" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= (player1.state._damage / 4) * 3;
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "blockMid" &&
		player1.stateTime == player1.state._actionEnd) {
		player1.state = idle; 
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "slashMid" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= (player2.state._damage / 4) * 3;
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "stabMid" &&
		player2.stateTime == player2.state._actionEnd) {
		player2.state = idle;
	}

	///////////////LOW STATES///////////////

	// idle states
	if (player1.state._name == "stabLow" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "stabLow" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "idle" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "idle" &&
		player2.state._name == "slashLow" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}
	//attacking states
	if (player1.state._name == "stabLow" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	//blocking states
	if (player1.state._name == "slashLow" &&
		player2.state._name == "blockLow" &&
		player1.stateTime == player1.state._actionEnd) {
		player2.health -= (player1.state._damage / 4) * 3;
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "blockLow" &&
		player1.stateTime == player1.state._actionEnd) {
		player1.state = idle; 
							  
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "slashLow" &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= (player2.state._damage / 4) * 3;
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "stabLow" &&
		player2.stateTime == player2.state._actionEnd) {
		player2.state = idle;
	}
	 

	
	





	//MIXED STATES

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}
	if (player1.state._name == "stabMid" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "BlockHigh" &&
		player2.state._name == "BlockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}


	if (player1.state._name == "BlockHigh" &&
		player2.state._name == "BlockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}


	if (player1.state._name == "BlockLow" &&
		player2.state._name == "BlockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "BlockLow" &&
		player2.state._name == "BlockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "BlockMid" &&
		player2.state._name == "BlockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "BlockMid" &&
		player2.state._name == "BlockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}


	//Parry
	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}




	//MOVE COMBOS


	//STAB

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			//player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "stabMid" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	//if (player1.state._name == "stabHigh" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabHigh" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabHigh" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabLow" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabLow" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}
	
	//if (player1.state._name == "stabLow" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabMid" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabMid" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "stabMid" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//SLASH MOVE COMBOS

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "blockHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "blockLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "blockMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	//if (player1.state._name == "slashHigh" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashHigh" &&
	//	player2.state._name == "parryMid" &&
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

	//if (player1.state._name == "slashLow" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashLow" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashLow" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashMid" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashMid" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "slashMid" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}


	//Block Move Combos

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "stabhigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "stabLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "stabMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "stabHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashLow") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashMid") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "slashHigh") {
		if (player1.stateTime == player1.state._actionEnd) {
			player2.health -= player1.state._damage;
		}
		if (player2.stateTime == player2.state._actionEnd) {
			player1.health -= player2.state._damage;
		}
	}

	//if (player1.state._name == "blockHigh" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockHigh" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockHigh" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockMid" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockMid" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockMid" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockLow" &&
	//	player2.state._name == "parryLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockLow" &&
	//	player2.state._name == "parryMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "blockLow" &&
	//	player2.state._name == "parryHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}



	//PARRY MOVE COMBOS


	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "stabLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "stabMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "stabHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "stabLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "stabMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "stabHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "stabLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "stabMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "stabHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "blockLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "blockMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "blockMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "blockHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "blockMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "blockLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "blockHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "blockMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "blockLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "slashLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "slashMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryHigh" &&
	//	player2.state._name == "slashHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "slashLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "slashMid" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryLow" &&
	//	player2.state._name == "slashHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "slashLow" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "slashHigh" &&
	//	player1.stateTime >= player1.state._prep &&
	//	player1.stateTime <= player1.state._actionStart &&
	//	player2.stateTime == player2.state._actionEnd) {
	//	player1.state = idle; // would be knockback state is added
	//	player2.state = idle;
	//}

	//if (player1.state._name == "parryMid" &&
	//	player2.state._name == "slashMid" &&
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
	floorTimer++;
	staminaDelay++;

	if (floorTimer > 3) {
		floorCount += 384;
		if (floorCount >= 3000)
		{
			floorCount = 0;
		}
		floorTimer = 0;
	}
	if (staminaDelay > 15) {
		staminaDelay = 0;
		player1.stamina += player1.staminaGain;
		player2.stamina += player2.staminaGain;
	}

	if (player1.stamina >= 200) {
		player1.stamina = 200;
	}
	if (player2.stamina >= 200) {
		player2.stamina = 200;
	}
	if (player1.stamina < 0) {
		player1.stamina = 0;
		player1.state = idle;
	}
	if (player2.stamina < 0) {
		player2.stamina = 0;
		player2.state = idle;
	}

	if (aiStateHold != 0)
		aiStateHold--;
	else
		player2.state = idle;


	player2.blockLowChance -= 5 * (simLength / 3);

	player1.animUpdate();
	player2.animUpdate();

	stateCompare();

	player1.inState("blockHigh");
	player1.inState("blockMid");
	player1.inState("blockLow");
	player1.inState("stabHigh");
	player1.inState("stabMid");
	player1.inState("stabLow");
	player1.inState("slashHigh");
	player1.inState("slashMid");
	player1.inState("slashLow");

	player2.inState("blockHigh");
	player2.inState("blockMid");
	player2.inState("blockLow");
	player2.inState("stabHigh");
	player2.inState("stabMid");
	player2.inState("stabLow");
	player2.inState("slashHigh");
	player2.inState("slashMid");
	player2.inState("slashLow");
}

void render()
{
		//First clear the renderer
		SDL_RenderClear(ren);
		SDL_RenderSetLogicalSize(ren, width, height);
		//Draw the texture
		SDL_Rect srcPlayer;
		SDL_Rect dstPlayer;

		SDL_Rect srcPlayer2;
		SDL_Rect dstPlayer2;

		SDL_Rect srcStaminaP1;
		SDL_Rect dstStaminaP1;

		SDL_Rect srcStaminaP2;
		SDL_Rect dstStaminaP2;

		SDL_Rect srcHealthP1;
		SDL_Rect dstHealthP1;

		SDL_Rect srcHealthP2;
		SDL_Rect dstHealthP2;

		SDL_Rect srcStaminaBoxP1;
		SDL_Rect dstStaminaBoxP1;

		SDL_Rect srcStaminaBoxP2;
		SDL_Rect dstStaminaBoxP2;

		SDL_Rect srcHealthBoxP1;
		SDL_Rect dstHealthBoxP1;

		SDL_Rect srcHealthBoxP2;
		SDL_Rect dstHealthBoxP2;

		SDL_Rect srcFloor;
		SDL_Rect dstFloor;
		
		srcPlayer.x = player1.xSpriteIndex;
		srcPlayer.y = player1.ySpriteIndex;
		srcPlayer.w = 500; // player1.state._sprWidth;
		srcPlayer.h = 400;

		dstPlayer.x = 325;
		dstPlayer.y = 150;
		dstPlayer.w = 250;
		dstPlayer.h = 200;

		srcPlayer2.x = player2.xSpriteIndex;
		srcPlayer2.y = player2.ySpriteIndex;
		srcPlayer2.w = 500; // player2.state._sprWidth;
		srcPlayer2.h = 400;

		dstPlayer2.x = 500;
		dstPlayer2.y = 150;
		dstPlayer2.w = 250;
		dstPlayer2.h = 200;
		///////////////////////////////////////
		srcStaminaP1.x = 210;
		srcStaminaP1.y = 0;
		srcStaminaP1.w = 20;
		srcStaminaP1.h = 20;

		dstStaminaP1.x = 100;
		dstStaminaP1.y = 100;
		dstStaminaP1.w = player1.stamina;
		dstStaminaP1.h = 20;

		srcStaminaBoxP1.x = 0;
		srcStaminaBoxP1.y = 0;
		srcStaminaBoxP1.w = 200;
		srcStaminaBoxP1.h = 20;

		dstStaminaBoxP1.x = 100;
		dstStaminaBoxP1.y = 100;
		dstStaminaBoxP1.w = 200;
		dstStaminaBoxP1.h = 20;
		///////////////////////////////////////
		srcStaminaP2.x = 210;
		srcStaminaP2.y = 0;
		srcStaminaP2.w = 20;
		srcStaminaP2.h = 20;

		dstStaminaP2.x = 700;
		dstStaminaP2.y = 100;
		dstStaminaP2.w = player2.stamina;
		dstStaminaP2.h = 20;

		srcStaminaBoxP2.x = 0;
		srcStaminaBoxP2.y = 0;
		srcStaminaBoxP2.w = 200;
		srcStaminaBoxP2.h = 20;

		dstStaminaBoxP2.x = 700;
		dstStaminaBoxP2.y = 100;
		dstStaminaBoxP2.w = 200;
		dstStaminaBoxP2.h = 20;
		//////////////////////////////////////
		srcHealthP1.x = 210;
		srcHealthP1.y = 0;
		srcHealthP1.h = 20;
		srcHealthP1.w = 20;

		dstHealthP1.x = 100;
		dstHealthP1.y = 70;
		dstHealthP1.h = 20;
		dstHealthP1.w = player1.health;

		srcHealthBoxP1.x = 0;
		srcHealthBoxP1.y = 0;
		srcHealthBoxP1.h = 20;
		srcHealthBoxP1.w = 200;

		dstHealthBoxP1.x = 100;
		dstHealthBoxP1.y = 70;
		dstHealthBoxP1.h = 20;
		dstHealthBoxP1.w = 200;
		/////////////////////////////////////
		srcHealthP2.x = 210;
		srcHealthP2.y = 0;
		srcHealthP2.h = 20;
		srcHealthP2.w = 20;

		dstHealthP2.x = 700;
		dstHealthP2.y = 70;
		dstHealthP2.h = 20;
		dstHealthP2.w = player2.health;

		srcHealthBoxP2.x = 0;
		srcHealthBoxP2.y = 0;
		srcHealthBoxP2.h = 20;
		srcHealthBoxP2.w = 200;

		dstHealthBoxP2.x = 700;
		dstHealthBoxP2.y = 70;
		dstHealthBoxP2.h = 20;
		dstHealthBoxP2.w = 200;
		////////////////////////////////////
		srcFloor.x = 0;
		srcFloor.y = floorCount;
		srcFloor.w = 1000;
		srcFloor.h = 384;

		dstFloor.x = 0;
		dstFloor.y = 0;
		dstFloor.w = 1000;
		dstFloor.h = 384;

		SDL_RenderCopy(ren, floorTex, &srcFloor, &dstFloor);


		SDL_RenderCopy(ren, staminaTex, &srcStaminaP1, &dstStaminaP1);
		SDL_RenderCopy(ren, staminaTex, &srcStaminaP2, &dstStaminaP2);

		SDL_RenderCopy(ren, staminaTex, &srcStaminaBoxP1, &dstStaminaBoxP1);
		SDL_RenderCopy(ren, staminaTex, &srcStaminaBoxP2, &dstStaminaBoxP2);


		SDL_RenderCopy(ren, healthTex, &srcHealthP1, &dstHealthP1);
		SDL_RenderCopy(ren, healthTex, &srcHealthP2, &dstHealthP2);

		SDL_RenderCopy(ren, healthTex, &srcHealthBoxP1, &dstHealthBoxP1);
		SDL_RenderCopy(ren, healthTex, &srcHealthBoxP2, &dstHealthBoxP2);


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
	win = SDL_CreateWindow("Swords Of Turing Alpha 0.1", 100, 100, width, height, SDL_WINDOW_RESIZABLE);

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

	std::string imagePath = "assets/sprites/character.png";
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

	imagePath = "assets/sprites/background black and white to colour v5.png";
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

	imagePath = "assets/sprites/playerStamina.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	staminaTex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (floorTex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	imagePath = "assets/sprites/playerHealth.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	healthTex = SDL_CreateTextureFromSurface(ren, surface);
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
		cout << "P1STATE: " << player1.state._name << "       P2STATE: " << player2.state._name << "       BLOCKLOW: " << player2.blockLowChance << std::endl;

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
