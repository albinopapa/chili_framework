#pragma once

#include "Rect.h"
#include "Sprite.h"

struct ItemBase
{
	virtual Rectf GetRect()const noexcept = 0;
	virtual Sprite const& GetSprite()const noexcept = 0;
};

struct InventoryItem
{
	std::string m_name;
	Sprite const* m_pSprite = nullptr;
};