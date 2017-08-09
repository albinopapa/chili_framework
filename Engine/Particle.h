#pragma once

#include "AlphaSprite.h"
#include "Entity.h"

struct ParticleSetupDesc
{
public:
	ParticleSetupDesc() = default;
	ParticleSetupDesc(
		float Speed, float MinRadius, float MaxRadius,
		float MinTimeToLive, float MaxTimeToLive,
		Color Clr );

public:
	float speed = 16.f;
	float minRadius = 5.f, maxRadius = 15.f;
	float minTimeToLive = .1f, maxTimeToLive = 2.f;
	Color color = Colors::White;
};

class Particle
{
public:
	Particle() = default;
	Particle(
		const Vec2f &StartPos,
		const Vec2f &StartVelocity,
		float Radius,
		float TimeToLive,
		Color C );

	void Update( float DeltaTime );
	void Draw( const Rectf &Viewport, Graphics &Gfx )const;

	void Position( const Vec2f &Pos ) { m_position = Pos; }
	Vec2f Position()const { return m_position; }

	void Velocity( const Vec2 &Vel ) { m_velocity = Vel; }
	Vec2f Velocity()const { return m_velocity; }

	Rectf GetRect()const;
	bool IsDead()const;
private:
	float m_timeToLive = 0.f, m_liveCounter = 0.f;
	float m_radius;
	Color m_color;
	Vec2f m_position, m_velocity;
};
