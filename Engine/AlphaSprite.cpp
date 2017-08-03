#include "AlphaSprite.h"
#include "TraceLog.h"

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

	return std::make_unique<AlphaSprite>( Clone( Src ) );
}

std::unique_ptr<Sprite> AlphaSprite::CloneMirrored( const Recti &Src ) const
{
	return std::make_unique<AlphaSprite>( ReverseClone( Src ) );
}

void AlphaSprite::Draw( const Rectf & Dst, Graphics & Gfx ) const
{
	Draw( GetRect(), Dst, Gfx );
}

void AlphaSprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto rectified = Rectify( Dst );
	const auto src = rectified.Translate( static_cast<Vec2i>( Src.LeftTop() ) );
	const auto dst = rectified.Translate( static_cast<Vec2i>( Dst.LeftTop() ) );

	if( dst.right < dst.left || dst.bottom < dst.top ) return;

	for( int srcy = src.top, dsty = dst.top; srcy < src.bottom; ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.right; ++srcx, ++dstx )
		{
			Gfx.PutPixelAlpha( dstx, dsty, m_pPixels[ srcx + ( srcy * m_size.width ) ] );
		}
	}
}
