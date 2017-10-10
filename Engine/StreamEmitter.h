#pragma once

#include "Emitter.h"

// Single stream: emits a stream of particles in a specified width and no delay
class StreamEmitter :public Emitter
{
public:
	StreamEmitter() = default;
	StreamEmitter(
		EmitterData &&DataTemplate,
		const Vec2f &FacingDirection,
		float WidthOfStream );
	StreamEmitter(
		const Vec2f &Position,
		size_t LaunchCount,
		size_t MaxParticles,
		const Vec2f &FacingDirection,
		float WidthOfStream );

	template<class ParticleType>
	void SpawnParticles( const ParticleSetupDesc &PartDesc )
	{
		if( CanSpawn() )
		{
			std::uniform_real_distribution<float> m_ttlDist(
				PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
			std::uniform_real_distribution<float> m_widthDist(
				PartDesc.minWidth, PartDesc.maxWidth );
			std::uniform_real_distribution<float> m_heightDist(
				PartDesc.minHeight, PartDesc.maxHeight );

			const float widthStep = ( ( PartDesc.maxWidth + PartDesc.minWidth ) * .5f );
			const float halfWidth = ( widthStep * launchCount ) * .5f;

			const Vec2f impulse = m_direction * ( PartDesc.speed );
			const Vec2f leftRotated = { -m_direction.y, m_direction.x };
			const Vec2f rightRotated = -leftRotated;
			const Vec2f start = position + ( leftRotated * halfWidth );

			for( int i = 0; i < launchCount; ++i )
			{
				const float ttl = m_ttlDist( m_rng );
				const auto width = m_widthDist( m_rng );
				const auto height = m_heightDist( m_rng );

				const float step = static_cast< float >( i ) * widthStep;
				const Vec2f pos = start + ( rightRotated * step );

				m_pParticles.emplace_back( 
					std::make_unique<ParticleType>( 
						pos, impulse, width, height, ttl, PartDesc.drawFunc, PartDesc.color ) );
			}
		}
	}
	void SetDirection( const Vec2f &Direction )override;

private:
	Vec2f m_direction;
	float m_width = 0;
};

