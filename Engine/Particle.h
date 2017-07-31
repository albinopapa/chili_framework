#pragma once

#include "AlphaSprite.h"

class Particle
{
public:
	struct Description
	{
		float m_minParticleRadius, m_maxParticleRadius;
		float m_minParticleTimeToLive, m_maxParticleTimeToLive;
		Color m_particleColor;
	};
public:
	Particle() = default;
	Particle( 
		const Vec2f &StartPos, 
		const Vec2f &StartVelocity, 
		float Radius,
		float DragCoeff,
		float TimeToLive,
		bool IsAffectedByGravity,
		Color C );

	void Update( float DeltaTime );
	void Draw( const Rectf &Viewport, Graphics &Gfx )const;

	void KillOffscreen( const Rectf &Viewport );
	bool IsDead()const;
	Vec2f GetPosition()const;

private:
	Vec2f m_position, m_velocity;
	float m_timeToLive = 0.f, m_liveCounter = 0.f;
	float m_dragcoeff = 0.f;
	float m_radius;
	bool m_affectedByGravity = true;
	Color m_color;
	static Vec2f s_gravity;
};

