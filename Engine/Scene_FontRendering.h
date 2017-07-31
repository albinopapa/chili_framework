#pragma once

#include "Font.h"
#include "Scene.h"

class Scene_FontRendering :public Scene
{
public:
	Scene_FontRendering( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	Font m_consola;
};

