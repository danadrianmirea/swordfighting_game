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
	int randomChance;

	bool gaveitmybestshot = false;

	int response = 5;
	void aiUpdate(Player other);
	void aiChoice(string state);
};

void Ai::aiUpdate(Player human)
{
	if (human.state._name != "idle")
		dCount++;

	if (human.state._name == "idle" && (state._name == "blockMid" || state._name == "blockLow" || state._name == "blockHigh"))
		state = idle;

	if (human.state._name != "idle"
		&& state._name == "idle"
		&& dCount >= delay
		&& gaveitmybestshot == false) {
		delay = rand() % response + 10;
		dCount = 0;
		aiChoice(human.state._name);
	}

	if (human.state._name == "idle") {
		gaveitmybestshot = false;
		dCount = 0;
	}
}

void Ai::aiChoice(string playerState) {

	gaveitmybestshot = true;
	randomChance = rand() % 100 + 1;

	if (playerState == "stabMid") {
		if (randomChance <= blockMidChance)
			state = blockMid;

		successRateUpdate(blockMid);
	}

	if (playerState == "stabHigh") {
		state = blockHigh;
	}

	if (playerState == "stabLow") {
		state = blockLow;
	}
}