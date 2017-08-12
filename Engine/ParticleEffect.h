#pragma once

#include "EmitterData.h"
#include "Vec2.h"

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

protected:
	std::vector<ParticleVector *> m_pParticles;

};

