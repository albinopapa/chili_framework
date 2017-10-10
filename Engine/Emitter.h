#pragma once

#include "EmitterData.h"

class Emitter :public EmitterData
{
public:
	Emitter() = default;
	Emitter( EmitterData &&DataTemplate );
	Emitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );

	void SetPosition( const Vec2f &Pos );
	virtual void SetDirection( const Vec2f &Direction ) {}
	void SetLaunchCount( size_t Count );

	void EnableSpawning();
	void DisableSpawning();
	bool CanSpawn()const;


	ParticleVector TakeParticles();

protected:
	ParticleVector m_pParticles;
	bool m_canSpawn = true;

	std::mt19937 m_rng;
};
