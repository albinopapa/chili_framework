#pragma once

#include "AlphaSprite.h"
#include "Entity.h"

struct ParticleSetupDesc
{
	enum DrawFunc
	{
		Circle, CircleAlpha,
		Rect, RectAlpha
	};
public:
	ParticleSetupDesc() = default;
	ParticleSetupDesc(
		float Speed, 
		float MinWidth, float MaxWidth,
		float MinHeight, float MaxHeight,
		float MinTimeToLive, float MaxTimeToLive,
		Color Clr );

public:
	float speed = 16.f;
	float minWidth = 4.f, maxWidth = 16.f;
	float minHeight = 1.f, maxHeight = 15.f;
	float minTimeToLive = .1f, maxTimeToLive = 2.f;
	DrawFunc drawFunc = CircleAlpha;
	Color color = Colors::White;
};

class Particle
{
public:
	Particle() = default;
	Particle(
		const Vec2f &StartPos,
		const Vec2f &StartVelocity,
		float Width, float Height,
		float TimeToLive,
		ParticleSetupDesc::DrawFunc Fn,
		Color C );

	virtual void Update( float DeltaTime );
	virtual void Draw( const Rectf &Viewport, Graphics &Gfx )const;

	void Position( const Vec2f &Pos ) { m_position = Pos; }
	Vec2f Position()const { return m_position; }

	void Velocity( const Vec2 &Vel ) { m_velocity = Vel; }
	Vec2f Velocity()const { return m_velocity; }

	Rectf GetRect()const;
	bool IsDead()const;

protected:
	float m_timeToLive = 0.f, m_liveCounter = 0.f;
	float m_width, m_height;
	Color m_color;
	Vec2f m_position, m_velocity;
	void( Graphics::*DrawFn )( const Recti &Rect, Color C );
};
