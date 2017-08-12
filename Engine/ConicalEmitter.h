#pragma once

#include "Emitter.h"

// Conical emitter: emits particles clamped to the dot product of the direction the 
//					emitter is facing and some other vector.  
class ConicalEmitter :public Emitter
{
public:
	ConicalEmitter() = default;
	ConicalEmitter( EmitterData &&DataTemplate,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );
	ConicalEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );

	void SpawnParticles( const ParticleSetupDesc &PartDesc )override;
	void SetDirection( const Vec2f &Direction );

private:
	Vec2f m_direction, m_start, m_end;
	float m_baseRadius, m_distanceToBase;

};
