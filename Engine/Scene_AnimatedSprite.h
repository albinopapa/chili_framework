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
	Frames m_walk, m_stand;
	AnimationController m_animController;
	Direction m_direction = Direction::Right;
	Action m_action = Action::Standing;
};
