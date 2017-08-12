#pragma once

#include "Keyboard.h"
#include "ParticleEffect.h"
#include "ConicalEmitter.h"

class WaveBeam :public ParticleEffect
{
public:
	WaveBeam( Keyboard &Kbd );

	const std::vector<ParticleVector *> &GetParticleVectors()const override;
private:
	void Spawn( float DeltaTime, const Vec2f &BasePos )override;
	void Remove()override;
	void Collect()override;

private:
	Keyboard &m_keyboard;
	ConicalEmitter m_emitter;
	ParticleVector m_particles;
	float m_angle = 0.f;
	float m_delayCounter = 0;
	float m_delay;
};

