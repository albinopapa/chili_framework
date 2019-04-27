#pragma once

#include "AnimatedSprite.h"

struct SpriteCache
{
	SpriteCache();

public:
	Frames m_character_walkR/*, m_character_walkL*/,  m_character_standR/*, m_character_standL*/;
	Sprite m_background;

};