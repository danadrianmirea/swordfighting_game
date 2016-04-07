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
	int staminaGain = 1;
	int stateTime = 0;
	int animFrame = 0;
	int xSpriteIndex = 0;
	int ySpriteIndex = 0;
	int staminaTime = 0;
	int animTimer = 0;

	State state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0);

	void inState(string name);
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
			state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0);
		}
	}
}

void Player::animUpdate()
{
	animTimer++;
	if (animTimer > 2) {
		if (state._actionEnd == 75) {

		}
		if (state._actionEnd == 36) {
			xSpriteIndex += state._sprWidth;
			ySpriteIndex = state._sprLocation + 300;
			if (xSpriteIndex >= 4800) {
				state = State("idle", 0, 0, 0, 0, 0, 0, 0, 0, 0, 300, 0);
			}
		}
		if (state._actionEnd == 0) {
			xSpriteIndex += state._sprWidth;
			ySpriteIndex = state._sprLocation + 300;
			if (xSpriteIndex >= 4200) {
				xSpriteIndex = 0;
			}
		}
		if (state._actionEnd == 2) {
			xSpriteIndex += state._sprWidth;
			ySpriteIndex = state._sprLocation + 300;
			if (xSpriteIndex >= 600) {
				xSpriteIndex = 600;
			}
		}
		animTimer = 0;
	}
}

void Player::animReset()
{
	xSpriteIndex = 0;
	ySpriteIndex = 0;
}