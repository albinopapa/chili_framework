#pragma once

#include "Camera.h"
#include "Scene.h"
#include "Maze.h"

#include <array>
#include <cassert>
#include <random>


class Hero
{
public:
	Hero();
	void Update( Keyboard& kbd, float dt );

	Vec2f const& GetPosition()const noexcept;
	Vec2f const& GetVelocity()const noexcept;
	RectF GetRect()const noexcept;
	RectF GetSpriteRect()const noexcept;
	Sprite const& GetSprite()const noexcept;

	void SetPosition( Vec2f _position )noexcept;
	void SetVelocity( Vec2f _velocity )noexcept;
private:
	static constexpr float speed = 160.f;
	Sprite m_sprite;
	Vec2f m_position = Graphics::GetRect<float>().GetCenter();
	Vec2f m_velocity = { 0.f, 0.f };
	static constexpr RectF m_rect = { -16.f, -32.f, 16.f, 0.f };
	static constexpr Vec2f m_sprite_rect_offset = { -25.f, -120.f };
};

constexpr Vec2i operator+( const Vec2i& _left, const dim2d::offset& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}
constexpr Vec2i operator+( const dim2d::offset& _left, const Vec2i& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}
template<typename T>
constexpr _Vec2<T> operator*( const Size_t<T>& _left, const _Vec2<T>& _right )noexcept
{
	return { _left.width * _right.x, _left.height * _right.y };
}
template<typename T>
constexpr _Vec2<T> operator*( const _Vec2<T>& _left, const Size_t<T>& _right )noexcept
{
	return { _left.x * _right.width, _left.y * _right.height };
}

class Scene_FeatureDemo : public Scene
{
public:
	Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx )noexcept;
	void Update( float dt )override;
	void Draw()const override;

private:
	RectI CalculateBoundingRegion(RectF const& hero_rect, dim2d::offset pixel_position)const noexcept;
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