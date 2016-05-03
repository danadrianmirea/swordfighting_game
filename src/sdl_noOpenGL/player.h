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

State stabHigh = State("stabHigh", 10, 20, 36, 40, 20, 0, 5, 0, 0, 0, 1600, 7000);
State stabMid = State("stabMid", 10, 20, 36, 40, 20, 0, 5, 0, 0, 0, 2000, 7000);
State stabLow = State("stabLow", 10, 20, 36, 40, 20, 0, 5, 0, 0, 0, 2400, 7000);

State slashHigh = State("slashHigh", 10, 30, 50, 80, 40, 0, 10, 0, 0, 0, 400, 10000);
State slashMid = State("slashMid", 10, 30, 50, 80, 40, 0, 10, 0, 0, 0, 800, 10000);
State slashLow = State("slashLow", 10, 30, 50, 80, 40, 0, 10, 0, 0, 0, 1200, 10000);

State blockHigh = State("blockHigh", 2, 0, 4, 0, 0, 0, 2, 100, 25, 1000, 2800, 1000);
State blockMid = State("blockMid", 2, 0, 4, 0, 0, 0, 2, 100, 25, 1000, 3200, 1000);
State blockLow = State("blockLow", 2, 0, 4, 0, 0, 0, 2, 100, 25, 1000, 3600, 1000);

State parryHigh = State("parryHigh", 5, 10, 15, 45, 0, 10, 0, 100, 100, 0, 4000, 5000);
State parryMid = State("parryMid", 5, 10, 15, 45, 0, 10, 0, 100, 100, 0, 4400, 5000);
State parryLow = State("parryLow", 5, 10, 15, 45, 0, 10, 0, 100, 100, 0, 4800, 5000);
State idle = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7000);

class Player
{
public:
	int health = 200;
	int stamina = 200;
	int staminaGain = 6;
	int stateTime = 0;
	int animFrame = 0;
	int xSpriteIndex = 0;
	int ySpriteIndex = 0;
	int staminaTime = 0;
	int animTimer = 0;

	float blockHighChance = 20;
	float blockMidChance = 20;
	float blockLowChance = 20;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7000);

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
		if (staminaTime > 6) {
			staminaTime = 0;
			stamina -= state._staminaDrain;
		}
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
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7000);
		}
	}
}

void Player::successRateUpdate(State attack)
{
	if (attack._name == "blockHigh")
	{
		blockHighChance += 20;
	}

	if (attack._name == "blockMid")
	{
		blockMidChance	+= 20;
	}

	if (attack._name == "blockLow")
	{
		blockLowChance	+= 20;
	}
}

void Player::animUpdate()
{
	animTimer++;
	if (animTimer > 2) {
		xSpriteIndex += 500;
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