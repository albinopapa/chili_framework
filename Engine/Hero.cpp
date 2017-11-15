#include "Hero.h"



Hero::Hero()
	:
	Entity( Graphics::fScreenRect.GetCenter(), { 0.f, 0.f } ),
	m_orientation( 1.f, 0.f )
{
}

void Hero::Update( float DeltaTime )
{
	m_fireDelayCounter -= DeltaTime;
	m_position += ( m_velocity * DeltaTime );
}

void Hero::Draw( const Rectf & Viewport, Graphics & Gfx ) const
{
	auto DrawShoulder = [ this, &Gfx, &Viewport ]( float xOffset, int Radius )
	{
		const auto center = m_position + Vec2f( xOffset, 0.f );
		const auto size = Sizef( Radius, Radius );

		const Recti rect = static_cast< Recti >( MakeRectFromCenter(
			center, size ).Translate( -Viewport.LeftTop() ) );
		Gfx.DrawCircle( rect, Colors::Orange );
	};

	// Draw shoulders
	{
		const auto radius = static_cast< int >( m_torsoThickness * .5f );
		DrawShoulder( m_leftShoulderOffset, radius );
		DrawShoulder( m_rightShoulderOffset, radius );
	}
	
	// Draw torso
	{
		const auto size = Sizef( m_rightShoulderOffset - m_leftShoulderOffset, m_torsoThickness );
		const auto rect = static_cast< Recti >(
			MakeRectFromCenter( m_position, size ).Translate( -Viewport.LeftTop()
			) );
		Gfx.DrawRect( rect, Colors::Orange );
	}

	// Draw head
	{
		const auto size = Sizef( m_headRadius, m_headRadius );
		const auto rect = static_cast< Recti >(
			MakeRectFromCenter( m_position, size ).Translate( -Viewport.LeftTop() ) );
		Gfx.DrawCircle( rect, Colors::Yellow );
	}	
}

void Hero::ChangeVelocity( const Vec2f & Direction )
{
	m_velocity = Direction * m_speed;
}

void Hero::ChangeOrientation( const Vec2f & Direction )
{
	m_orientation = Direction;
}

Vec2f Hero::Orientation() const
{
	return m_orientation;
}

bool Hero::Fire()
{
	const bool isReady = m_fireDelayCounter <= 0.f;
	m_fireDelayCounter = isReady ? m_fireDelay : m_fireDelayCounter;
	return isReady;
}

