#pragma once

#include "Emitter.h"

// Radial emitter: emits particles after delay in all directions
class RadialEmitter :public Emitter
{
public:
	RadialEmitter() = default;
	RadialEmitter( EmitterData &&DataTemplate );
	RadialEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );

	template<class ParticleType>
	void SpawnParticles( const ParticleSetupDesc &PartDesc )
	{
		if( !CanSpawn() ) return;

		std::uniform_real_distribution<float> m_ttlDist(
			PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
		std::uniform_real_distribution<float> m_radiusDist(
			PartDesc.minWidth, PartDesc.maxWidth );
		std::uniform_real_distribution<float> speedDist( 0.f, PartDesc.speed );

		const float ttl = m_ttlDist( m_rng );
		const auto radius = m_radiusDist( m_rng );

		for( int i = 0; i < launchCount; ++i )
		{
			// Choose how to determine speed
			// Random speed for random positions
			const float speed = speedDist( m_rng );

			// Uncomment to make a ring of particles
			//const float speed = PartDesc.speed;

			const Vec2f impulse = m_bursts[ i ] * ( speed );
			m_pParticles.emplace_back(
				std::make_unique<ParticleType>(
					position, impulse, radius, radius, ttl, PartDesc.drawFunc, PartDesc.color ) );
		}
	}
	
private:
	void InitCommon();

private:
	std::vector<Vec2> m_bursts;
};
