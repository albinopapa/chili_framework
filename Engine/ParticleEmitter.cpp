#include "ParticleEmitter.h"
#include <algorithm>



Emitter::Emitter( EmitterData &&DataTemplate )
	:
	EmitterData( std::move( DataTemplate ) )
{
	m_particles.reserve( maxParticles );
}

Emitter::Emitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles )
	:
	EmitterData( Position, LaunchCount, MaxParticles )
{
	m_particles.reserve( MaxParticles );
}

void Emitter::SetPosition( const Vec2f &Pos )
{
	position = Pos;
}

void Emitter::EnableSpawning()
{
	m_canSpawn = true;
}

void Emitter::DisableSpawning()
{
	m_canSpawn = false;
}

bool Emitter::CanSpawn() const
{
	return m_canSpawn;
}

ParticleVector Emitter::TakeParticles()
{
	return std::move( m_particles );
}

SingleEmitter::SingleEmitter(
	EmitterData &&DataTemplate, const Vec2f & FacingDirection )
	:
	Emitter( std::move( DataTemplate ) ),
	m_direction( FacingDirection )
{
}

SingleEmitter::SingleEmitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles, const Vec2f & FacingDirection )
	:
	Emitter( Position, LaunchCount, MaxParticles ),
	m_direction( FacingDirection )
{
}

void SingleEmitter::SpawnParticles( const ParticleSetupDesc &PartDesc )
{
	if( !CanSpawn() ) return;

	std::uniform_real_distribution<float> m_ttlDist( PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
	std::uniform_real_distribution<float> m_radiusDist( PartDesc.minRadius, PartDesc.maxRadius );

	const Vec2f impulse = m_direction * ( PartDesc.speed * 64.f );
	const float ttl = m_ttlDist( m_rng );
	const auto radius = m_radiusDist( m_rng );

#if USE_SMART_POINTER
	m_particles.emplace_back(
		std::make_unique<Particle>( position, impulse, radius, ttl, PartDesc.color ) );
#else
	m_particles.emplace_back( position, impulse, radius, ttl, PartDesc.color );
#endif
	
}

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

void RadialEmitter::SpawnParticles( const ParticleSetupDesc &PartDesc )
{
	if( !CanSpawn() ) return;

	std::uniform_real_distribution<float> m_ttlDist( 
		PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
	std::uniform_real_distribution<float> m_radiusDist( 
		PartDesc.minRadius, PartDesc.maxRadius );
	std::uniform_real_distribution<float> speedDist( 0.f, PartDesc.speed );

	const float ttl = m_ttlDist( m_rng );
	const auto radius = m_radiusDist( m_rng );

	const size_t count = std::min( maxParticles - m_particles.size(), launchCount );
	const size_t growto = m_particles.size() + count;

	if( m_particles.capacity() < growto )
	{
		m_particles.reserve( growto );
	}	

	for( int i = 0; i < launchCount; ++i )
	{
		// Choose how to determine speed
		// Random speed for random positions
		const float speed = speedDist( m_rng );

		// Uncomment to make a ring of particles
		//const float speed = PartDesc.speed;

		const Vec2f impulse = m_bursts[i] * ( speed * 64.f );

#if USE_SMART_POINTER
		m_particles.emplace_back(
			std::make_unique<Particle>( position, impulse, radius, ttl, PartDesc.color ) );
#else
		m_particles.emplace_back( position, impulse, radius, ttl, PartDesc.color );
#endif
	}

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
