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

	void DrawMaze( RectF const& view)const noexcept;
	void DrawHero( RectF const& view )const noexcept;
	void DrawHUD( RectF const& view )const noexcept;
private:
	static constexpr int ui_height = 128;
	Maze m_maze;
	Camera m_camera;
	Hero m_ranger;
	RectI m_ui_background = { 0,Graphics::GetHeight<int>() - ui_height, Sizei{Graphics::GetWidth<int>(), ui_height } };
	Font m_consolas;
	Vec2i start_index, end_index;
	bool m_success = false;
};

/*
Single screen, tiles, character, lighting, fire particles, hud
*/