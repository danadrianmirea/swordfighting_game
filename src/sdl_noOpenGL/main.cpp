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
SDL_Texture *staminaTex;
SDL_Texture *healthTex;

SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

Player player1 = Player();
Player player2 = Player();
State stabHigh = State("stabHigh", 5, 10, 45, 10, 10, 0, 5, 0, 0);
State stabMid = State("stabMid", 5, 10, 45, 10, 10, 0, 5, 0, 0);
State stabLow = State("stabLow" ,5, 10, 45, 10, 10, 0, 5, 0, 0);

State slashHigh = State("slashHigh", 10, 20, 75, 20, 20, 0, 10, 0, 0);
State slashMid = State("slashMid", 10, 20, 75, 20, 20, 0, 10, 0, 0);
State slashLow = State("slashLow", 10, 20, 75, 20, 20, 0, 10, 0, 0);

State blockHigh = State("blockHigh", 2, 0, 0, 0, 0, 0, 1, 100, 25);
State blockMid = State("blockMid", 2, 0, 0, 0, 0, 0, 1, 100, 25);
State blockLow = State("blockLow", 2, 0, 0, 0, 0, 0, 1, 100, 25);

State parryHigh = State("parryHigh", 5, 0, 0, 15, 0, 10, 0, 100, 100);
State parryMid = State("parryMid", 5, 0, 0, 15, 0, 10, 0, 100, 100);
State parryLow = State("parryLow", 5, 0, 0, 15, 0, 10, 0, 100, 100);
State idle = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);



bool done = false;
bool up = false;
bool down = false;

int staminaDelay = 0;
int animTime = 0;
int floorCount = 0;
int floorTimer = 0;



// TODO Add Animations for Blocking
// TODO Add Animations for Directional Attacks and Blocks
// TODO add title screen
// TODO add control screen/ on screen controls
// TODO death end state
// TODO victory animations


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
						player1.animReset();

						if (up == true) {
							player1.state = stabHigh;
						}
						else if (down == true) {
							player1.state = stabLow;
						}
						else {
							player1.state = stabMid;
					}

						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
					case SDLK_w: if (player1.stamina >= slashHigh._stamina &&
						player1.state._name == "idle") {
						player1.animReset();

						if (up == true)
							player1.state = slashHigh;
						else if (down == true)
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
						player1.state = blockHigh;
						player1.stateTime = 0;
						player1.stamina -= player1.state._stamina;
					}
						break;
						
						// player 2 controls
					case SDLK_p: if (player2.stamina >= stabHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						player2.state = stabHigh;
						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
					case SDLK_o: if (player2.stamina >= slashHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						player2.state = slashHigh;
						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
					case SDLK_u: if (player2.stamina >= blockHigh._stamina &&
						player2.state._name == "idle") {
						player2.animReset();
						player2.state = blockHigh;
						player2.stateTime = 0;
						player2.stamina -= player2.state._stamina;
					}
						break;
					case SDLK_UP: up = true;
						break;
					case SDLK_DOWN: down = true;
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
				case SDLK_UP: up = false;
					break;
				case SDLK_DOWN: down = false;
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
			player2.health -= player1.state._damage;
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
		floorCount += 375;
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

	player1.animUpdate();
	player2.animUpdate();

	player1.inState("blockHigh");
	player1.inState("stabHigh");
	player1.inState("stabMid");
	player1.inState("stabLow");
	player1.inState("slashHigh");

	player2.inState("blockHigh");
	player2.inState("stabHigh");
	player2.inState("slashHigh");
	stateCompare();
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
		srcPlayer.w = 300;
		srcPlayer.h = 300;

		dstPlayer.x = 500;
		dstPlayer.y = 150;
		dstPlayer.w = 200;
		dstPlayer.h = 200;

		srcPlayer2.x = player2.xSpriteIndex;
		srcPlayer2.y = player2.ySpriteIndex;
		srcPlayer2.w = 300;
		srcPlayer2.h = 300;

		dstPlayer2.x = 325;
		dstPlayer2.y = 150;
		dstPlayer2.w = 200;
		dstPlayer2.h = 200;
		///////////////////////////////////////
		srcStaminaP1.x = 210;
		srcStaminaP1.y = 0;
		srcStaminaP1.w = 20;
		srcStaminaP1.h = 20;

		dstStaminaP1.x = 100;
		dstStaminaP1.y = 100;
		dstStaminaP1.w = player1.stamina * 4;
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
		dstStaminaP2.w = player2.stamina * 4;
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
		dstHealthP1.w = player1.health * 2;

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
		dstHealthP2.w = player2.health * 2;

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
		srcFloor.h = 375;

		dstFloor.x = 0;
		dstFloor.y = 0;
		dstFloor.w = 1000;
		dstFloor.h = 375;

		SDL_RenderCopy(ren, floorTex, &srcFloor, &dstFloor);


		SDL_RenderCopy(ren, staminaTex, &srcStaminaP1, &dstStaminaP1);
		SDL_RenderCopy(ren, staminaTex, &srcStaminaP2, &dstStaminaP2);

		SDL_RenderCopy(ren, staminaTex, &srcStaminaBoxP1, &dstStaminaBoxP1);
		SDL_RenderCopy(ren, staminaTex, &srcStaminaBoxP2, &dstStaminaBoxP2);


		SDL_RenderCopy(ren, healthTex, &srcHealthP1, &dstHealthP1);
		SDL_RenderCopy(ren, healthTex, &srcHealthP2, &dstHealthP2);

		SDL_RenderCopy(ren, healthTex, &srcHealthBoxP1, &dstHealthBoxP1);
		SDL_RenderCopy(ren, healthTex, &srcHealthBoxP2, &dstHealthBoxP2);


		SDL_RenderCopy(ren, playerTex, &srcPlayer2, &dstPlayer2);
		SDL_RenderCopyEx(ren, playerTex, &srcPlayer, &dstPlayer, 0, 0, flip);

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
	win = SDL_CreateWindow("Swords Of Turing Alpha 0.1", 100, 100, 1000, 375, SDL_WINDOW_SHOWN);

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

	imagePath = "assets/sprites/background.png";
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
