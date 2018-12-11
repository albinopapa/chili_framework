#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Fireworks.h"
#include "Flames.h"
#include "WaveBeam.h"
#include "Text.h"
#include <memory>
#include <vector>

class Scene_Particles :public Scene
{
public:
	Scene_Particles( Keyboard &Kbd, Graphics &Gfx );
	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	void UpdateEffect( ParticleEffect &Effect, float DeltaTime, const Vec2f &Position );
	void DrawEffect( const ParticleEffect &Effect )const;

	void UpdateFramerateData( const ParticleEffect &Effect, float DeltaTime );
	void DrawDebugInfo()const;
	
private:
	Camera m_camera;
	Rectf m_levelrect, m_screenrect, m_viewport;

	Fireworks m_fireworks;
	Flames m_flames;
	WaveBeam m_beam;
	Font m_consola;

	static constexpr unsigned int m_maxFrameCount = 300u * 60u;
	unsigned int m_frameCounter = 0u;
	float m_dtCounter = 0.f;
	size_t m_partCount = 0u;


	Vec2f m_chicaPos;
	AlphaSprite m_chica;
};