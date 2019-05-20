#pragma once

#include "ItemBase.h"
#include <optional>

class Chest : public ItemBase
{
public:
	Chest() = default;
	Chest( Vec2f const& _position, Sprite const& _sprite, std::optional<InventoryItem> _treasure )noexcept;

	Vec2f const& GetPosition()const noexcept override;
	Rectf GetRect()const noexcept override;
	Rectf GetSpriteRect()const noexcept override;
	Sprite const& GetSprite()const noexcept override;
	
	std::optional<InventoryItem> const& GetItem()const noexcept;

private:
	static constexpr Rectf m_rect = { -32.f,-32.f, 32.f, 0.f };
	static constexpr Vec2f m_sprite_offset = { -32.f, -64.f };
	Sprite const* m_pSprite = nullptr;
	Vec2f m_position;
	std::optional<InventoryItem> m_treasure;
};