#pragma once

#include "Emitter.h"

// Single stream: emits a stream of particles in a specified width and no delay
class StreamEmitter :public Emitter
{
public:
	StreamEmitter() = default;
	StreamEmitter(
		EmitterData &&DataTemplate,
		const Vec2f &FacingDirection,
		float WidthOfStream );
	StreamEmitter(
		const Vec2f &Position,
		size_t LaunchCount,
		size_t MaxParticles,
		const Vec2f &FacingDirection,
		float WidthOfStream );

	void SpawnParticles( const ParticleSetupDesc &PartDesc )override;

private:
	Vec2f m_direction;
	float m_width = 0;
};

