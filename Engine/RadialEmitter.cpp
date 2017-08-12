#include "RadialEmitter.h"


RadialEmitter::RadialEmitter( EmitterData &&DataTemplate )
	:
	Emitter( std::move( DataTemplate ) )
{
	InitCommon();
}

RadialEmitter::RadialEmitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles )
	:
	Emitter( Position, LaunchCount, MaxParticles )
{
	InitCommon();
}

ParticleVector RadialEmitter::SpawnParticles( const ParticleSetupDesc &PartDesc )
{
	ParticleVector particles;
	if( !CanSpawn() ) return particles;

	std::uniform_real_distribution<float> m_ttlDist(
		PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
	std::uniform_real_distribution<float> m_radiusDist(
		PartDesc.minWidth, PartDesc.maxWidth);
	std::uniform_real_distribution<float> speedDist( 0.f, PartDesc.speed );

	const float ttl = m_ttlDist( m_rng );
	const auto radius = m_radiusDist( m_rng );

	const size_t count = std::min( maxParticles - particles.size(), launchCount );
	const size_t growto = particles.size() + count;

	if( particles.capacity() < growto )
	{
		particles.reserve( growto );
	}

	for( int i = 0; i < launchCount; ++i )
	{
		// Choose how to determine speed
		// Random speed for random positions
		const float speed = speedDist( m_rng );

		// Uncomment to make a ring of particles
		//const float speed = PartDesc.speed;

		const Vec2f impulse = m_bursts[ i ] * ( speed );
		particles.emplace_back( position, impulse, radius, radius, ttl, PartDesc.drawFunc, PartDesc.color );
	}

	return particles;
}

void RadialEmitter::InitCommon()
{
	if( launchCount > maxParticles )
	{
		std::swap( launchCount, maxParticles );
	}

	m_bursts.reserve( launchCount );

	const float radianStep = ( 3.141592f / 180.f ) * ( 360.f / launchCount );
	for( size_t i = 0u; i < launchCount; ++i )
	{
		const auto angle = radianStep * i;
		m_bursts.emplace_back( std::cos( angle ), std::sin( angle ) );
	}
}
