#include "AlphaSprite.h"


void AlphaSprite::Draw( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	const auto offsetX = std::lroundf( OffsetX );
	const auto offsetY = std::lroundf( OffsetY );

	int xStart = offsetX, yStart = offsetY, xEnd = offsetX + m_width, yEnd = offsetY + m_height;
	Rectify( xStart, xEnd, yStart, yEnd );

	for( auto y = yStart; y < yEnd; ++y )
	{
		for( auto x = xStart; x < xEnd; ++x )
		{
			Gfx.PutPixelAlpha( x + OffsetX, y + OffsetY, m_pPixels[ x + ( y * m_width ) ] );
		}
	}
}

void AlphaSprite::DrawReverse( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	const auto offsetX = std::lroundf( OffsetX );
	const auto offsetY = std::lroundf( OffsetY );

	int xStart = offsetX, yStart = offsetY, xEnd = offsetX + m_width, yEnd = offsetY + m_height;
	Rectify( xStart, xEnd, yStart, yEnd );

	for( auto y = yStart; y < yEnd; ++y )
	{
		for( auto rx = xEnd - 1, x = 0; rx >= xStart; --rx, ++x )
		{
			Gfx.PutPixelAlpha( x + offsetX, y + offsetY, m_pPixels[ rx + ( y * m_width ) ] );
		}
	}
}
