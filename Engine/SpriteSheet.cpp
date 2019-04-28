#include "SpriteSheet.h"
#include "../../Includes/algorithm2d.h"

SpriteSheet::SpriteSheet( int TileWidth, int TileHeight, const std::string & Filename )
	:
	m_tileWidth( TileWidth ),
	m_tileHeight( TileHeight ),
	m_sheet( Filename )
{
}

Sprite SpriteSheet::CreateSolidSprite( int Column, int Row ) const
{
	const Recti src( 
		Vec2i{Column * m_tileWidth,Row * m_tileHeight}, 
		Sizei{ m_tileWidth, m_tileHeight });

	return CreateSolidSprite( src );
}

Sprite SpriteSheet::CreateSolidSprite( const Recti & Src ) const
{
	Sprite result;
	{
		auto& surf = reinterpret_cast< dim2d::surface<Color>& >( result );
		surf = dim2d::surface<Color>( Src.GetWidth(), Src.GetHeight() );
	}
	const auto src =
		dim2d::surface_wrapper( 
			dim2d::offset{ Src.left,Src.top }, 
			Src.GetWidth(), 
			Src.GetHeight(), 
			m_sheet.columns(),
			m_sheet );

	auto dst =
		dim2d::surface_wrapper(
			dim2d::offset{ 0, 0 }, 
			Src.GetWidth(), 
			Src.GetHeight(), 
			result.columns(),
			result );

	dim2d::copy( src.begin(), src.end(), dst.begin() );

	return result;
}

Frames SpriteSheet::CreateFrames( int StartCol, int NumColTiles, int StartRow, int NumRowTiles )
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
	
	return CreateFrames( rects );
}

Frames SpriteSheet::CreateFrames( const std::vector<Recti>& Src ) const
{
	std::vector<Sprite> sprites( Src.size() );
	for( size_t i = 0; i < Src.size(); ++i )
	{
		sprites[ i ] = CreateSolidSprite( Src[ i ] );
	}

	return Frames( std::move( sprites ) );
}
