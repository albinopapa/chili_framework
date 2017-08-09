#include "Scene_Particles.h"

Scene_Particles::Scene_Particles( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_camera( { 400.f, fScreenHeight - 1.f } ),
	m_levelrect( Graphics::fScreenRect ),
	m_screenrect( m_levelrect )
{
	m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
	m_viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );
}

void Scene_Particles::Update( float DeltaTime )
{
	m_fireworkds.Update( DeltaTime );

	for( const auto &vec : m_fireworkds.GetParticleVectors() )
	{
		for( auto &pParticles : *vec )
		{
			pParticles->Update( DeltaTime );
		}
	}
}

void Scene_Particles::Draw() const
{
	for( const auto &vec : m_fireworkds.GetParticleVectors() )
	{
		for( auto &pParticles : *vec )
		{
			pParticles->Draw( m_viewport, m_graphics );
		}
	}
}
