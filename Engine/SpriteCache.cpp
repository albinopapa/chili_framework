#include "SpriteCache.h"

SpriteCache::SpriteCache()
	:
	m_character_walk( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png" ),
	m_character_stand( Frames::SpriteType::Alpha, 1, "Images/RangerStand", ".png" ),
	m_background( "Images/MesaCanyon_bg.png" )
{
}
