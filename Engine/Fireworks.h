#pragma once

#include "ParticleEffect.h"
#include "RadialEmitter.h"
#include "SingleEmitter.h"

struct TimeSpawned
{
	static constexpr size_t m_count = 10;
	float m_launchCounter[ m_count ], m_currentDelay[ m_count ];
	Vec2f m_positions[ m_count ];
};

class Fireworks :public ParticleEffect
{
public:
	Fireworks();
	const std::vector<ParticleVector *> &GetParticleVectors()const override;

private:
	void Spawn( float DeltaTime, const Vec2f &BasePos )override;
	void Remove()override;
	void Collect()override;

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
};

