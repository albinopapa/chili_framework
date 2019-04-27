#include "Sprite.h"
#include <algorithm>
#include "DXException.h"
#include "Logger.h"
#include "../../Includes/algorithm2d.h"

Sprite::Sprite( const std::string & Filename )	
{
	AutoLogger logger( GetFunctionName() );
	auto pBitmap = ImageLoader::Load( Filename );
	*this = GatherBitmapPixels( pBitmap );
}

Sprite::Sprite( dim2d::surface<Color> _surface )noexcept
	:
	dim2d::surface<Color>( std::move( _surface ) )
{}

int Sprite::GetWidth() const
{
	return columns();
}

int Sprite::GetHeight() const
{
	return rows();
}

Recti Sprite::GetRect() const
{
	return Recti( Vec2i( 0, 0 ), Sizei( GetWidth(), GetHeight() ) );
}

dim2d::surface<Color> Sprite::GatherBitmapPixels( Microsoft::WRL::ComPtr<IWICBitmap> pBitmap )const
{
	std::uint32_t w = 0u, h = 0u;
	pBitmap->GetSize( &w, &h );
	
	Microsoft::WRL::ComPtr<IWICBitmapLock> pLock;
	auto hr = pBitmap->Lock( nullptr, WICBitmapLockRead, &pLock );
	if( FAILED( hr ) )
		throw std::system_error( hr, std::system_category() );

	UINT buffSize = 0u;
	Color *pBuffer = nullptr;
	pLock->GetDataPointer( &buffSize, reinterpret_cast< BYTE** >( &pBuffer ) );
	
	auto surface = dim2d::surface<Color>( int( w ), int( h ) );
	auto buffer = dim2d::raw_pointer_wrapper( dim2d::offset{ 0, 0 }, int( w ), int( h ), int( w ), pBuffer );

	dim2d::copy( buffer.begin(), buffer.end(), surface.begin() );

	return std::move( surface );
}
