#include "SpriteCache.h"

SpriteCache::SpriteCache( const WicInitializer &Wic )
	:
	m_character_walk( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png", Wic ),
	m_character_stand( Frames::SpriteType::Alpha, 1, "Images/RangerStand", ".png", Wic ),
	m_background( "Images/MesaCanyon_bg.png", Wic )
{
}
