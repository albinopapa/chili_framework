#include "ParticleEmitter.h"
#include <algorithm>

ConicalEmitter::ConicalEmitter(
	EmitterData &&DataTemplate,
	const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius )
	:
	Emitter( std::move( DataTemplate ) ),
	m_distanceToBase( DistanceToBase ),
	m_baseRadius( BaseRadius )
{
	SetDirection( FacingDirection );
}

ConicalEmitter::ConicalEmitter(
	const Vec2f &Position, size_t LaunchCount, size_t MaxParticles,
	const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius )
	:
	ConicalEmitter( { Position, LaunchCount, MaxParticles }, 
					FacingDirection, DistanceToBase, BaseRadius )
{	
	SetDirection( FacingDirection );
}

ParticleVector ConicalEmitter::SpawnParticles( const ParticleSetupDesc & PartDesc )
{
	ParticleVector particles;

	if( !CanSpawn() )return particles;
	particles.reserve( launchCount );

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

		particles.emplace_back( position, impulse, partWidth, partHeight, ttl, PartDesc.drawFunc, PartDesc.color );
	}

	return particles;

}

void ConicalEmitter::SetDirection( const Vec2f & Direction )
{
	m_direction = ( Direction.LenSq() != 1.f ) ? Direction.Normalize() : Direction;
	
	const Vec2f center = position + ( m_direction * m_distanceToBase );
	const Vec2f leftRotatedDir = { -m_direction.y, m_direction.x };
	m_start = center + ( leftRotatedDir * m_baseRadius );
	m_end = center + ( ( -leftRotatedDir ) * m_baseRadius );
}
