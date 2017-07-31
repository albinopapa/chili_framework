#pragma once

#include "Scene.h"
#include "ParticleEmitter.h"
#include <vector>

class Scene_Particles :public Scene
{
public:
	Scene_Particles( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	void AddBurstEmitter( const Vec2f &Position,
						  const Vec2 &Direction,
						  size_t MaxParticles,
						  float MinEmitterDelay,
						  float MaxEmitterDelay,
						  bool Infinite,
						  float MinParticleRadius,
						  float MaxParticleRadius,
						  float MinParticleTimeToLive,
						  float MaxParticleTimeToLive,
						  Color ParticleColor );
private:
	TrackingCamera m_camera;
	Rectf m_levelrect, m_screenrect;
	Emitter m_baseEmitter;
	std::vector<Emitter> m_burstEmitters;

	static constexpr Color m_colorPalette[ 5 ] = {
		Colors::Red, Colors::Green, Colors::Blue, Colors::Yellow, Colors::Magenta
	};
	std::mt19937 m_rng;
	std::uniform_int_distribution<unsigned int> m_colorDist;
};