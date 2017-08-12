#pragma once

#include "Emitter.h"

// Radial emitter: emits particles after delay in all directions
class RadialEmitter :public Emitter
{
public:
	RadialEmitter() = default;
	RadialEmitter( EmitterData &&DataTemplate );
	RadialEmitter( const Vec2f &Position, size_t LaunchCount, size_t MaxParticles );

	//void SpawnParticles( const ParticleSetupDesc &PartDesc )override;
	ParticleVector SpawnParticles( const ParticleSetupDesc &PartDesc )override;


private:
	void InitCommon();
private:
	std::vector<Vec2> m_bursts;

};
