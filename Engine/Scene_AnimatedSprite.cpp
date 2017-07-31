#include "Scene_AnimatedSprite.h"


Scene_AnimatedSprite::Scene_AnimatedSprite( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_walk( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png" ),
	m_stand( Frames::SpriteType::Alpha, 1, "Images/RangerStand", ".png" ),
	m_animController( 30.f / 900.f, m_stand )
{
}

void Scene_AnimatedSprite::Update( float DeltaTime )
{
	bool dirChange = false;
	switch( m_direction )
	{
		case Direction::Right:
			if( m_keyboard.KeyIsPressed( VK_LEFT ) )
			{
				m_animController = AnimationController( 30.f / 900.f, m_walk );
			}
			break;
		case Direction::Left:
			if( m_keyboard.KeyIsPressed( VK_RIGHT ) )
			{
				m_animController = AnimationController( 30.f / 900.f, m_walk );
			}
			break;
	}

	m_animController.Advance( DeltaTime );
}

void Scene_AnimatedSprite::Draw() const
{
	const Rectf charRect = { 360.f, 336.f, 440.f, 464.f };
	switch( m_direction )
	{
		case Direction::Right:
			m_animController.Draw( charRect, m_graphics );
			break;
		case Direction::Left:
			m_animController.DrawReverse( charRect, m_graphics );
			break;
	}
}

