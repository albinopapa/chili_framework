#include "Scene_Sprite.h"


Scene_Sprite::Scene_Sprite( Keyboard &Kbd, Graphics &Gfx )
	:
	Scene( Kbd, Gfx ),
	//m_sprite("Images/MesaCanyon_bg.png")
	m_sprite( "Images/RangerStand00.png" )
{
}

void Scene_Sprite::Update( float DeltaTime )
{
}

void Scene_Sprite::Draw() const
{
	const Vec2i offset = { -m_sprite.GetWidth() / 2, 0 };
	m_sprite.Draw( Graphics::ScreenRect.Translate( offset ), m_graphics );
}
