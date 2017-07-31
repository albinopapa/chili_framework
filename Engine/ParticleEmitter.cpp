#include "ParticleEmitter.h"
#include <algorithm>


Emitter::Emitter( const Particle::Description &PartDesc, const Emitter::Description &EmDesc )
	:
	m_particleDesc( PartDesc ),
	m_emitterDesc( EmDesc ),
	m_rng( std::random_device()( ) ),
	m_ttlDist( PartDesc.m_minParticleTimeToLive, PartDesc.m_maxParticleTimeToLive ),
	m_radiusDist( PartDesc.m_minParticleRadius, PartDesc.m_maxParticleRadius ),
	m_delayDist( EmDesc.m_minDelay, EmDesc.m_maxDelay )
{
}

void Emitter::Update( float DeltaTime, const Rectf &Viewport )
{
	m_delayCounter -= DeltaTime;		
	SpawnParticle();

	for( auto &particle : m_particles )
	{
		particle.Update( DeltaTime );
		particle.KillOffscreen( Viewport );
	}
	
	auto remIter = GetFirstDead();
	while( remIter != m_particles.end() )
	{
		SaveDeadPositions( remIter );
		m_particles.erase( remIter );
		remIter = GetFirstDead();
	}
}

void Emitter::Draw( const Rectf &Viewport, Graphics & Gfx ) const
{
	for( const auto &particle : m_particles )
	{
		particle.Draw( Viewport, Gfx );
	}
}

const std::vector<Vec2f>& Emitter::GetPositionVector() const
{
	return m_deadParticlePositions;
}

void Emitter::ClearPositionVector()
{
	if(!m_deadParticlePositions.empty())
		m_deadParticlePositions.clear();
}

bool Emitter::IsAllDead() const
{
	return m_particles.empty();
}

void Emitter::SpawnParticle()
{
	if( m_particles.size() >= m_emitterDesc.m_maxParticles || m_delayCounter > 0.f || !m_canSpawn ) return;
	if( !m_emitterDesc.m_infinite ) m_canSpawn = false;

	m_delayCounter = m_delayDist( m_rng );

	const Vec2f impulse = m_emitterDesc.m_direction * ( 16.f * 64.f );
	const float ttl = m_ttlDist( m_rng );
	const auto radius = m_radiusDist( m_rng );
	m_particles.push_back( 
		Particle( m_emitterDesc.m_position, impulse, radius, .01f, ttl, true, m_particleDesc.m_particleColor ) );
}

std::vector<Particle>::iterator Emitter::GetFirstDead()
{
	return std::find_if( m_particles.begin(), m_particles.end(), [ this ]( const Particle &P )
	{
		return P.IsDead();
	} );
}

void Emitter::SaveDeadPositions( std::vector<Particle>::iterator FirstDead )
{
	m_deadParticlePositions.push_back( FirstDead->GetPosition() );
}
