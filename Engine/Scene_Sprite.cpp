#include "Scene_Sprite.h"
#include "MathOps.h"

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
	const Vec2f offset = { float( -m_sprite.GetRect().GetCenter().x ), 0.f };
	const auto position = Vec2f{ 400.f,300.f } + offset;
	m_graphics.DrawSprite( Rectf( m_sprite.GetRect() ) + position, m_sprite, 1.f, CopyEffect{ m_graphics } );

}
