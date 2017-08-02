#include "Sprite.h"
#include <algorithm>
#include "DXException.h"

#define MEM_EXCEPTION(hr, note)DXException(hr, note, _CRT_WIDE(__FILE__),__LINE__ )

Sprite::Sprite( const std::string & Filename )	
{
	auto pBitmap = ImageLoader::Load( Filename );
	m_size = GatherSize( pBitmap );
	m_pPixels = GatherBitmapPixels( pBitmap );
}

Sprite::Sprite( Sprite && Src )
	:
	m_size( Src.m_size ),
	m_pPixels( std::move( Src.m_pPixels ) )
{
	Src.m_size = { 0, 0 };
}

Sprite::~Sprite() = default;

Sprite & Sprite::operator=( Sprite && Src )
{
	m_size = Src.m_size;
	m_pPixels = std::move( Src.m_pPixels );
	Src.m_size = { 0, 0 };

	return *this;
}

std::unique_ptr<Sprite> Sprite::CopyFromRegion( const Recti & Src ) const
{
	const Sizei size = Src.GetSize();
	
	std::unique_ptr<Color[]> pPixels = std::make_unique<Color[]>( Src.GetSize().Area() );
	if( !pPixels )
	{
		throw MEM_EXCEPTION( E_OUTOFMEMORY, L"It appears the image was so large, your computer had to take out a second mortgage and went bankrupt." );
	}
	for( int srcy = Src.top, dsty = 0; srcy < Src.bottom; ++srcy, ++dsty )
	{
		Color *pSrc = &m_pPixels[ srcy * m_size.width ];
		Color *pDst = &pPixels[ dsty * size.width ];
		memcpy( pDst, pSrc, sizeof( Color ) * size.width );
	}

	return std::make_unique<Sprite>( size, std::move( pPixels ) );
}

void Sprite::Draw( const Rectf & Dst, Graphics & Gfx ) const
{
	Draw( GetRect(), Dst, Gfx );
}

void Sprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.left + dst.GetWidth(); ++srcx, ++dstx )
		{
			Gfx.PutPixel( dstx, dsty, m_pPixels[ srcx + ( srcy * m_size.width ) ] );
		}
	}
}

void Sprite::DrawReverse( const Rectf & Dst, Graphics & Gfx ) const
{
	DrawReverse( GetRect(), Dst, Gfx );
}

void Sprite::DrawReverse( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{	
	const auto dst = Rectify( Dst ).Translate( static_cast<Vec2i>( Dst.LeftTop() ) );
	const auto src = GetRect().ClipTo( static_cast< Recti >( Src ) );

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left + ( dst.GetWidth() - 1 ), dstx = dst.left; srcx >= src.left; --srcx, ++dstx )
		{
			Gfx.PutPixel( dstx, dsty, m_pPixels[ srcx + ( srcy * m_size.width ) ] );
		}
	}
}

int Sprite::GetWidth() const
{
	return m_size.width;
}

int Sprite::GetHeight() const
{
	return m_size.height;
}

Recti Sprite::GetRect() const
{
	return Recti( 0, 0, m_size.width, m_size.height );
}

Recti Sprite::Rectify( const Rectf &Src ) const
{
	const auto left = static_cast< int >( Src.left );
	const auto top = static_cast< int >( Src.top );
	return Recti(
		std::max( -left, 0 ),
		std::max( -top, 0 ),
		std::min( Graphics::ScreenWidth - left, m_size.width ),
		std::min( Graphics::ScreenHeight - top, m_size.height )
	);
}

Sprite::Sprite( const Sizei & SrcSize, std::unique_ptr<Color[]>&& pPixels )
	:
	m_size( SrcSize ),
	m_pPixels( std::move( pPixels ) )
{
}

std::unique_ptr<Color[]> Sprite::GatherBitmapPixels( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )const
{
	Microsoft::WRL::ComPtr<IWICBitmapLock> pLock;
	WICRect wr{ 0, 0, m_size.width, m_size.height };
	pBitmap->Lock( &wr, WICBitmapLockRead, &pLock );

	UINT buffSize = 0u;
	Color *pBuffer = nullptr;
	pLock->GetDataPointer( &buffSize, reinterpret_cast< BYTE** >( &pBuffer ) );
	
	std::unique_ptr<Color[]> pPixels = std::make_unique<Color[]>( m_size.Area() );
	if( !pPixels )
	{
		throw MEM_EXCEPTION( E_OUTOFMEMORY, L"It appears the image was so large, your computer had to take out a second mortgage and went bankrupt." );
	}

	for( int y = 0; y < m_size.height; ++y )
	{
		Color *pSrc = &pBuffer[ y * m_size.width ];
		Color *pDst = &pPixels[ y * m_size.width ];
		memcpy( pDst, pSrc, sizeof( Color ) * m_size.width );
	}

	return pPixels;
}

Sizei Sprite::GatherSize( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )
{
	UINT width = 0u, height = 0u;
	pBitmap->GetSize( &width, &height );
	return Sizei(
		static_cast< int >( width ),
		static_cast< int >( height )
	);
}
