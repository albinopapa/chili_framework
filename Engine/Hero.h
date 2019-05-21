#pragma once

#include "GlobalEnums.h"
#include "Graphics.h"
#include "ItemBase.h"
#include "Keyboard.h"
#include "Rect.h"
#include "SpriteState.h"
#include <vector>

class Hero
{
public:
	Hero();
	void Update( Keyboard& kbd, float dt );

	Vec2f const& GetPosition()const noexcept;
	Vec2f const& GetVelocity()const noexcept;
	Direction GetDirection()const noexcept;
	RectF GetRect()const noexcept;
	RectF GetSpriteRect()const noexcept;
	Sprite const& GetSprite()const noexcept;

	void SetPosition( Vec2f _position )noexcept;
	void SetVelocity( Vec2f _velocity )noexcept;
	void SetDirection( Direction _direction )noexcept;

private:
	enum class State
	{
		idle, running
	};
private:
	static constexpr float speed = 300.f;
	static constexpr float holdTime = .018f;
	static constexpr RectF m_rect = { -16.f, -16.f, 16.f, 0.f };
	static constexpr Vec2f m_sprite_rect_offset = { -64.f, -96.f };
	Vec2f m_position = Graphics::GetRect<float>().GetCenter();
	Vec2f m_velocity = { 0.f, 0.f };
	AnimationController m_animation;
	Direction m_direction = Direction::Left;
	State m_state = State::idle;
	std::vector<InventoryItem> m_items;
};
