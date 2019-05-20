#include "Chest.h"
#include "MathOps.h"

Chest::Chest( Vec2f const & _position, Sprite const & _sprite, std::optional<InventoryItem> _treasure ) noexcept
	:
	m_position( _position ),
	m_pSprite( std::addressof( _sprite ) ),
	m_treasure( std::move( _treasure ) )
{
}

Vec2f const & Chest::GetPosition() const noexcept
{
	return m_position;
}

Rectf Chest::GetRect() const noexcept
{
	return m_rect;
}

Rectf Chest::GetSpriteRect() const noexcept
{
	return Rectf( m_pSprite->GetRect() ) + m_sprite_offset;
}

Sprite const & Chest::GetSprite() const noexcept
{
	return *m_pSprite;
}

std::optional<InventoryItem> const& Chest::GetItem() const noexcept
{
	return m_treasure;
}
