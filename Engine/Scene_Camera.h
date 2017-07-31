#pragma once

#include "Camera.h"
#include "Character.h"
#include "Scene.h"
#include "SpriteCache.h"

class Scene_Camera :public Scene
{
public:
	Scene_Camera( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	SpriteCache m_cache;
	Character m_player;
	TrackingCamera m_camera;
	Rectf m_screenrect, m_levelrect;
};

