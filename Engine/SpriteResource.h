#pragma once

#include "AnimatedSprite.h"

struct SpriteResource
{
	Frames hero_running[ 8 ];
	Frames hero_idle[ 8 ];
	// Frames hero_jumping[ 8 ];
	Sprite chest, key;
};

inline SpriteResource sprites;
