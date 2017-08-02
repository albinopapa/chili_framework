#pragma once

#include "Keyboard.h"
#include "Graphics.h"

constexpr float fScreenWidth = static_cast< float >( Graphics::ScreenWidth );
constexpr float fScreenHeight = static_cast< float >( Graphics::ScreenHeight );

class Scene
{
public:

	Scene( Keyboard &Kbd, Graphics &Gfx );

	virtual void Update( float DeltaTime ) = 0;
	virtual void Draw()const = 0;

	virtual ~Scene() = default;

protected:
	Graphics &m_graphics;
	Keyboard &m_keyboard;
};

