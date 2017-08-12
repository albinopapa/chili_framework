#pragma once

#include "ParticleEffect.h"
#include "StreamEmitter.h"
#include "AlphaSprite.h"

class Flames :public ParticleEffect
{
public:
	Flames();

	const std::vector<ParticleVector *> &GetParticleVectors()const override;

private:
	void Spawn( float DeltaTime, const Vec2& BasePos )override;
	void Collect()override;
	void Remove()override;

private:
	StreamEmitter m_water, m_fire;
	ParticleVector m_particles;

	std::vector<Vec2f> m_points;
};

