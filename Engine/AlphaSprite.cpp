#include "AlphaSprite.h"


std::unique_ptr<Sprite> AlphaSprite::CopyFromRegion( const Recti & Src ) const
{
	const Sizei size = Src.GetSize();

	std::unique_ptr<Color[]> pPixels = std::make_unique<Color[]>( Src.GetSize().Area() );

	int x = Src.left;
	for( int srcy = Src.top, dsty = 0; srcy < Src.bottom; ++srcy, ++dsty )
	{
		Color *pSrc = &m_pPixels[ x + ( srcy * m_size.width ) ];
		Color *pDst = &pPixels[ dsty * size.width ];
		memcpy( pDst, pSrc, sizeof( Color ) * size.width );
	}

	return std::make_unique<AlphaSprite>( size, std::move( pPixels ) );
}

void AlphaSprite::Draw( const Rectf & Dst, Graphics & Gfx ) const
{
	Draw( GetRect(), Dst, Gfx );
}

void AlphaSprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.left + dst.GetWidth(); ++srcx, ++dstx )
		{
			Gfx.PutPixelAlpha( dstx, dsty, m_pPixels[ srcx + ( srcy * m_size.width ) ] );
		}
	}
}

void AlphaSprite::DrawReverse( const Rectf & Dst, Graphics & Gfx ) const
{
	DrawReverse( GetRect(), Dst, Gfx );
}

void AlphaSprite::DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left + ( dst.GetWidth() - 1 ), dstx = dst.left; srcx >= src.left; --srcx, ++dstx )
		{
			Gfx.PutPixelAlpha( dstx, dsty, m_pPixels[ srcx + ( srcy * m_size.width ) ] );
		}
	}
}
