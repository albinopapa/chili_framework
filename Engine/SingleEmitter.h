#pragma once

#include "Emitter.h"

template<class T>struct uniform_dist {};
template<> struct uniform_dist<int>
{
	uniform_dist( int MinV, int MaxV )
		:
		dist( MinV, MaxV )
	{
	}
	template<class Engine>
	int operator()( Engine &Rng )
	{
		return dist( Rng );
	}

	std::uniform_int_distribution<int> dist;
};
template<> struct uniform_dist<float>
{
	uniform_dist( float MinV, float MaxV )
		:
		dist( MinV, MaxV )
	{
	}
	template<class Engine>
	float operator()( Engine &Rng )
	{
		return dist( Rng );
	}

	std::uniform_real_distribution<float> dist;
};


// Single emitter: emits a single particle after the delay
class SingleEmitter : public Emitter
{
public:
	SingleEmitter() = default;
	SingleEmitter( EmitterData &&DataTemplate, const Vec2f &FacingDirection );
	SingleEmitter(
		const Vec2f &Position,
		size_t LaunchCount,
		size_t MaxParticles,
		const Vec2f &FacingDirection );

	void SetDirection( const Vec2f &Dir )override;

	template<class ParticleType>
	void SpawnParticles( const ParticleSetupDesc &PartDesc )
	{
		if( !CanSpawn() ) return;

		uniform_dist<float> m_ttlDist(
			PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
		uniform_dist<float> m_radiusDist(
			PartDesc.minWidth, PartDesc.maxWidth );

		const Vec2f impulse = m_direction * ( PartDesc.speed );
		const float ttl = m_ttlDist( m_rng );
		const auto radius = m_radiusDist( m_rng );

		m_pParticles.emplace_back(
			std::make_unique<ParticleType>(
				position, impulse, radius, radius, ttl, PartDesc.drawFunc, PartDesc.color ) );
	}

private:
	Vec2f m_direction;
};
