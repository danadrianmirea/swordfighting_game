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
//test
using namespace std;
//hellorrr
class Ai : Player
{
public:
	void aiUpdate(Player other);
};

void Ai::aiUpdate(Player other)
{

}