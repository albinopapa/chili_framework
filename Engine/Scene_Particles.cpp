#include "Scene_Particles.h"
#include <future>

Scene_Particles::Scene_Particles( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_camera( { 400.f, Graphics::fScreenHeight - 1.f } ),
	m_levelrect( Graphics::fScreenRect ),
	m_screenrect( m_levelrect ),
	m_chica( "Images/RangerStand00.png" ),
	m_beam( Kbd )
{
	m_chicaPos = static_cast< Vec2f >(
		Vec2i( Graphics::fScreenWidth * .5f, Graphics::fScreenHeight - m_chica.GetHeight() )
		);
	TextFormat::Properties props;
	props.size = 24.f;
	m_consola = Font{ props };
	m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
	m_viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );
}

void Scene_Particles::Update( float DeltaTime )
{
	UpdateFramerateData( m_beam, DeltaTime );
	UpdateEffect( m_beam, DeltaTime, Graphics::fScreenRect.GetCenter() );
	//UpdateFramerateData( m_fireworks, DeltaTime );
	//UpdateEffect( m_fireworks, DeltaTime, { Graphics::fScreenWidth *.5f, Graphics::fScreenHeight } );
	//UpdateFramerateData( m_flames, DeltaTime );
	//UpdateEffect( m_flames, DeltaTime, m_chicaPos );
}

void Scene_Particles::Draw() const
{
	auto DrawFlamesDemo = [ this ]()
	{
		m_chica.Draw(
			static_cast<Rectf>( m_chica.GetRect() ).Translate( m_chicaPos ), m_graphics );
		DrawEffect( m_flames );
	};
	auto DrawFireworksDemo = [ this ]()
	{
		DrawEffect( m_fireworks );
	};
	auto DrawBeamDemo = [ this ]()
	{
		DrawEffect( m_beam );
	};

	DrawBeamDemo();
	//DrawFireworksDemo();
	//DrawFlamesDemo();
	DrawDebugInfo();
}

void Scene_Particles::UpdateEffect( ParticleEffect & Effect, float DeltaTime, const Vec2f &Position )
{
	Effect.Update( DeltaTime, Position );

	for( auto *v : Effect.GetParticleVectors() )
	{
		for( auto &particle : *v )
		{
			particle.Update( DeltaTime );
		}
	}
}

void Scene_Particles::DrawEffect( const ParticleEffect &Effect ) const
{
	for( const auto &vec : Effect.GetParticleVectors() )
	{
		for( auto &particle : *vec )
		{
			if( !Graphics::fScreenRect.Overlaps( particle.GetRect() ) )continue;
			particle.Draw( m_viewport, m_graphics, DrawTransparentCircleEffect{} );
		}
	}
}

void Scene_Particles::UpdateFramerateData(const ParticleEffect &Effect, float DeltaTime )
{
	m_dtCounter += DeltaTime;
	++m_frameCounter;

	if( m_frameCounter >= m_maxFrameCount )
	{
		m_frameCounter = 0u;
		m_dtCounter = 0.f;
	}

	size_t partCount = 0u;
	for( const auto &vec : Effect.GetParticleVectors() )
	{
		for( auto &pParticles : *vec )
		{
			if( !Graphics::fScreenRect.Overlaps( pParticles.GetRect() ) )continue;
			++partCount;
		}
	}

	m_partCount = partCount;
}

void Scene_Particles::DrawDebugInfo() const
{
	const std::string partCountStr =
		"Particle count on screen: " + std::to_string( m_partCount );

	float x = 0.f, y = 0.f;
	const float dt = m_dtCounter / static_cast<float>( m_frameCounter );
	const int fps = std::lroundf( 1.f / dt );
	m_consola.DrawString( { x, y }, "FPS: " + std::to_string( fps ), Colors::Cyan, m_graphics );

	const float fontHeight = static_cast<float>( m_consola.GetCharSize().height );
	m_consola.DrawString( { x, y += fontHeight }, partCountStr, Colors::Cyan, m_graphics );
	m_consola.DrawString( { x, y += fontHeight }, "Press ESC to exit!", Colors::Yellow, m_graphics );
}
