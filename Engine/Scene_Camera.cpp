#include "Scene_Camera.h"


Scene_Camera::Scene_Camera( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_player( { 8000.f-80.f, 700.f }, Kbd, m_cache ),
	m_camera( m_player.GetPosition() ),
	m_levelrect( static_cast< Rectf >( m_cache.m_background.GetRect() ) ),
	m_screenrect( 0.f, 0.f, Graphics::fScreenWidth, Graphics::fScreenHeight )
{
}

void Scene_Camera::Update( float DeltaTime )
{
	m_player.Update( DeltaTime );
	m_camera.SetTarget( m_player.GetRect().GetCenter() );
	m_camera.Update( DeltaTime );
	m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
}

void Scene_Camera::Draw() const
{
	const auto viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );

	m_cache.m_background.Draw( viewport, m_screenrect, m_graphics );
	m_player.Draw( viewport, m_graphics );
}

