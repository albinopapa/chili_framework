#include "Sprite.h"
#include <algorithm>



Sprite::Sprite( const std::string & Filename, const WicInitializer & Wic )
	:
	m_pBitmap( ImageLoader::Load( Filename, Wic ) ),
	m_width( GatherWidth() ),
	m_height( GatherHeight() ),
	m_pLock( Lock() )
{
}

Sprite::~Sprite() = default;

void Sprite::Draw( int OffsetX, int OffsetY, Graphics & Gfx ) const
{
	int xStart = OffsetX, yStart = OffsetY, xEnd = OffsetX + m_width, yEnd = OffsetY + m_height;
	Rectify( xStart, xEnd, yStart, yEnd );

	Color *pPixels = nullptr;
	UINT buffSize = sizeof( Color ) * static_cast< UINT >( m_width * m_height );
	m_pLock->GetDataPointer( &buffSize, reinterpret_cast< BYTE** >( &pPixels ) );

	for( auto y = yStart; y < yEnd; ++y )
	{
		for( auto x = xStart; x < xEnd; ++x )
		{
			Gfx.PutPixel( x + OffsetX, y + OffsetY, pPixels[ x + ( y * m_width ) ] );
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

Microsoft::WRL::ComPtr<IWICBitmapLock> Sprite::Lock()const
{
	Microsoft::WRL::ComPtr<IWICBitmapLock> pLock;
	WICRect wr{ 0, 0, m_width, m_height };
	m_pBitmap->Lock( &wr, WICBitmapLockRead, &pLock );
	return pLock;
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
