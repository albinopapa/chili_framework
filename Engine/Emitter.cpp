#include "Emitter.h"


Emitter::Emitter( EmitterData &&DataTemplate )
	:
	EmitterData( std::move( DataTemplate ) )
{
	//m_particles.reserve( maxParticles );
}

Emitter::Emitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles )
	:
	EmitterData( Position, LaunchCount, MaxParticles )
{
	//m_particles.reserve( MaxParticles );
}

void Emitter::SetPosition( const Vec2f &Pos )
{
	position = Pos;
}

void Emitter::SetLaunchCount( size_t Count )
{
	launchCount = Count;
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

