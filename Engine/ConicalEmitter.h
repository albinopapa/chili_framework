#pragma once

#include "Emitter.h"

// Conical emitter: emits particles clamped to the dot product of the direction the 
//					emitter is facing and some other vector.  
class ConicalEmitter :public Emitter
{
public:
	ConicalEmitter() = default;
	ConicalEmitter( EmitterData &&DataTemplate,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );
	ConicalEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );

	template<class ParticleType>
	void SpawnParticles( const ParticleSetupDesc &PartDesc )
	{
		if( !CanSpawn() )return;

		std::uniform_real_distribution<float> wDist( PartDesc.minWidth, PartDesc.maxWidth );
		std::uniform_real_distribution<float> hDist( PartDesc.minHeight, PartDesc.maxHeight );
		std::uniform_real_distribution<float> ttlDist( PartDesc.minTimeToLive, PartDesc.maxTimeToLive );

		const float partWidth = wDist( m_rng );
		const float partHeight = hDist( m_rng );
		const float ttl = ttlDist( m_rng );
		const Vec2f range = m_end - m_start;
		const Vec2f step = range * ( launchCount > 0 ? 1.f / launchCount : 0.f );

		for( int i = 0; i < launchCount; ++i )
		{
			const Vec2f spawnPos = m_start + ( step * static_cast< float >( i ) );
			const Vec2f impulse = ( spawnPos - position ).Normalize() * PartDesc.speed;

			m_pParticles.emplace_back(
				std::make_unique<ParticleType>(
					position, impulse, partWidth, partHeight, ttl, PartDesc.drawFunc, PartDesc.color ) );
		}
	}
	void SetDirection( const Vec2f &Direction )override;

private:
	Vec2f m_direction, m_start, m_end;
	float m_baseRadius, m_distanceToBase;

};
