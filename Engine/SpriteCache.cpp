#include "SpriteCache.h"

SpriteCache::SpriteCache()
	:
	m_character_walkR( 30, "Images/RangerWalk", ".png" ),
	//m_character_walkL( m_character_walkR ),
	m_character_standR( 1, "Images/RangerStand", ".png" ),
	//m_character_standL( m_character_standR ),
	m_background( "Images/MesaCanyon_bg.png" )
{
}
