#include "Particle.h"
#include "ChiliMath.h"


Vec2f Particle::s_gravity = { 0.f, ( 9.8f * 64.f ) };

Particle::Particle(
	const Vec2f & StartPos,
	const Vec2f & StartVelocity,
	float Radius,
	float DragCoeff,
	float TimeToLive,
	bool IsAffectedByGravity,
	Color C )
	:
	m_position( StartPos ),
	m_velocity( StartVelocity ),
	m_timeToLive( TimeToLive ),
	m_radius( Radius ),
	m_affectedByGravity( IsAffectedByGravity ),
	m_liveCounter( TimeToLive ),
	m_dragcoeff( DragCoeff ),
	m_color( C )
{
}

void Particle::Update( float DeltaTime )
{
	m_position += ( m_velocity * DeltaTime );

	if( m_affectedByGravity )
	{		
		const auto gravAffect = s_gravity * DeltaTime;

		const Vec2f drag = m_velocity * ( -m_dragcoeff );
		const Vec2f accel = gravAffect + drag;

		m_velocity += accel;
	}

	m_liveCounter -= DeltaTime;
}

void Particle::Draw( const Rectf &Viewport, Graphics & Gfx ) const
{
	const auto offset = static_cast<Vec2i>( m_position - Viewport.LeftTop() );
	const auto iRad = std::lroundf( m_radius );
	Gfx.DrawCircleAlpha( offset, iRad, m_color );
}

void Particle::KillOffscreen( const Rectf & Viewport )
{
	const auto offset = static_cast<Vec2i>( m_position - Viewport.LeftTop() );
	const auto iRad = std::lroundf( m_radius );
	const auto rect = MakeRectFromCenter( offset, Sizei( iRad * 2, iRad * 2 ) );

	if( ( rect.left < 0 && m_velocity.x < 0.f ) ||
		( rect.right >= Graphics::ScreenWidth && m_velocity.x > 0.f ) ||
		( rect.top < 0 && m_velocity.y < 0.f ) ||
		( rect.bottom >= Graphics::ScreenHeight && m_velocity.y > 0.f ) )
	{
		m_liveCounter = 0.f;
	}
}

bool Particle::IsDead() const
{
	if( m_liveCounter <= 0.f )
		int a = 0;
	return m_liveCounter <= 0.f;
}

Vec2f Particle::GetPosition() const
{
	return m_position;
}
