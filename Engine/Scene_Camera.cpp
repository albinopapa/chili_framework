#include "Scene_Camera.h"
#include "MathOps.h"

Scene_Camera::Scene_Camera( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_player( { 8000.f-80.f, 700.f }, Kbd, m_cache ),
	m_camera( m_player.GetPosition(), Graphics::GetRect<float>() ),
	m_levelrect( static_cast< Rectf >( m_cache.m_background.GetRect() ) )
{
}

void Scene_Camera::Update( float DeltaTime )
{
	m_player.Update( DeltaTime );
	m_camera.SetPosition( m_player.GetRect().GetCenter() );
	m_camera.ClampTo( m_levelrect );
}

void Scene_Camera::Draw() const
{
	const auto viewport = m_camera.GetRect() + m_camera.GetPosition();

	m_graphics.DrawSprite( viewport, Graphics::GetRect<float>(), m_cache.m_background, 1.f, CopyEffect{ m_graphics } );
	m_player.Draw( viewport, m_graphics );
}

