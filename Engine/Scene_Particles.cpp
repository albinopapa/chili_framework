#include "Scene_Particles.h"

Scene_Particles::Scene_Particles( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_camera( { 400.f, Graphics::fScreenHeight - 1.f } ),
	m_levelrect( Graphics::fScreenRect ),
	m_screenrect( m_levelrect )
{
#if CALCULATE_FPS
	TextFormat::Properties props;
	props.size = 24.f;
	m_consola = Font{ props };
#endif
		m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
	m_viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );
}

void Scene_Particles::Update( float DeltaTime )
{
#if CALCULATE_FPS
	m_dtCounter += DeltaTime;
	++m_frameCounter;

	if( m_frameCounter >= m_maxFrameCount )
	{
		m_frameCounter = 0u;
		m_dtCounter = 0.f;
	}
#endif
	
	m_fireworkds.Update( DeltaTime );

	for( const auto &vec : m_fireworkds.GetParticleVectors() )
	{
		for( auto &pParticles : *vec )
		{
#if USE_SMART_POINTER
			pParticles->Update( DeltaTime );
#else
			pParticles.Update( DeltaTime );
#endif
		}
	}
}

void Scene_Particles::Draw() const
{
#if CALCULATE_FPS
	float dt = m_dtCounter / static_cast<float>( m_frameCounter );
	const int fps = std::lroundf( 1.f / dt );
	const float fontHeight = static_cast<float>( m_consola.GetCharSize().height );
	m_consola.DrawString( { 0.f, 0.f }, "FPS: " + std::to_string( fps ), Colors::Cyan, m_graphics );
	/*Text fpsText( "FPS: " + std::to_string( fps ), Text::TextLayout(), m_viewport, m_consola );

	fpsText.Draw( { 0.f, 0.f }, Colors::Cyan, m_graphics );*/
#endif

	size_t partCount = 0u;
	for( const auto &vec : m_fireworkds.GetParticleVectors() )
	{
		for( auto &pParticles : *vec )
		{
			
#if USE_SMART_POINTER
			if( !Graphics::fScreenRect.Overlaps( pParticles->GetRect() ) )continue;
			pParticles->Draw( m_viewport, m_graphics );
#else
			if( !Graphics::fScreenRect.Overlaps( pParticles.GetRect() ) )continue;
			pParticles.Draw( m_viewport, m_graphics );
#endif
			++partCount;
		}
	}

	const std::string partCountStr =
		"Particle count on screen: " + std::to_string( partCount );
	m_consola.DrawString( { 0.f, fontHeight }, partCountStr, Colors::Cyan, m_graphics );
	m_consola.DrawString( { 0.f, fontHeight * 2.f }, "Press ESC to exit!", Colors::Yellow, m_graphics );
}
