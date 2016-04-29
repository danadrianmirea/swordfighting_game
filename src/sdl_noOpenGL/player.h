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
	int health = 200;
	int stamina = 200;
	int staminaGain = 6;
	int stateTime = 0;
	int animFrame = 0;
	int xSpriteIndex = 0;
	int ySpriteIndex = 0;
	int staminaTime = 0;
	int animTimer = 0;

	float blockHighChance = 50;
	float blockMidChance = 50;
	float blockLowChance = 50;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 0, 7000);

	void inState(string name);
	void successRateUpdate(State name);
	void animUpdate();
	void animReset();
};//
//TODO: probalby want to put the AI in here or at least this is a good place to record what actions the player has taken
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
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 500, 0, 7000);
		}
	}
}

void Player::successRateUpdate(State attack)
{
	if (attack._name == "blockHigh")
	{
		blockHighChance += 10;
		blockMidChance	-= 5;
		blockLowChance	-= 5;
	}

	if (attack._name == "blockMid")
	{
		blockHighChance -= 5;
		blockMidChance	+= 10;
		blockLowChance	-= 5;
	}

	if (attack._name == "blockLow")
	{
		blockHighChance -= 5;
		blockMidChance	-= 5;
		blockLowChance	+= 10;
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