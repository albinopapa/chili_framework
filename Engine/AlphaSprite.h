#pragma once

#include "Sprite.h"

class AlphaSprite : public Sprite
{
public:
	using Sprite::Sprite;

	void Draw( float OffsetX, float OffsetY, Graphics &Gfx )const override;
	void DrawReverse( float OffsetX, float OffsetY, Graphics &Gfx )const override;
};

