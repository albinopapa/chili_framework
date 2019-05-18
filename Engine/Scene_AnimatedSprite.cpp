#include "Scene_AnimatedSprite.h"
#include "MathOps.h"

Scene_AnimatedSprite::Scene_AnimatedSprite( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_walk_right( 30, "Images/RangerWalk", ".png" ),
	m_stand_right( 1, "Images/RangerStandCentered", ".png" ),
	m_animController( 30.f / 900.f, m_stand_right )
{
	const Vec2i startPos = 
	{ 
		Graphics::GetWidth<int>() / 2,
		Graphics::GetHeight<int>() - 1 
	};

	const auto offset = Vec2i{ 
		-m_animController.CurrentFrame().GetWidth() / 2, 
		-m_animController.CurrentFrame().GetHeight() 
	};
	const auto spriteSize = Sizei{ 
		m_animController.CurrentFrame().GetWidth(), 
		m_animController.CurrentFrame().GetHeight() 
	};
	const auto spriteRect = Recti{ offset, spriteSize };
	m_charPosition = static_cast< Vec2f >( startPos );
	m_charRect = static_cast< Rectf >( spriteRect );

}

void Scene_AnimatedSprite::Update( float DeltaTime )
{
	DetermineState();
	UpdateState( DeltaTime );
}

void Scene_AnimatedSprite::Draw() const
{
	// TODO: Implement sprite drawing effects
	const auto charRect = m_charRect + m_charPosition;
	if( m_direction == Direction::Left )
		m_graphics.DrawSprite( charRect, m_animController.CurrentFrame(), MirrorEffect{ m_graphics } );
	else
		m_graphics.DrawSprite( charRect, m_animController.CurrentFrame(), CopyEffect{ m_graphics } );
}

void Scene_AnimatedSprite::DetermineState()
{
	if( auto event = m_keyboard.ReadKey(); event )
	{
		const auto keycode = (*event).GetCode();

		if( ( *event ).IsPress() &&
			m_action == Action::Standing && 
			( keycode == VK_RIGHT || keycode == VK_LEFT ) )
		{
			m_action = Action::Walking;			
			
			if( keycode == VK_RIGHT )
			{
				m_direction = Direction::Right;
				m_animController = AnimationController( 30.f / 900.f, m_walk_right );
			}
			else if( keycode == VK_LEFT )
			{
				m_direction = Direction::Left;
				//m_animController = AnimationController( 30.f / 900.f, m_walk_left );
			}
		}
		else if( ( *event ).IsRelease() )
		{
			if( keycode == VK_LEFT && !m_keyboard.KeyIsPressed( VK_RIGHT ) )
			{
				m_action = Action::Standing;
				//m_animController = AnimationController( 30.f / 900.f, m_stand_left );
			}
			else if( keycode == VK_RIGHT && !m_keyboard.KeyIsPressed( VK_LEFT ) )
			{
				m_action = Action::Standing;
				m_animController = AnimationController( 30.f / 900.f, m_stand_right );
			}
			else if(keycode == VK_LEFT || keycode == VK_RIGHT)
			{ 
				m_direction = keycode == VK_LEFT ? Direction::Left : Direction::Right;
			}
		}
	}
}

void Scene_AnimatedSprite::UpdateState( float DeltaTime )
{
	if( m_action == Action::Walking )
	{
		const float speed = 2.f;
		switch( m_direction )
		{
			case Scene_AnimatedSprite::Direction::Left:
				m_charPosition.x -= speed;
				break;
			case Scene_AnimatedSprite::Direction::Right:
				m_charPosition.x += speed;
				break;
		}
	}

	m_animController.Advance( DeltaTime );
}

