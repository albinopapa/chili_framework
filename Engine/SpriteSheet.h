#pragma once

#include "AlphaSprite.h"
#include "AnimatedSprite.h"

class SpriteSheet
{
public:
	SpriteSheet() = default;
	SpriteSheet( int TileWidth, int TileHeight, const std::string &Filename );

	std::unique_ptr<Sprite> CreateSolidSprite( int Column, int Row )const;
	std::unique_ptr<Sprite> CreateSolidSprite( const Recti &Src )const;

	std::unique_ptr<Sprite> CreateAlphaSprite( int Column, int Row )const;
	std::unique_ptr<Sprite> CreateAlphaSprite( const Recti &Src )const;

	Frames CreateFrames( int StartCol, int NumColTiles, int StartRow, int NumRowTiles, Frames::SpriteType Type );
	Frames CreateFrames( const std::vector<Recti> &Src, Frames::SpriteType Type )const;

private:
	std::unique_ptr<Sprite> m_pSpritesheet;
	int m_tileWidth, m_tileHeight;
};

