#pragma once

#include "Font.h"
#include "Text.h"
#include "Scene.h"

class Scene_FontRendering :public Scene
{
public:
	Scene_FontRendering( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	Font m_consola;
	Text m_text;
	Text m_marqueeText;
	Vec2f m_marqueePos;
	
	float fps = 0.f;
};

