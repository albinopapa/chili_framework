#include "Emitter.h"
#include <random>

Emitter::Emitter( const Vec2f &_position, const Vec2f& _direction )
	:
	m_position( _position ),
	m_direction( _direction )
{
}

bool Emitter::IsEnabled() const noexcept
{
	return m_canSpawn;
}

void Emitter::SetPosition( const Vec2f &Pos )noexcept
{
	m_position = Pos;
}

void Emitter::SetDirection( const Vec2f & _direction )noexcept
{
	if( _direction.LenSq() == 1.f )
		m_direction = _direction;
	else
		m_direction = _direction.Normalize();
}

void Emitter::Enable() noexcept
{
	m_canSpawn = true;
}

void Emitter::Disable() noexcept
{
	m_canSpawn = false;
}

std::optional<Particle> Emitter::SpawnParticles( ParticleSetupDesc const& _desc )
{
	return ( m_canSpawn ) ?
		std::optional<Particle>{ std::in_place_t{}, m_position, m_direction, _desc } :
		std::optional<Particle>{};
}
