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
	int animFrame = 0;
	int xSpriteIndex = 0;
	int ySpriteIndex = 0;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);

	void inState(string name);
	void animUpdate();
};//

void Player::inState(string name)
{
	if (state._name == "blockHigh") {
		return;
	}
	else if (state._name == name) {
		stateTime++;
		if (stateTime >= state._prep) {
			animFrame = 1;
		}
		if (stateTime >= state._actionStart) {
			animFrame = 2;
		}
		if (stateTime >= state._actionEnd) {
			animFrame = 3;
		}
		if (stateTime > state._actionEnd) {
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0);
		}
	}
}

void Player::animUpdate()
{
	if (animFrame == 1) {
		xSpriteIndex = 100;
		ySpriteIndex = 0;
	}
	if (animFrame == 2) {
		xSpriteIndex = 0;
		ySpriteIndex = 100;
	}
	if (animFrame == 3) {
		xSpriteIndex = 100;
		ySpriteIndex = 100;
	}
}