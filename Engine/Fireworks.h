#pragma once

#include "Observer.h"
#include "ParticleEmitter.h"


struct TimeSpawned
{
	static constexpr size_t m_count = 5;
	float m_launchCounter[ m_count ], m_currentDelay[ m_count ];
	Vec2f m_positions[ m_count ];
};

class Fireworks
{
public:
	Fireworks();
	void Update( float DeltaTime );
		
	const std::vector<ParticleVector *> &GetParticleVectors()const;

private:
	void EmitPrimary( float DeltaTime );
	void EmitSecondary();

private: // Members
	TimeSpawned m_baseEmitterData;
	SingleEmitter m_baseEmitter;
	RadialEmitter m_burstEmitter;

	std::mt19937 m_rng;

	std::uniform_int_distribution<unsigned int> m_colorDist;
	static constexpr Color m_colorPalette[ 5 ] = {
		Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow, Colors::Magenta
	};

	std::uniform_real_distribution<float> m_delayDist;

	ParticleVector m_primary, m_secondary;
	std::vector<ParticleVector *> m_particles;
};

