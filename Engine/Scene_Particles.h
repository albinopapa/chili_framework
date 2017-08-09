#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Fireworks.h"
#include "Text.h"
#include <memory>
#include <vector>

#define CALCULATE_FPS 1
class Scene_Particles :public Scene
{
public:
	Scene_Particles( Keyboard &Kbd, Graphics &Gfx );
	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	StaticCamera m_camera;
	Rectf m_levelrect, m_screenrect, m_viewport;

	Fireworks m_fireworkds;
#if CALCULATE_FPS
	Font m_consola;

	static constexpr unsigned int m_maxFrameCount = 300u * 60u;
	unsigned int m_frameCounter = 0u;
	float m_dtCounter = 0.f;
#endif
};