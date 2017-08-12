#include "SingleEmitter.h"

template<class T>struct uniform_dist {};
template<> struct uniform_dist<int>
{
	uniform_dist( int MinV, int MaxV )
		:
		dist( MinV, MaxV )
	{
	}
	template<class Engine>
	int operator()(Engine &Rng)
	{
		return dist( Rng );
	}

	std::uniform_int_distribution<int> dist;
};
template<> struct uniform_dist<float>
{
	uniform_dist( float MinV, float MaxV )
		:
		dist( MinV, MaxV )
	{
	}
	template<class Engine>
	float operator()( Engine &Rng )
	{
		return dist( Rng );
	}

	std::uniform_real_distribution<float> dist;
};


SingleEmitter::SingleEmitter(
	EmitterData &&DataTemplate, const Vec2f & FacingDirection )
	:
	Emitter( std::move( DataTemplate ) ),
	m_direction( FacingDirection )
{
}

SingleEmitter::SingleEmitter( const Vec2f & Position, size_t LaunchCount, 
							  size_t MaxParticles, const Vec2f & FacingDirection )
	:
	Emitter( Position, LaunchCount, MaxParticles ),
	m_direction( FacingDirection )
{
}

void SingleEmitter::SpawnParticles( const ParticleSetupDesc &PartDesc )
{
	//ParticleVector particles;
	//if( !CanSpawn() ) return particles;
	if( !CanSpawn() ) return;

	uniform_dist<float> m_ttlDist(
		PartDesc.minTimeToLive, PartDesc.maxTimeToLive );
	uniform_dist<float> m_radiusDist(
		PartDesc.minWidth, PartDesc.maxWidth );

	const Vec2f impulse = m_direction * ( PartDesc.speed );
	const float ttl = m_ttlDist( m_rng );
	const auto radius = m_radiusDist( m_rng );

	m_particles.emplace_back( 
		position, impulse, radius, radius, ttl, PartDesc.drawFunc, PartDesc.color );
	//return particles;
}

