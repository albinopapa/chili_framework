#pragma once

#include "Particle.h"
#include <optional>

class Emitter
{
public:
	Emitter() = default;
	Emitter( const Vec2f &_position, const Vec2f& _direction );

	bool IsEnabled()const noexcept;

	void SetPosition( const Vec2f& _position )noexcept;
	void SetDirection( const Vec2f& _direction )noexcept;
	void Enable()noexcept;
	void Disable()noexcept;

	std::optional<Particle> SpawnParticles( const ParticleSetupDesc& _desc );

private:
	Vec2f m_position, m_direction;
	bool m_canSpawn = true;
};
