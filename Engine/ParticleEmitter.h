#pragma once

#include "Frametimer.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Particle.h"
#include <cassert>
#include <random>
#include <vector>


// TODO: Make a global unit of measure
// NOTE: Currently hard coded to be 64 pixels per meter

using ParticleVector = std::vector<std::unique_ptr<Particle>>;

struct EmitterData
{
public:
	EmitterData() = default;
	EmitterData( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles )
		:
		position( Position ), launchCount( LaunchCount ), maxParticles( MaxParticles )
	{
	}

public:
	Vec2f position;
	size_t launchCount = 0u;
	size_t maxParticles = 1u;
};

class Emitter :public EmitterData
{	
public:
	Emitter() = default;
	Emitter( EmitterData &&DataTemplate );
	Emitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );
	
	void SetPosition( const Vec2f &Pos );

	void EnableSpawning();
	void DisableSpawning();
	bool CanSpawn()const;

	virtual void SpawnParticles( const ParticleSetupDesc &PartDesc ) = 0;

	ParticleVector TakeParticles();

protected:
	ParticleVector m_particles;
	bool m_canSpawn = true;

	std::mt19937 m_rng;
};

// EMITTER TYPES
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


// Single stream: emits a stream of particles in a specified width and no delay
// Radial emitter: emits particles after delay in all directions
class RadialEmitter :public Emitter
{
public:
	RadialEmitter() = default;
	RadialEmitter( EmitterData &&DataTemplate );
	RadialEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );

	void SpawnParticles( const ParticleSetupDesc &PartDesc )override;

private:
	void InitCommon();
private:
	std::vector<Vec2> m_bursts;

};
// Conical emitter: emits particles clamped to the dot product of the direction the 
				// emitter is facing

// EMITTER ON CONDITIONS
// User input:  Particle emitted when user presses a designated button
// Time based:  Particle emitted when a specified amount of time has elapsed
// Event based: Particle emitted when a game event is processed

// EMITTER OFF CONDITIONS
// User input:  Emitter stops when user presses a designated button
// Time based:  Emitter stops after a specified amount of time has elapsed
// Event based: Emitter stops when a game event is processed