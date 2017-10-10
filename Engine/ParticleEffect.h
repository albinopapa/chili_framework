#pragma once

#include "EmitterData.h"
#include "Vec2.h"

// HANDLED BY A CHILD OF ParticleEffect CREATED BY END USER
// EMITTER ON/OFF CONDITIONS
// User input:  Emitter enabled/disabled when user presses/releases a designated button
// Time based:  Emitter enabled/disabled when a specified amount of time has elapsed
// Event based: Emitter enabled/disabled when a game event is processed

class ParticleEffect
{
public:
	ParticleEffect() = default;
	virtual ~ParticleEffect() = default;

	void Update( float DeltaTime, const Vec2f & BasePos )
	{
		Spawn( DeltaTime, BasePos );
		Remove();
		Collect();
	}
	virtual const std::vector<ParticleVector *> &GetParticleVectors()const = 0;
	
protected:
	virtual void Spawn( float DeltaTime, const Vec2f &BasePos ) = 0;
	virtual void Remove() = 0;
	virtual void Collect() = 0;
	void RemoveFrom( ParticleVector &pParticles );
protected:
	std::vector<ParticleVector *> m_pParticles;
};

