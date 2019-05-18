#pragma once

#include "ItemBase.h"
#include <optional>

class Chest : public ItemBase
{
public:
	Chest() = default;
	Chest( Vec2f const& _position, Sprite const& _sprite, std::optional<InventoryItem> _treasure )noexcept;

	Rectf GetRect()const noexcept;
	Sprite const& GetSprite()const noexcept;
	std::optional<InventoryItem> const& GetItem()const noexcept;

private:
	static constexpr Rectf m_rect = { -32.f,-32.f, 32.f, 0.f };
	Sprite const* m_pSprite = nullptr;
	Vec2f m_position;
	std::optional<InventoryItem> m_treasure;
};