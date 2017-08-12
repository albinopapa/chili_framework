#pragma once

#include "Emitter.h"

// Single emitter: emits a single particle after the delay
class SingleEmitter : public Emitter
{
public:
	SingleEmitter() = default;
	SingleEmitter( EmitterData &&DataTemplate, const Vec2f &FacingDirection );
	SingleEmitter(
		const Vec2f &Position,
		size_t LaunchCount,
		size_t MaxParticles,
		const Vec2f &FacingDirection );

	void SpawnParticles( const ParticleSetupDesc &PartDesc )override;

private:
	Vec2f m_direction;
};
