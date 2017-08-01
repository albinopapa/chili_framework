#pragma once

#include "Sprite.h"

class AlphaSprite : public Sprite
{
public:
	using Sprite::Sprite;

	std::unique_ptr<Sprite> CopyFromRegion( const Recti &Src )const override;

	virtual void Draw( const Rectf &Dst, Graphics &Gfx )const override;
	virtual void Draw( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const override;
	virtual void DrawReverse( const Rectf &Dst, Graphics &Gfx )const override;
	virtual void DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics &Gfx )const override;
};

