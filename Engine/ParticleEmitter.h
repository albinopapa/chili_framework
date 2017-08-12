#pragma once

// DEV FILE: EMITTER TYPES
#include "Emitter.h"
// Conical emitter: emits particles clamped to the dot product of the direction the 
//					emitter is facing and some other vector.  
//	NOTE:	Vectors at right angles will have a dot product of 0.
//			Vectors over 90 (pi / 2) degrees to each other will have a negative dot product,
//			meaning particles will fire in the opposite direction the emitter is facing
class ConicalEmitter :public Emitter
{
public:
	ConicalEmitter() = default;
	ConicalEmitter( EmitterData &&DataTemplate,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );
	ConicalEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles,
					const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius );

	//void SpawnParticles( const ParticleSetupDesc &PartDesc )override;
	void SetDirection( const Vec2f &Direction );
	ParticleVector SpawnParticles( const ParticleSetupDesc &PartDesc )override;
private:
	Vec2f m_direction, m_start, m_end;
	float m_baseRadius, m_distanceToBase;

};

// HANDLED BY A CHILD OF ParticleEffect CREATED BY END USER
// EMITTER ON/OFF CONDITIONS
// User input:  Emitter enabled/disabled when user presses/releases a designated button
// Time based:  Emitter enabled/disabled when a specified amount of time has elapsed
// Event based: Emitter enabled/disabled when a game event is processed
