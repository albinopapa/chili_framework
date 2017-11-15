#pragma once

#include "Particle.h"
#include "Entity.h"

class Projectile :public Particle
{
public:
	Projectile();
	Projectile( const Vec2f &StartPos,
				const Vec2f &StartVelocity,
				float Width, float Height,
				float TimeToLive,
				ParticleSetupDesc::DrawFunc Fn,
				Color C );

	void Update( float DeltaTime ) { m_position += ( m_velocity * DeltaTime ); }
	void Draw( const Rectf &Viewport, Graphics &Gfx )const override;

	void Kill() { m_timeToLive = 0.f; }

public:
	static constexpr float m_speed = 500.f;
private:
};

