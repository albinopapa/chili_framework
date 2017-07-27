#pragma once

#include "AnimatedSprite.h"
#include "Camera.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Rect.h"
#include "SpriteCache.h"

class Character
{
	enum class State
	{
		StandingLeft, StandingRight, WalkingLeft, WalkingRight
	};

public:
	Character() = default;
	Character( const Vec2f &StartPos, Keyboard &Kbd, const SpriteCache &Cache );

	void Update( float DeltaTime );
	void Draw( const Rectf &Viewport, Graphics &Gfx );

	const Vec2f &GetPosition()const;
	Rectf GetRect()const;

protected:
	bool Transition();
	void DoState( bool DidTransition, float DeltaTime );

protected:
	const SpriteCache &m_cache;
	AnimationController m_animController;
	Sizef m_size;
	Vec2f m_position;
	Keyboard &m_keyboard;
	State m_state = State::StandingRight;
};

