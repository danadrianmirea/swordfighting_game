#pragma once
#include "State.h"
#include "player.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>

using namespace std;

class Ai : public Player
{
public:
	void aiUpdate(Player other);
};

void Ai::aiUpdate(Player other)
{
	// When player 1 is idle or defending, think about attacking

		// Do a stamina check to see if we can attack
			// if(stamina > 40)
			// CREATE MOVE LOGGER FOR "OTHER" PLAYER
		// Check which attack will be optimal based off of previous defense moves
		// Make attack based off of these criteria

	// When player 2 is attacking, think about the defending stance
		
		// Do a check against the blockChance (already kind of implemented)
		// Randomise a possible response, based off of the check done above
		// Delay the response until the attack in in motion (looks weird if you defend the instant they attack)
		// Hold the block for a second and return to idle

	// Oh shit, parry
}