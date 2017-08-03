#pragma once

#include "Scene.h"
#include "AlphaSprite.h"
#include "TraceLog.h"

class Scene_Sprite :public Scene
{
public:
	Scene_Sprite( Keyboard &Kbd, Graphics &Gfx );
	
	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	AlphaSprite m_sprite;

};

