#pragma once

#include "Scene.h"
#include "AnimatedSprite.h"

class Scene_AnimatedSprite :public Scene
{
	enum class Direction
	{
		Left, Right
	};
	enum class Action
	{
		Standing, Walking
	};

public:
	Scene_AnimatedSprite( Keyboard &Kbd, Graphics &Gfx );

	void Update( float DeltaTime )override;
	void Draw()const override;

private:
	void DetermineState();
	void UpdateState( float DeltaTime );

private:
	Frames m_walk_right, m_walk_left, m_stand_right, m_stand_left;
	AnimationController m_animController;
	Direction m_direction = Direction::Right;
	Action m_action = Action::Standing;
	Vec2f m_charPosition;
	Rectf m_charRect;
};
