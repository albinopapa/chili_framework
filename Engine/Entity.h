#pragma once

#include "Observer.h"
#include "Graphics.h"

class Entity
{
public:
	Entity() = default;
	Entity( const Vec2f &Pos, const Vec2f &Vel );
	virtual ~Entity() = default;

	virtual void Update( float DeltaTime ) = 0;
	virtual void Draw( const Rectf &Viewport, Graphics &Gfx )const {}

	void Position( const Vec2f &Pos ) { m_position = Pos; }
	Vec2f Position()const { return m_position; }

	void Velocity( const Vec2f &Vel ) { m_velocity = Vel; }
	Vec2f Velocity()const { return m_velocity; }

protected:
	Vec2f m_position, m_velocity;
};

