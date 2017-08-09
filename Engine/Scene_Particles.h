#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Fireworks.h"
#include <memory>
#include <vector>

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
};