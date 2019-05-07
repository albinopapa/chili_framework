#include "Character.h"

Character::Character( const Vec2f & StartPos, Keyboard &Kbd, const SpriteCache & Cache )
	:
	m_cache( Cache ),
	m_animController( 30.f / 900.f, m_cache.m_character_standR ),
	m_size( 24.f, 128.f ),
	m_position( StartPos ),
	m_keyboard( Kbd )
{
}

void Character::Update( float DeltaTime )
{
	DoState( Transition(), DeltaTime );
}

void Character::Draw( const Rectf &Viewport, Graphics & Gfx )const
{
	const auto playerrect = GetRect().Translate( -Viewport.LeftTop() );
	Gfx.DrawSprite( playerrect, m_animController.CurrentFrame(), AlphaEffect{ Gfx } );
}

const Vec2f & Character::GetPosition() const
{
	return m_position;
}

Rectf Character::GetRect() const
{
	return Rectf(
		m_position.x - m_size.width * .5f,
		m_position.y - m_size.height,
		m_position.x + m_size.width * .5f,
		m_position.y
	);
}

bool Character::Transition()
{
	bool state_change = false;
	if( m_keyboard.KeyIsPressed( VK_LEFT ) || m_keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		if( m_keyboard.KeyIsPressed( VK_LEFT ) )
		{
			state_change = m_state != State::WalkingLeft;
			m_state = State::WalkingLeft;
		}
		else
		{
			state_change = m_state != State::WalkingRight;
			m_state = State::WalkingRight;
		}
	}
	else
	{
		if( m_state == State::WalkingLeft )
		{
			state_change = true;
			m_state = State::StandingLeft;
		}
		else if( m_state == State::WalkingRight )
		{
			state_change = true;
			m_state = State::StandingRight;
		}
	}
	return state_change;
}

void Character::DoState( bool DidTransition, float DeltaTime )
{
	const auto speed = 1.f;

	switch( m_state )
	{
		case State::StandingLeft:
			if( DidTransition )
			{
				//m_animController = AnimationController( 30.f / 900.f, m_cache.m_character_standL );
			}
			break;
		case State::StandingRight:
			if( DidTransition )
			{
				m_animController = AnimationController( 30.f / 900.f, m_cache.m_character_standR );
			}
			break;
		case State::WalkingLeft:
			if( DidTransition )
			{
				//m_animController = AnimationController( 30.f / 900.f, m_cache.m_character_walkL );
			}
			m_position.x -= speed;
			break;
		case State::WalkingRight:
			if( DidTransition )
			{
				m_animController = AnimationController( 30.f / 900.f, m_cache.m_character_walkR );
			}
			m_position.x += speed;
			break;
	}
	m_animController.Advance( DeltaTime );
}
