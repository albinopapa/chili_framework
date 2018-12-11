#pragma once

#include "Particle.h"
#include <vector>
#include <random>

using ParticleVector = std::vector<Particle>;
class Emitter
{
public:
	Emitter() = default;
	Emitter( 
		const Vec2f &_position,
		const Vec2f& _direction, 
		int _launchCount, 
		int _maxParticles );

	void SetPosition( const Vec2f& _position );
	void SetDirection( const Vec2f& _direction );

	Particle SpawnParticles( const ParticleSetupDesc& _desc );

private:
	Vec2f m_position, m_direction;
	int m_launchCount = 0u;
	int m_maxParticles = 1u;
	bool m_canSpawn = true;

};
