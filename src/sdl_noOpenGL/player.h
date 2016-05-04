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

using namespace std;

State stabHigh = State("stabHigh", 2, 30, 36, 40, 20, 0, 5, 0, 0, 0, 1280, 5600);
State stabMid = State("stabMid", 2, 30, 36, 40, 20, 0, 5, 0, 0, 0, 1600, 5600);
State stabLow = State("stabLow", 2, 30, 36, 40, 20, 0, 5, 0, 0, 0, 1920, 5600);

State slashHigh = State("slashHigh", 0, 45, 50, 60, 40, 0, 0, 0, 0, 0, 320, 8000);
State slashMid = State("slashMid", 0, 45, 50, 60, 40, 0, 0, 0, 0, 0, 640, 8000);
State slashLow = State("slashLow", 0, 45, 50, 60, 40, 0, 0, 0, 0, 0, 960, 8000);

State blockHigh = State("blockHigh", 2, 0, 4, 0, 0, 0, 2, 100, 25, 800, 2240, 800);
State blockMid = State("blockMid", 2, 0, 4, 0, 0, 0, 2, 100, 25, 800, 2560, 800);
State blockLow = State("blockLow", 2, 0, 4, 0, 0, 0, 2, 100, 25, 800, 2880, 800);

State parryHigh = State("parryHigh", 5, 6, 8, 45, 0, 10, 50, 100, 100, 0, 3200, 4000);
State parryMid = State("parryMid", 5, 6, 8, 45, 0, 10, 50, 100, 100, 0, 3520, 4000);
State parryLow = State("parryLow", 5, 6, 8, 45, 0, 10, 50, 100, 100, 0, 3840, 4000);
State idle = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5600);

class Player
{
public:
	int health = 200;
	int stamina = 200;
	int staminaGain = 6;
	int stateTime = 0;
	int xSpriteIndex = 0;
	int ySpriteIndex = 0;
	int staminaTime = 0;
	int animTimer = 0;

	float blockHighChance = 50;
	float blockMidChance = 50;
	float blockLowChance = 50;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5600);

	void inState(string name);
	void successRateUpdate(State name);
	void animUpdate();
	void animReset();
};

void Player::inState(string name)
{
	if (state._name == "blockHigh" ||
		state._name == "blockMid" ||
		state._name == "blockLow") {
		staminaTime++;
		if (staminaTime > 16) {
			staminaTime = 0;
			stamina -= state._staminaDrain;
		}
		return;
	}
	else if (state._name == name) {
		stateTime++;
		if (stateTime >= state._prep) {
		}
		if (stateTime >= state._actionStart) {
		}
		if (stateTime > state._actionEnd) {
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5600);
		}
	}
}

void Player::successRateUpdate(State attack)
{
	if (attack._name == "blockHigh")
	{
		blockHighChance += 15;
	}

	if (attack._name == "blockMid")
	{
		blockMidChance	+= 15;
	}

	if (attack._name == "blockLow")
	{
		blockLowChance	+= 15;
	}
}

void Player::animUpdate()
{
	animTimer++;
	if (animTimer > 2) {
		xSpriteIndex += 400;
		ySpriteIndex = state._sprLocation;
		if (xSpriteIndex >= state._sprLength)
			xSpriteIndex = state._sprWidth;
		animTimer = 0;
	}
}

void Player::animReset()
{
	xSpriteIndex = 0;
	ySpriteIndex = 0;
}