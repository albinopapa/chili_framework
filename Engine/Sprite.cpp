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

void Sprite::Draw( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	const auto offsetX = std::lroundf( OffsetX );
	const auto offsetY = std::lroundf( OffsetY );

	int xStart = offsetX, yStart = offsetY, xEnd = offsetX + m_width, yEnd = offsetY + m_height;
	Rectify( xStart, xEnd, yStart, yEnd );

	for( auto y = yStart; y < yEnd; ++y )
	{
		for( auto x = xStart; x < xEnd; ++x )
		{
			Gfx.PutPixel( x + OffsetX, y + OffsetY, m_pPixels[ x + ( y * m_width ) ] );
		}
	}
}

void Sprite::DrawReverse( float OffsetX, float OffsetY, Graphics & Gfx ) const
{
	const auto offsetX = std::lroundf( OffsetX );
	const auto offsetY = std::lroundf( OffsetY );

	int xStart = offsetX, yStart = offsetY, xEnd = offsetX + m_width, yEnd = offsetY + m_height;
	Rectify( xStart, xEnd, yStart, yEnd );

	for( auto y = yStart; y < yEnd; ++y )
	{
		for( auto rx = xEnd - 1, x = 0; rx >= xStart; --rx, ++x )
		{
			Gfx.PutPixel( x + offsetX, y + offsetY, m_pPixels[ rx + ( y * m_width ) ] );
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

void Sprite::Rectify( int & xStart, int & xEnd, int & yStart, int & yEnd )const
{
	xStart = std::max( -xStart, 0 );
	xEnd = std::min( Graphics::ScreenWidth - xStart, m_width );
	yStart = std::max( -yStart, 0 );
	yEnd = std::min( Graphics::ScreenHeight - yStart, m_height );
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
