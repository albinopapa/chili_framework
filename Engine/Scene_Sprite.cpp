#include "Scene_Sprite.h"



Scene_Sprite::Scene_Sprite( Keyboard &Kbd, Graphics &Gfx )
	:
	Scene(Kbd, Gfx),
	m_sprite("Images/MesaCanyon_bg.png")
{
}

void Scene_Sprite::Update( float DeltaTime )
{
}

void Scene_Sprite::Draw() const
{
	const Vec2i offset = 
	{ 
		( m_sprite.GetWidth() - Graphics::ScreenWidth ) / 2,
		( m_sprite.GetHeight() - Graphics::ScreenHeight ) 
	};
	const auto rect = Graphics::ScreenRect.Translate( offset );
	m_sprite.Draw( rect, Graphics::ScreenRect, m_graphics );
}
