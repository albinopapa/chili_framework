#include "Sprite.h"
#include <algorithm>



Sprite::Sprite( const std::string & Filename, const WicInitializer & Wic )
	:
	m_pBitmap( ImageLoader::Load( Filename, Wic ) ),
	m_width( GatherWidth() ),
	m_height( GatherHeight() ),
	m_pPixels( GatherBitmapPixels() )
{
}

Sprite::~Sprite() = default;

void Sprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.left + dst.GetWidth(); ++srcx, ++dstx )
		{
			Gfx.PutPixel( dstx, dsty, m_pPixels[ srcx + ( srcy * m_width ) ] );
		}
	}
}

void Sprite::DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{	
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left + ( dst.GetWidth() - 1 ), dstx = dst.left; srcx >= src.left; --srcx, ++dstx )
		{
			Gfx.PutPixel( dstx, dsty, m_pPixels[ srcx + ( srcy * m_width ) ] );
		}
	}
}

int Sprite::GetWidth() const
{
	return m_width;
}

int Sprite::GetHeight() const
{
	return m_height;
}

Recti Sprite::GetRect() const
{
	return Recti( 0, 0, m_width, m_height );
}

void Sprite::Rectify( int & xStart, int & xEnd, int & yStart, int & yEnd )const
{
	xEnd = std::min( Graphics::ScreenWidth - xStart, m_width );
	xStart = std::max( -xStart, 0 );
	yEnd = std::min( Graphics::ScreenHeight - yStart, m_height );
	yStart = std::max( -yStart, 0 );
}

Recti Sprite::Rectify( const Rectf &Src ) const
{
	const auto left = static_cast< int >( Src.left );
	const auto top = static_cast< int >( Src.top );
	return Recti(
		std::max( -left, 0 ),
		std::max( -top, 0 ),
		std::min( Graphics::ScreenWidth - left, m_width ),
		std::min( Graphics::ScreenHeight - top, m_height )
	);
}

Color *Sprite::GatherBitmapPixels()const
{
	Microsoft::WRL::ComPtr<IWICBitmapLock> pLock;
	WICRect wr{ 0, 0, m_width, m_height };
	m_pBitmap->Lock( &wr, WICBitmapLockRead, &pLock );

	UINT buffSize = 0u;
	Color *pPixels = nullptr;
	pLock->GetDataPointer( &buffSize, reinterpret_cast< BYTE** >( &pPixels ) );
	
	return pPixels;
}

int Sprite::GatherWidth()const
{
	UINT width = 0u, height = 0u;
	m_pBitmap->GetSize( &width, &height );

	return static_cast<int>( width );
}

int Sprite::GatherHeight()const
{
	UINT width = 0u, height = 0u;
	m_pBitmap->GetSize( &width, &height );

	return static_cast<int>( height );
}
