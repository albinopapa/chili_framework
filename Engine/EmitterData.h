#pragma once

#include "Particle.h"
#include <cassert>
#include <memory>
#include <random>
#include <vector>

using ParticleVector = std::vector<std::unique_ptr<Particle>>;

struct EmitterData
{
public:
	EmitterData() = default;
	EmitterData( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );
	virtual ~EmitterData() = default;

public:
	Vec2f position;
	size_t launchCount = 0u;
	size_t maxParticles = 1u;
};

