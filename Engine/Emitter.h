#pragma once

#include "EmitterData.h"

class Emitter :public EmitterData
{
public:
	Emitter() = default;
	Emitter( EmitterData &&DataTemplate );
	Emitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );

	void SetPosition( const Vec2f &Pos );
	void SetLaunchCount( size_t Count );

	void EnableSpawning();
	void DisableSpawning();
	bool CanSpawn()const;

	//virtual void SpawnParticles( const ParticleSetupDesc &PartDesc ) = 0;

	virtual ParticleVector SpawnParticles( const ParticleSetupDesc &PartDesc ) = 0;

	ParticleVector TakeParticles();

protected:
	//ParticleVector m_particles;
	bool m_canSpawn = true;

	std::mt19937 m_rng;
};
