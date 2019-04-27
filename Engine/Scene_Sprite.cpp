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
	const Vec2f offset = { float( -m_sprite.GetWidth() / 2 ), 0.f };
	m_graphics.DrawSprite( Graphics::GetRect<float>().Translate( offset ), m_sprite );
}
