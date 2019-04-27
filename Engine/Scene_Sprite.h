#pragma once

#include "Scene.h"
#include "Logger.h"

class Scene_Sprite :public Scene
{
public:
	Scene_Sprite( Keyboard &Kbd, Graphics &Gfx );
	
	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	Sprite m_sprite;
};

