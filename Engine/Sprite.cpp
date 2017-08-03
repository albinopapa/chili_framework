#include "Sprite.h"
#include <algorithm>
#include "DXException.h"
#include "TraceLog.h"

#define MEM_EXCEPTION(hr, note)DXException(hr, note, _CRT_WIDE(__FILE__),__LINE__ )

Sprite::Sprite( const std::string & Filename )	
{
	AutoLogger logger( GetFunctionName() );
	auto pBitmap = ImageLoader::Load( Filename );
	m_size = GatherSize( pBitmap );
	m_pPixels = GatherBitmapPixels( pBitmap );
}

Sprite::Sprite( Sprite && Src )
	:
	m_size( Src.m_size ),
	m_pPixels( std::move( Src.m_pPixels ) )
{
	AutoLogger logger( GetFunctionName() );
	Src.m_size = { 0, 0 };
}

Sprite::~Sprite() = default;

Sprite & Sprite::operator=( Sprite && Src )
{
	AutoLogger logger( GetFunctionName() );
	m_size = Src.m_size;
	m_pPixels = std::move( Src.m_pPixels );
	Src.m_size = { 0, 0 };

	return *this;
}

std::unique_ptr<Sprite> Sprite::CopyFromRegion( const Recti & Src ) const
{
	AutoLogger logger( GetFunctionName() );
	return std::make_unique<Sprite>( Clone( Src ) );
}

void Sprite::Draw( const Rectf & Dst, Graphics & Gfx ) const
{
	Draw( GetRect(), Dst, Gfx );
}

void Sprite::Draw( const Rectf &Src, const Rectf &Dst, Graphics & Gfx ) const
{
	const auto rectified = Rectify( Dst );
	const auto src = rectified.Translate( static_cast<Vec2i>( Src.LeftTop() ) );
	const auto dst = rectified.Translate( static_cast<Vec2i>( Dst.LeftTop() ) );

	// No point in even going through the loop if completely off screen
	if( dst.right < dst.left || dst.bottom < dst.top ) return;

	for( int srcy = src.top, dsty = dst.top; srcy < src.top + dst.GetHeight(); ++srcy, ++dsty )
	{
		for( int srcx = src.left, dstx = dst.left; srcx < src.left + dst.GetWidth(); ++srcx, ++dstx )
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
		std::min( Graphics::ScreenWidth - left-1, m_size.width ),
		std::min( Graphics::ScreenHeight - top-1, m_size.height )
	);
}

Sprite::SpriteData Sprite::MakeResource( const Recti & Src ) const
{
	SpriteData result;
	result.m_size = Src.GetSize();
	result.m_pPixels = std::make_unique<Color[]>( result.m_size.Area() );

	if( !result.m_pPixels )
	{
		throw MEM_EXCEPTION( E_OUTOFMEMORY, L"It appears the image was so large, your computer had to take out a second mortgage and went bankrupt." );
	}

	return result;
}

Sprite::SpriteData Sprite::Clone( const Recti &Src ) const
{
	SpriteData result = MakeResource( Src );

	const int stride = result.m_size.width * sizeof( Color );
	for( int y = 0; y < result.m_size.height; ++y )
	{
		int idx = y * result.m_size.width;
		memcpy( &result.m_pPixels[ idx ], &m_pPixels[ idx ], stride );
	}

	return result;
}

Sprite::SpriteData Sprite::ReverseClone( const Recti &Src ) const
{
	SpriteData result = MakeResource( Src );

	for( int y = 0; y < result.m_size.height; ++y )
	{
		const int rowOffset_f = y*result.m_size.width;
		const int rowOffset_r = ( m_size.width - 1 ) + rowOffset_f;

		Color *pDst = &result.m_pPixels[ rowOffset_f ];
		Color *pDstEnd = &result.m_pPixels[ result.m_size.width + rowOffset_f ];
		Color *pSrc = &m_pPixels[ rowOffset_r ];

		for( ; pDst != pDstEnd; ++pDst, --pSrc )
		{
			const auto diff = pDstEnd - pDst;
			*pDst = *pSrc;

			int a = 0;
		}
	}

	return result;
}

std::unique_ptr<Sprite> Sprite::CloneMirrored( const Recti &Src ) const
{
	return std::make_unique<Sprite>( ReverseClone( Src ) );
}

Sprite::Sprite( SpriteData &&Data )
	:
	m_size( Data.m_size ),
	m_pPixels( std::move( Data.m_pPixels ) )
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
