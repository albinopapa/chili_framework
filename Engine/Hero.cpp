#include "Hero.h"
#include "SpriteResource.h"
#include "MathOps.h"
Hero::Hero()
{
	m_animation = AnimationController( holdTime, sprites.hero_idle[ int( m_direction ) ] );
}

void Hero::Update( Keyboard& _keyboard, float dt )
{
	Vec2f delta;
	if( _keyboard.KeyIsPressed( VK_LEFT ) )
	{
		delta.x = -1.f;
	}
	else if( _keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		delta.x = 1.f;
	}
	if( _keyboard.KeyIsPressed( VK_UP ) )
	{
		delta.y = -1.f;
	}
	else if( _keyboard.KeyIsPressed( VK_DOWN ) )
	{
		delta.y = 1.f;
	}

	Direction direction = m_direction;
	State state = State::idle;

	if( delta.x == -1.f && delta.y == -1.f )
	{
		// Left up
		direction = Direction::LeftUp;
		state = State::running;
	}
	else if( delta.x == -1.f &&  delta.y == 0.f )
	{
		// Left
		direction = Direction::Left;
		state = State::running;
	}
	else if( delta.x == -1.f && delta.y == 1.f )
	{
		// Left down
		direction = Direction::LeftDown;
		state = State::running;
	}
	else if( delta.x == 0.f && delta.y == -1.f )
	{
		// Up
		direction = Direction::Up;
		state = State::running;
	}
	else if( delta.x == 0.f && delta.y == 1.f )
	{
		// Down
		direction = Direction::Down;
		state = State::running;
	}
	else if( delta.x == 1.f && delta.y == -1.f )
	{
		// Right up
		direction = Direction::RightUp;
		state = State::running;
	}
	else if( delta.x == 1.f && delta.y == 0.f )
	{
		// Right
		direction = Direction::Right;
		state = State::running;
	}
	else if( delta.x == 1.f && delta.y == 1.f )
	{
		// Right down
		direction = Direction::RightDown;
		state = State::running;
	}

	switch( m_state )
	{
		case Hero::State::idle:
			if(state == State::running)
			{
				m_animation = AnimationController( holdTime, sprites.hero_running[ int( direction ) ] );
				m_state = state;
			}
			break;
		case Hero::State::running:
			if( state == State::idle )
			{
				m_animation = AnimationController( holdTime, sprites.hero_idle[ int( direction ) ] );
				m_state = state;
				m_direction = direction;
			}
			else if( direction != m_direction )
			{
				m_animation = AnimationController(
					holdTime,
					sprites.hero_running[ int( direction ) ],
					m_animation.CurrentFrameIndex() );
				m_direction = direction;
			}
			break;
		default:
			break;
	}
	
	dt = std::min( dt, .016f );
	m_animation.Advance( dt );
	m_velocity = delta.Normalize() * ( speed * dt );

	m_position += m_velocity;
}

Vec2f const & Hero::GetPosition() const noexcept
{
	return m_position;
}

Vec2f const & Hero::GetVelocity() const noexcept
{
	return m_velocity;
}

Direction Hero::GetDirection() const noexcept
{
	return m_direction;
}

RectF Hero::GetRect() const noexcept
{
	return m_rect;
}

RectF Hero::GetSpriteRect() const noexcept
{
	return RectF( m_animation.CurrentFrame().GetRect() ) + m_sprite_rect_offset;
}

Sprite const & Hero::GetSprite() const noexcept
{
	return m_animation.CurrentFrame();
}

void Hero::SetPosition( Vec2f _position ) noexcept
{
	m_position = _position;
}

void Hero::SetVelocity( Vec2f _velocity ) noexcept
{
	m_velocity = _velocity;
}

void Hero::SetDirection( Direction _direction ) noexcept
{
	m_direction = _direction;
}
