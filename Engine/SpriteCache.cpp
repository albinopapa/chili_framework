#include "SpriteCache.h"

SpriteCache::SpriteCache()
	:
	m_character_walkR( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png" ),
	m_character_walkL( m_character_walkR.CloneMirrored() ),
	m_character_standR( Frames::SpriteType::Alpha, 1, "Images/RangerStand", ".png" ),
	m_character_standL( m_character_standR.CloneMirrored() ),
	m_background( "Images/MesaCanyon_bg.png" )
{
}
