#pragma once

#include "Sprite.h"

class AlphaSprite : public Sprite
{
public:
	using Sprite::Sprite;

	void Draw( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const override;
	void DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const override;
};

