#pragma once

#include "Camera.h"
#include "Hero.h"
#include "Scene.h"
#include "SpriteState.h"
#include "Maze.h"

#include <array>
#include <cassert>
#include <random>
#include <vector>



class Scene_FeatureDemo : public Scene
{
public:
	Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx )noexcept;
	void Update( float dt )override;
	void Draw()const override;

private:
	void DoWallCollision()noexcept;
	void DoItemCollision()noexcept;

	void DrawMaze( RectF const& view)const noexcept;
	void DrawHero( )const noexcept;
	void DrawHUD( )const noexcept;
private:
	static constexpr int ui_height = 128;
	
	Maze m_maze;
	Camera m_camera;
	Hero m_ranger;
	RectI m_ui_background = { 0,Graphics::GetHeight<int>() - ui_height, Sizei{Graphics::GetWidth<int>(), ui_height } };
	Font m_consolas;
	Vec2i start_index, end_index;
	std::vector<dim2d::index> m_deadends;
	bool m_success = false;
};

/*
lighting, fire particles, enemeies, items
*/