#include "Chest.h"
#include "MathOps.h"

Chest::Chest( Vec2f const & _position, Sprite const & _sprite, std::optional<InventoryItem> _treasure ) noexcept
	:
	m_position( _position ),
	m_pSprite( std::addressof( _sprite ) ),
	m_treasure( std::move( _treasure ) )
{
}

Rectf Chest::GetRect() const noexcept
{
	return m_rect + m_position;
}

Sprite const & Chest::GetSprite() const noexcept
{
	return *m_pSprite;
}

std::optional<InventoryItem> const& Chest::GetItem() const noexcept
{
	return m_treasure;
}
