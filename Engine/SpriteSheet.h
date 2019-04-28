#pragma once

#include "AnimatedSprite.h"

class SpriteSheet
{
public:
	SpriteSheet() = default;
	SpriteSheet( int TileWidth, int TileHeight, const std::string &Filename );

	Sprite CreateSolidSprite( int Column, int Row )const;
	Sprite CreateSolidSprite( const Recti &Src )const;

	Frames CreateFrames( int StartCol, int NumColTiles, int StartRow, int NumRowTiles );
	Frames CreateFrames( const std::vector<Recti> &Src )const;
	
private:
	Sprite m_sheet;
	int m_tileWidth, m_tileHeight;
};

