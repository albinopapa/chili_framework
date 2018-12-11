#include "Emitter.h"
#include <random>

Emitter::Emitter(
	const Vec2f &_position,
	const Vec2f& _direction,
	int _launchCount,
	int _maxParticles )
	:
	m_position( _position ),
	m_direction( _direction ),
	m_launchCount( _launchCount ),
	m_maxParticles( _maxParticles )
{
}

void Emitter::SetPosition( const Vec2f &Pos )
{
	m_position = Pos;
}

void Emitter::SetDirection( const Vec2f & _direction )
{
	m_direction = _direction;
}

Particle Emitter::SpawnParticles( const ParticleSetupDesc & _desc )
{
	std::mt19937 rng( std::random_device{}( ) );
	std::uniform_int_distribution<int> widthDist( _desc.minWidth, _desc.maxWidth );
	std::uniform_int_distribution<int> heightDist( _desc.minHeight, _desc.maxHeight );
	std::uniform_int_distribution<int> ttlDist( _desc.minTimeToLive, _desc.maxTimeToLive );
	std::uniform_real_distribution<float> speedDist( _desc.minSpeed, _desc.maxSpeed );

	return {
		m_position,
			m_direction * speedDist( rng ),
			widthDist( rng ),
			heightDist( rng ),
			ttlDist( rng ),
			_desc.color
	};
}
