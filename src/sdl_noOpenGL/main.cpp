#pragma once
#include "Player.h"
#include "State.h"
#include "Ai.h"

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
SDL_Texture *menuTex;
SDL_Texture *controlsTex;
SDL_Texture *p1Tex;
SDL_Texture *p2Tex;

SDL_RendererFlip flip = SDL_FLIP_HORIZONTAL;

Player player1 = Player();
Ai player2 = Ai();

bool done = false;
bool menu = true;
bool controls = false;
bool p1win = false;
bool p2win = false;

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
int height = 384;


void restart()
{
	player1.health = 200;
	player2.health = 200;
	player1.stamina = 200;
	player2.stamina = 200;
	player1.animReset();
	player2.animReset();
	player1.state = idle;
	player2.state = idle;
	p1win = false;
	p2win = false;

}
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
						}
						else if (p1Down == true) {
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

						if (p1Up == true)
							player1.state = parryHigh;
						else if (p1Down == true)
							player1.state = parryLow;
						else
							player1.state = parryMid;


						
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
								 if (menu == true) {
									 menu = false;
								 }
					case SDLK_SPACE: 
						if (menu == true) {
							menu = false;
							controls = false;
						}
						controls = false;
						break;
					case SDLK_c:
						if (controls == false) {
							controls = true;
						}
						break;
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
					case SDLK_UP: p1Up = true;
						break;
					case SDLK_DOWN: p1Down = true;
						break;
					case SDLK_RETURN: restart();
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
				case SDLK_UP: p1Up = false;
					break;
				case SDLK_DOWN: p1Down = false;
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
	if (player1.state._name == "stabHigh" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "stabHigh" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "stabLow" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashHigh" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashLow" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}
	
	if (player1.state._name == "slashLow" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.stamina -= player2.state._staminaDrain;
		player1.state = idle;
	}

	if (player1.state._name == "slashMid" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.health -= player1.state._damage;
	}

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

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockHigh" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockMid" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "parryLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "parryMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "blockLow" &&
		player2.state._name == "parryHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}



	//PARRY MOVE COMBOS


	if (player1.state._name == "parryHigh" &&
		player2.state._name == "stabLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "stabMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "stabHigh" &&
		player2.stateTime >= player2.state._prep &&
		player2.stateTime <= player2.state._actionStart &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "stabLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "stabMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "stabHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "stabLow" &&
		player2.stateTime >= player2.state._prep &&
		player2.stateTime <= player2.state._actionStart &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "stabMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "stabHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "blockLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "blockMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "blockMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "blockHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "blockMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "blockLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
		player2.state = idle;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "blockHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "blockMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "blockLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "slashLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "slashMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.state = idle; // would be knockback state is added
		player2.state = idle;
	}

	if (player1.state._name == "parryHigh" &&
		player2.state._name == "slashHigh" &&
		player2.stateTime >= player2.state._prep &&
		player2.stateTime <= player2.state._actionStart &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "slashLow" &&
		player2.stateTime >= player2.state._prep &&
		player2.stateTime <= player2.state._actionStart &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "slashMid" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryLow" &&
		player2.state._name == "slashHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "slashLow" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "slashHigh" &&
		player1.stateTime >= player1.state._prep &&
		player1.stateTime <= player1.state._actionStart &&
		player2.stateTime == player2.state._actionEnd) {
		player1.health -= player2.state._damage;
	}

	if (player1.state._name == "parryMid" &&
		player2.state._name == "slashMid" &&
		player2.stateTime >= player2.state._prep &&
		player2.stateTime <= player2.state._actionStart &&
		player1.stateTime == player1.state._actionEnd) {
		player2.stamina -= player1.state._staminaDrain;
		player2.state = idle;
	}
}
// tag::updateSimulation[]
void updateSimulation(double simLength = 0.02) //update simulation with an amount of time to simulate for (in seconds)
{
 	floorTimer++;
	staminaDelay++;

	player2.aiUpdate(player1);

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

	if (player2.blockHighChance < 20) {
		player2.blockHighChance = 20;
	}

	if (player2.blockMidChance < 20) {
		player2.blockMidChance = 20;
	}

	if (player2.blockLowChance < 20) {
		player2.blockLowChance = 20;
	}

	if (player2.blockLowChance >= 100) {
		player2.blockLowChance = 100;
	}

	if (player2.blockMidChance >= 100) {
		player2.blockMidChance = 100;
	}

	if (player2.blockHighChance >= 100) {
		player2.blockHighChance = 100;
	}

	player2.blockLowChance	-= 5 * (simLength / 5);
	player2.blockMidChance	-= 5 * (simLength / 5);
	player2.blockHighChance -= 5 * (simLength / 5);

	player1.animUpdate();
	player2.animUpdate();

	if (player1.health <= 0) {
		p2win = true;
	}
	if (player2.health <= 0) {
		p1win = true;
	}

	stateCompare();

	player1.inState("parryHigh");
	player1.inState("parryMid");
	player1.inState("parryLow");
	player1.inState("blockHigh");
	player1.inState("blockMid");
	player1.inState("blockLow");
	player1.inState("stabHigh");
	player1.inState("stabMid");
	player1.inState("stabLow");
	player1.inState("slashHigh");
	player1.inState("slashMid");
	player1.inState("slashLow");


	player2.inState("parryHigh");
	player2.inState("parryMid");
	player2.inState("parryLow");
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

		SDL_Rect srcMenu;
		SDL_Rect dstMenu;
		
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
		srcFloor.h = 375;

		dstFloor.x = 0;
		dstFloor.y = 0;
		dstFloor.w = 1000;
		dstFloor.h = 384;
		///////////////////////////////////
		srcMenu.x = 0;
		srcMenu.y = 0;
		srcMenu.w = 1000;
		srcMenu.h = 384;

		dstMenu.x = 0;
		dstMenu.y = 0;
		dstMenu.w = 1000;
		dstMenu.h = 384;

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

		if (p1win == true)
		SDL_RenderCopy(ren, p1Tex, &srcMenu, &dstMenu);

		if (p2win == true)
		SDL_RenderCopy(ren, p2Tex, &srcMenu, &dstMenu);

		if (menu == true) {
			SDL_RenderCopy(ren, menuTex, &srcMenu, &dstMenu);
		}
		if (controls == true) {
			SDL_RenderCopy(ren, controlsTex, &srcMenu, &dstMenu);
		}
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

	imagePath = "assets/sprites/menu.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	menuTex = SDL_CreateTextureFromSurface(ren, surface);
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
	imagePath = "assets/sprites/p2win.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	p2Tex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (floorTex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}
	imagePath = "assets/sprites/p1win.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	p1Tex = SDL_CreateTextureFromSurface(ren, surface);
	SDL_FreeSurface(surface);
	if (floorTex == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	imagePath = "assets/sprites/controls.png";
	surface = IMG_Load(imagePath.c_str());
	if (surface == nullptr) {
		SDL_DestroyRenderer(ren);
		SDL_DestroyWindow(win);
		std::cout << "SDL IMG_Load Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		return 1;
	}

	controlsTex = SDL_CreateTextureFromSurface(ren, surface);
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
		cout << "BlockLow: " << player2.state._name << "  BlockHigh: " << player2.stamina << "  BlockMid: " << player2.state._name << std::endl;

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