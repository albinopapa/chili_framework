#include "SpriteSheet.h"


SpriteSheet::SpriteSheet( int TileWidth, int TileHeight, const std::string & Filename )
	:
	m_tileWidth( TileWidth ),
	m_tileHeight( TileHeight ),
	m_pSpritesheet( std::make_unique<AlphaSprite>( Filename ) )
{
}

std::unique_ptr<Sprite> SpriteSheet::CreateSolidSprite( int Column, int Row ) const
{
	const Recti src( 
		Vec2i{Column * m_tileWidth,Row * m_tileHeight}, 
		Sizei{ m_tileWidth, m_tileHeight });

	return CreateSolidSprite( src );
}

std::unique_ptr<Sprite> SpriteSheet::CreateSolidSprite( const Recti & Src ) const
{
	return m_pSpritesheet->CopyFromRegion( Src );
}

std::unique_ptr<Sprite> SpriteSheet::CreateAlphaSprite( int Column, int Row ) const
{
	const Recti src(
		Vec2i{ Column * m_tileWidth, Row * m_tileHeight },
		Sizei{ m_tileWidth, m_tileHeight } );

	return CreateAlphaSprite( src );
}

std::unique_ptr<Sprite> SpriteSheet::CreateAlphaSprite( const Recti & Src ) const
{
	return m_pSpritesheet->CopyFromRegion( Src );
}

Frames SpriteSheet::CreateFrames( int StartCol, int NumColTiles, int StartRow, int NumRowTiles, Frames::SpriteType Type )
{
	std::vector<Recti> rects;
	for( int y = StartRow; y < StartRow + NumRowTiles; ++y )
	{
		for( int x = StartCol; x < StartCol + NumColTiles; ++x )
		{
			Recti rect(
				Vec2i( x * m_tileWidth, y * m_tileWidth ),
				Sizei( m_tileWidth, m_tileHeight )
			);
			rects.push_back( std::move( rect ) );
		}
	}
	
	return CreateFrames( rects, Type );
}

Frames SpriteSheet::CreateFrames( const std::vector<Recti>& Src, Frames::SpriteType Type ) const
{
	std::vector<std::unique_ptr<Sprite>> pSprites( Src.size() );
	if( Type == Frames::SpriteType::Alpha )
	{
		for( size_t i = 0; i < Src.size(); ++i )
		{
			pSprites[ i ] = CreateAlphaSprite( Src[ i ] );
		}
	}
	else
	{
		for( size_t i = 0; i < Src.size(); ++i )
		{
			pSprites[ i ] = CreateSolidSprite( Src[ i ] );
		}
	}

	return Frames( std::move( pSprites ) );
}
