#pragma once

#include "Particle.h"
#include <random>
#include <vector>

class Emitter
{
public:
	struct Description
	{
		Vec2f m_position, m_direction;
		size_t m_maxParticles = 0u;
		float m_minDelay, m_maxDelay;
		bool m_infinite = true;
	};
	
public:
	Emitter() = default;
	Emitter( const Particle::Description &PartDesc, const Emitter::Description &EmDesc );

	void Update( float DeltaTime, const Rectf &Viewport );
	void Draw( const Rectf &Viewport, Graphics &Gfx )const;

	const std::vector<Vec2f> &GetPositionVector()const;
	void ClearPositionVector();

	bool IsAllDead()const;

private:
	void SpawnParticle();
	std::vector<Particle>::iterator GetFirstDead();
	void SaveDeadPositions( std::vector<Particle>::iterator FirstDead );

private:
	std::vector<Particle> m_particles;
	std::vector<Vec2f> m_deadParticlePositions;

	Particle::Description m_particleDesc;
	Emitter::Description m_emitterDesc;

	float m_delayCounter = 0.f;
	bool m_canSpawn = true;

	std::mt19937 m_rng;
	std::uniform_real_distribution<float> m_ttlDist, m_radiusDist, m_delayDist;
};

