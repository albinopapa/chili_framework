#pragma once

#include "AnimatedSprite.h"


//class SpriteState
//{
//public:
//	SpriteState() = default;
//	
//	virtual void Update( float dt, Direction direction )noexcept = 0;
//protected:
//	void update_sprite( float dt, Direction direction )noexcept;
//	
//protected:
//	AnimationController m_controller;
//	Direction m_direction = Direction::Left;
//};
//
//class IdleSpriteState : public SpriteState
//{
//public:
//	using SpriteState::SpriteState;
//
//	void Update( float dt, Direction direction )noexcept
//	{
//		if( m_direction != direction )
//		{
//			update_sprite( dt, direction );
//		}
//	}
//
//};
//
//class RunningSpriteState : public SpriteState
//{
//public:
//	using SpriteState::SpriteState;
//	void Update( float dt, Direction direction )noexcept override
//	{
//		update_sprite( dt, direction );
//	}
//};
