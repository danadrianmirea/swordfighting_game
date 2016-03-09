#pragma once
#include "State.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>
//test
using namespace std;
//hellorrr
class Player
{
public:
	int health = 100;
	int stamina = 50;
	int staminaGain = 1;
	int stateTime = 0;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	void inState(string name);
	void stateCompare();
};//

void Player::inState(string name)
{
	if (state._name == name) {
		stateTime++;
		if (stateTime > state._actionEnd) {
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}
	}
}

void Player::stateCompare()
{

}