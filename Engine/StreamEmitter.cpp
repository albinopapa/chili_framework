#include "StreamEmitter.h"


StreamEmitter::StreamEmitter(
	EmitterData && DataTemplate,
	const Vec2f &FacingDirection,
	float WidthOfStream )
	:
	Emitter( std::move( DataTemplate ) ),
	m_direction( FacingDirection ),
	m_width( WidthOfStream )
{
}

StreamEmitter::StreamEmitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles, const Vec2f & FacingDirection, float WidthOfStream )
	:
	Emitter( Position, LaunchCount, MaxParticles ),
	m_direction( FacingDirection ),
	m_width( WidthOfStream )
{
}

void StreamEmitter::SpawnParticles( const ParticleSetupDesc & PartDesc )
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

			m_particles.emplace_back( pos, impulse, width, height, ttl,
									PartDesc.drawFunc, PartDesc.color );
		}
	}
}
