#pragma once

#include "AnimatedSprite.h"

struct SpriteCache
{
	SpriteCache();

public:
	Frames m_character_walk, m_character_stand;
	Sprite m_background;

};