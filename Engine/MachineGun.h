#pragma once

#include "SingleEmitter.h"
#include "ParticleEffect.h"
#include "Projectile.h"
#include "Keyboard.h"

class MachineGun :
	public ParticleEffect
{
public:
	MachineGun( Keyboard &Kbd );
	const std::vector<ParticleVector *>& GetParticleVectors()const override;

private:
	void Spawn( float DeltaTime, const Vec2f &BasePos )override;
	void Remove()override;
	void Collect()override;

private:
	Keyboard &m_keyboard;
	static constexpr float m_delay = .1f;
	float m_delayCounter = 0.f;
	std::vector<std::unique_ptr<Particle>> m_Particles;
	SingleEmitter m_Emitter;
};

