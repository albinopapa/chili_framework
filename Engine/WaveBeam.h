#pragma once

#include "Keyboard.h"
#include "ParticleEffect.h"
#include "Emitter.h"

class WaveBeam :public ParticleEffect
{
public:
	ParticleVector Spawn( float DeltaTime, const Vec2f &BasePos )override;

private:
	Emitter m_emitter;
	ParticleVector m_particles;
	float m_angle = 0.f;
	float m_delayCounter = 0;
	float m_delay;
};

