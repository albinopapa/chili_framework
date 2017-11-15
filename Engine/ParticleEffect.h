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

	void Update( float DeltaTime, const Vec2f & BasePos );
	virtual const std::vector<ParticleVector *> &GetParticleVectors()const = 0;
	
protected:
	virtual void Spawn( float DeltaTime, const Vec2f &BasePos ) = 0;
	virtual void Remove() = 0;
	virtual void Collect() = 0;

	template<class ParticleType>
	void RemoveFrom( std::vector<std::unique_ptr<ParticleType>>& pParticles )
	{
		auto endIt = std::remove_if( pParticles.begin(), pParticles.end(),
			[]( const std::unique_ptr<Particle> &pParticle )
		{
			return pParticle->IsDead();
		} );

		pParticles.erase( endIt, pParticles.end() );
	}
protected:
	std::vector<ParticleVector *> m_pParticles;
};

