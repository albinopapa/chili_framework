#include "Particle.h"
#include "ChiliMath.h"
#include "Scene.h"
#include "Visitor.h"
#include "Fireworks.h"

ParticleSetupDesc::ParticleSetupDesc( float Speed, float MinRadius, float MaxRadius, float MinTimeToLive, float MaxTimeToLive, Color Clr )
	:
	speed( Speed ), minRadius( MinRadius ), maxRadius( MaxRadius ),
	minTimeToLive( MinTimeToLive ), maxTimeToLive( MaxTimeToLive ),
	color( Clr )
{
}

Particle::Particle(
	const Vec2f & StartPos,
	const Vec2f & StartVelocity,
	float Radius,
	float TimeToLive,
	Color C )
	:
	m_position( StartPos ),
	m_velocity( StartVelocity ),
	m_timeToLive( TimeToLive ),
	m_radius( Radius ),
	m_liveCounter( TimeToLive ),
	m_color( C )
{
}

void Particle::Update( float DeltaTime )
{
	if( IsDead() )return;

	m_position += ( m_velocity * DeltaTime );
	m_liveCounter -= DeltaTime;
}

void Particle::Draw( const Rectf &Viewport, Graphics & Gfx ) const
{
	const auto offset = static_cast<Vec2i>( m_position - Viewport.LeftTop() );
	const auto iRad = std::lroundf( m_radius );

	const float currentStep = Clamp( ( m_liveCounter / m_timeToLive ) * 2.f, 0.f, 1.f );
	const auto charStep = static_cast< unsigned char >( currentStep * 255.f );

	Gfx.DrawCircleAlpha( offset, iRad, m_color * charStep );
}

bool Particle::IsDead()const
{
	return m_liveCounter <= 0.f;
}
