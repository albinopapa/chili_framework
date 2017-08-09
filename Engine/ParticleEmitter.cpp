#include "ParticleEmitter.h"
#include <algorithm>



Emitter::Emitter( EmitterData &&DataTemplate )
	:
	EmitterData( std::move( DataTemplate ) )
{
}

Emitter::Emitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles )
	:
	EmitterData( Position, LaunchCount, MaxParticles )
{
}

//Emitter::Emitter(
//	const Particles::Description & PartDesc, 
//	const EmitterData & DataTemplate ) 
//	:
//	EmitterData( DataTemplate ),
//	m_particleDesc( PartDesc ),
//	m_rng( std::random_device()( ) ),
//	m_ttlDist( PartDesc.minTimeToLive, PartDesc.maxTimeToLive ),
//	m_radiusDist( PartDesc.minRadius, PartDesc.maxRadius ),
//	m_delayDist( DataTemplate.minDelay, DataTemplate.maxDelay )
//{
//
//}

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

	m_particles.emplace_back( std::make_unique<Particle>(
		position, impulse, radius, ttl, PartDesc.color ) );
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
	m_particles.reserve( m_particles.size() + count );

	for( int i = 0; i < launchCount; ++i )
	{
		const float speed = speedDist( m_rng );
		//const float speed = PartDesc.speed;
		const Vec2f impulse = m_bursts[i] * ( speed * 64.f );
		m_particles.emplace_back(
			std::make_unique<Particle>(
				position, impulse, radius, ttl, PartDesc.color )
		);
	}

}

void RadialEmitter::InitCommon()
{
	launchCount = std::min( launchCount, maxParticles );
	const auto partCount = launchCount;
	assert( partCount != 0 );

	const float radianStep = ( 3.141592f / 180.f ) * ( 360.f / partCount );
	m_bursts.reserve( partCount );

	for( size_t i = 0u; i < partCount; ++i )
	{
		const auto angle = radianStep * i;
		m_bursts.emplace_back( std::cos( angle ), std::sin( angle ) );
	}
}
