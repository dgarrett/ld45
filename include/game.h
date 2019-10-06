#ifndef GAME_H
#define GAME_H

#include <gbdkjs.h>
#include <gb/gb.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

enum PLAYER_BODY {
	DOT,
	LEGS,
	TORSO,
	HEAD
};

struct player_character {
	enum PLAYER_BODY body;
	unsigned char *sprite_sheet;
	int draw_direction;
};

#endif
