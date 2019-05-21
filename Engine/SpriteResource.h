#pragma once

#include "AnimatedSprite.h"

class SpriteResource
{
public:
	SpriteResource();

	Frames hero_running[ 8 ];
	Frames hero_idle[ 8 ];

	Sprite chest, key;
	Sprite m_ground, m_northwall, m_eastwall, m_southwall, m_westwall;
	Sprite m_corner;

private:
	void LoadHeroFrames();
	void LoadObjectSprites();
	void CreateWallSprites();

};

inline SpriteResource sprites;
