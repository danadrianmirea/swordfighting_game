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
	int delay = 20;
	int dCount = 0;
	int attackCount;
	int randomChance;

	bool madeAttempt = false;

	int response = 5;
	void aiUpdate(Player other);
	void aiChoice(string state);
};

void Ai::aiUpdate(Player human)
{
	attackCount++;

	if (human.state._name != "idle")
		dCount++;

	if (human.state._name == "idle" && attackCount > 125 && state._name == "idle") {
 		attackCount = 0;
		aiChoice(human.state._name);
	}

	if (human.state._name == "idle" && (state._name == "blockMid" || state._name == "blockLow" || state._name == "blockHigh"))
		state = idle;

	if (human.state._name != "idle" && state._name == "idle" && dCount >= delay && madeAttempt == false) {
		delay = rand() % response + 10;
		dCount = 0;
		aiChoice(human.state._name);
	}

	if (human.state._name == "idle") {
		madeAttempt = false;
		dCount = 0;
	}
}

void Ai::aiChoice(string playerState) {

	madeAttempt = true;
	randomChance = rand() % 100 + 1;

	if (playerState == "stabMid") {
		if (randomChance <= blockMidChance)
			state = blockMid;
		else if (rand() % 100 + 1 > 50)
			state = blockHigh;
		else
			state = blockLow;

		successRateUpdate(blockMid);
	}

	if (playerState == "stabHigh") {
		if (randomChance <= blockHighChance)
			state = blockHigh;
		else if (rand() % 100 + 1 > 50)
			state = blockMid;
		else
			state = blockLow;

		successRateUpdate(blockHigh);
	}

	if (playerState == "stabLow") {
		if (randomChance <= blockLowChance)
			state = blockLow;
		else if (rand() % 100 + 1 > 50)
			state = blockHigh;
		else
			state = blockMid;

		successRateUpdate(blockLow);
	}

	if (playerState == "blockMid") {
		if (rand() % 100 + 1 > 50)
			state = stabHigh;
		else
			state = stabLow;
	}

	if (playerState == "blockHigh") {
		if (rand() % 100 + 1 > 50)
			state = stabMid;
		else
			state = stabLow;
	}

	if (playerState == "blockLow") {
		if (rand() % 100 + 1 > 50)
			state = stabMid;
		else
			state = stabHigh;
	}

	if (playerState == "idle") {
		int rando = rand() % 100 + 1;
		if (rando > 60)
			state = stabMid;
		else if (rando < 30) 
			state = stabHigh;
		else
			state = stabLow;
		stateTime = 0;
	}
}