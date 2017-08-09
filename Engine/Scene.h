#pragma once

#include "Keyboard.h"
#include "Graphics.h"


class Scene
{
public:
	Scene( Keyboard &Kbd, Graphics &Gfx );
	virtual ~Scene() = default;

	virtual void Update( float DeltaTime ) = 0;
	virtual void Draw()const = 0;

protected:
	Graphics &m_graphics;
	Keyboard &m_keyboard;
};

