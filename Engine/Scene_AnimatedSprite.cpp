#include "Scene_AnimatedSprite.h"


Scene_AnimatedSprite::Scene_AnimatedSprite( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_walk_right( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png" ),
	m_walk_left( m_walk_right.CloneMirrored() ),
	m_stand_right( Frames::SpriteType::Alpha, 1, "Images/RangerStandCentered", ".png" ),
	m_stand_left( m_stand_right.CloneMirrored() ),
	m_animController( 30.f / 900.f, m_stand_right )
{
	const Vec2i startPos = 
	{ 
		Graphics::ScreenWidth / 2,
		Graphics::ScreenHeight - 1 
	};

	const auto offset = Vec2i{ -m_animController.GetWidth() / 2, -m_animController.GetHeight() };
	const auto spriteSize = Sizei{ m_animController.GetWidth(), m_animController.GetHeight() };
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
	const auto charRect = m_charRect.Translate( m_charPosition );

	m_animController.Draw( charRect, m_graphics );
}

void Scene_AnimatedSprite::DetermineState()
{
	if( !m_keyboard.KeyIsEmpty() )
	{
		const auto e = m_keyboard.ReadKey();
		const auto keycode = e.GetCode();

		if( e.IsPress() &&
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
				m_animController = AnimationController( 30.f / 900.f, m_walk_left );
			}
		}
		else if( e.IsRelease() )
		{
			if( keycode == VK_LEFT && !m_keyboard.KeyIsPressed( VK_RIGHT ) )
			{
				m_action = Action::Standing;
				m_animController = AnimationController( 30.f / 900.f, m_stand_left );
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

