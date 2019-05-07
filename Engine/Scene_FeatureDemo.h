#pragma once

#include "Camera.h"
#include "Hero.h"
#include "Scene.h"
#include "SpriteState.h"
#include "Maze.h"

#include <array>
#include <cassert>
#include <random>



class Scene_FeatureDemo : public Scene
{
public:
	Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx )noexcept;
	void Update( float dt )override;
	void Draw()const override;

private:
	void DoWallCollision()noexcept;
private:
	Maze m_maze;
	Camera m_camera;
	Hero m_ranger;
	dim2d::offset hero_room_index;
};

/*
Single screen, tiles, character, lighting, fire particles, hud
*/