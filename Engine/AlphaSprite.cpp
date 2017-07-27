#include "AlphaSprite.h"


void AlphaSprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.left + dst.GetWidth(); ++srcx, ++dstx )
		{
			Gfx.PutPixelAlpha( dstx, dsty, m_pPixels[ srcx + ( srcy * m_width ) ] );
		}
	}
}

void AlphaSprite::DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left + ( dst.GetWidth() - 1 ), dstx = dst.left; srcx >= src.left; --srcx, ++dstx )
		{
			Gfx.PutPixelAlpha( dstx, dsty, m_pPixels[ srcx + ( srcy * m_width ) ] );
		}
	}
}
