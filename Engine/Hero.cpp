#include "Hero.h"



Hero::Hero()
	:
	Entity( Graphics::fScreenRect.GetCenter(), { 0.f, 0.f } )
{
}

void Hero::Update( float DeltaTime )
{
	m_fireDelayCounter -= DeltaTime;
	m_position += ( m_velocity * DeltaTime );
}

void Hero::Draw( const Rectf & Viewport, Graphics & Gfx ) const
{
	auto DrawShoulder = [ this, &Gfx ]( float xOffset, int Radius )
	{
		const auto center = static_cast< Vec2i >( m_position + Vec2f( xOffset, 0.f ) );
		const Recti rect = MakeRectFromCenter( center, Sizei( Radius, Radius ) );
		Gfx.DrawCircle( rect, Colors::Orange );
	};

	{
		const auto radius = static_cast< int >( m_torsoThickness * .5f );
		DrawShoulder( m_leftShoulderOffset, radius );
		DrawShoulder( m_rightShoulderOffset, radius );
	}
	
	{
		const auto rect = static_cast< Recti >(
			MakeRectFromCenter( m_position, { m_rightShoulderOffset - m_leftShoulderOffset, m_torsoThickness } )
			);
		Gfx.DrawRect( rect, Colors::Orange );
	}

	{
		const auto rect = static_cast< Recti >(
			MakeRectFromCenter( m_position, { m_headRadius, m_headRadius } )
			);
		Gfx.DrawCircle( rect, Colors::Yellow );
	}
	
}

void Hero::ChangeDirection( const Vec2f & Direction )
{
	m_velocity = Direction * m_speed;
}

bool Hero::Fire()
{
	if( m_fireDelayCounter <= 0.f )
	{
		m_fireDelayCounter = m_fireDelay;
		return true;
	}
	return false;
}

