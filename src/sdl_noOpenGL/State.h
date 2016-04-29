#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_rect.h>

using namespace std;

class State
{
public:
	string _name;
	int _prep;
	int _actionStart;
	int _actionEnd;
	int _stamina;
	int _damage;
	int _knockback;
	int _staminaDrain;
	int _negateStab;
	int _negateSlash;
	int _sprWidth;
	int _sprLocation;
	int _sprLength;

	State(string name, int prep, int actionStart, int actionEnd, int stamina, int damage,
		int knockback, int staminaDrain, int negateStab, int negateSlash, 
		int spriteWidth, int spriteLocation, int sprLength);
};

State::State(string name, int prep, int actionStart, int actionEnd, int stamina, int damage,
	int knockback, int staminaDrain, int negateStab, int negateSlash, 
	int spriteWidth, int spriteLocation, int sprLength)
{
	_name = name;
	_prep = prep;
	_actionStart = actionStart;
	_actionEnd = actionEnd;
	_stamina = stamina;
	_damage = damage;
	_knockback = knockback;
	_staminaDrain = staminaDrain;
	_negateStab = negateStab;
	_negateSlash = negateSlash;
	_sprWidth = spriteWidth;
	_sprLocation = spriteLocation;
	_sprLength = sprLength;
}