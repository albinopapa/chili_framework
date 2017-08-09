#pragma once

#include "Graphics.h"

class Drawable
{
public:	
	virtual ~Drawable() = default;

	virtual void Draw( const Rectf &Viewport, Graphics &Gfx )const = 0;
};

