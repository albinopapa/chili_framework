#include "FontSheet.h"
#include <cassert>
#include <d2d1.h>
#include <wrl/client.h>
#pragma comment(lib, "d2d1.lib")

using Microsoft::WRL::ComPtr;

FontSheet::FontSheet( const TextFormat & Format, DWriteInitalizer & DWrite, const WicInitializer &Wic )
{
	auto CalculateFontSize = [ &DWrite, &Format ]()
	{
		ComPtr<IDWriteTextLayout> pLayout_H;

		DWrite->CreateTextLayout( L"H", 1, Format.GetFormat(), 32.f, Format->GetFontSize(), &pLayout_H );

		DWRITE_TEXT_METRICS metrics{};
		pLayout_H->GetMetrics( &metrics );
		return Sizei(
			static_cast< int >( std::ceilf( metrics.width ) ),
			static_cast< int >( std::ceilf( metrics.height ) )
		);
	};
	auto GenerateCharList = []()
	{
		std::string charList;
		charList.resize( 127 - 32 );
		char c = ' ';
		std::generate( charList.begin(), charList.end(), [ &c ]()
		{
			return c++;
		} );
		return charList;
	};
	auto CreateWICBitmap = [ this, &Wic ]()
	{
		ComPtr<IWICBitmap> pBitmap;

		Wic.GetFactory()->CreateBitmap(
			m_nCharsPerRow * m_charWidth,
			3 * m_charHeight,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapCacheOnDemand,
			&pBitmap );

		return pBitmap;
	};
	auto CreateD2DFactory = []()
	{
		ComPtr<ID2D1Factory> pFactory2d;
		D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, pFactory2d.GetAddressOf() );
		return pFactory2d;
	};
	auto CreateWICRenderTarget = []( ComPtr<ID2D1Factory> pFactory2d, ComPtr<IWICBitmap> pBitmap )
	{
		ComPtr<ID2D1RenderTarget> pRenderTarget;

		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties();
		props.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
		props.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
		pFactory2d->CreateWicBitmapRenderTarget( pBitmap.Get(), props, &pRenderTarget );
		return pRenderTarget;
	};
	auto CreateD2DSolidBrush = []( ComPtr<ID2D1RenderTarget> pRenderTarget )
	{
		ComPtr<ID2D1SolidColorBrush> pBrush;
		const D2D_COLOR_F color = D2D1::ColorF( D2D1::ColorF::Black );
		pRenderTarget->CreateSolidColorBrush( color, &pBrush );
		return pBrush;
	};
	auto RenderFontSheet = [ this ](
		ComPtr<ID2D1RenderTarget> pRenderTarget,
		ComPtr<ID2D1SolidColorBrush>  pBrush,
		const std::string &charList,
		const TextFormat &Format )
	{
		pRenderTarget->BeginDraw();
		pRenderTarget->Clear( D2D1::ColorF( D2D1::ColorF::White ) );
		const auto wCharList = std::wstring( charList.begin(), charList.end() );
		for( int y = 0; y < 3; ++y )
		{
			for( int x = 0; x < m_nCharsPerRow; ++x )
			{
				const auto index = x + ( y * m_nCharsPerRow );
				const auto c = charList[ index ];
				const auto wc = wCharList[ index ];
				const auto rect = GetCharRect( c );

				pRenderTarget->DrawText(
					&wc, 1, Format.GetFormat(), *reinterpret_cast<const D2D1_RECT_F *>( &rect ), pBrush.Get() );
			}
		}
		pRenderTarget->EndDraw();
	};
	auto GetPixels = [ this ]( ComPtr<IWICBitmap> pBitmap )
	{
		ComPtr<IWICBitmapLock> pLock;
		UINT width, height;
		pBitmap->GetSize( &width, &height );

		auto pPixels = std::make_unique<Color[]>( width * height );
		WICRect wr{ 0, 0, static_cast<int>( width ), static_cast<int>( height ) };
		pBitmap->Lock( &wr, WICBitmapLockRead, &pLock );
		UINT bufferSize = 0;
		Color *pBuffer;
		pLock->GetDataPointer( &bufferSize, reinterpret_cast< WICInProcPointer * >( &pBuffer ) );

		for( int y = 0; y < height; ++y )
		{
			for( int x = 0; x < width; ++x )
			{
				const int index = x + ( y * width );

				pPixels[ index ] = ( pBuffer[ index ] == Colors::White ? Colors::White : Colors::Black );
			}
		}

		return pPixels;
	};

	const auto fontsize = CalculateFontSize();
	m_charWidth = fontsize.width;
	m_charHeight = fontsize.height;
	m_nCharsPerRow = 32;
	const auto charList = GenerateCharList();
	auto pBitmap = CreateWICBitmap();
	auto pFactory2d = CreateD2DFactory();
	auto pRenderTarget = CreateWICRenderTarget( pFactory2d, pBitmap );
	auto pBrush = CreateD2DSolidBrush( pRenderTarget );
	RenderFontSheet( pRenderTarget, pBrush, charList, Format );
	m_pPixels = GetPixels( pBitmap );
}

Color FontSheet::GetPixel( int Idx ) const
{
	assert( Idx < ( m_nCharsPerRow * m_charWidth ) * ( 3 * m_charHeight ) );

	return m_pPixels[ Idx ];
}

Color FontSheet::GetPixel( int X, int Y ) const
{
	return GetPixel( X + ( Y * m_nCharsPerRow * m_charWidth ) );
}

Rectf FontSheet::GetCharRect( const char C ) const
{
	const int fontIndex = C - ' ';

	const int fontCol = fontIndex % m_nCharsPerRow;
	const int fontRow = fontIndex / m_nCharsPerRow;

	const float left = static_cast< float >( fontCol * m_charWidth );
	const float top = static_cast< float >( fontRow * m_charHeight );
	const auto fontSize = Sizef{
		static_cast< float >( m_charWidth ),
		static_cast< float >( m_charHeight )
	};
	return Rectf( { left, top }, fontSize );
}

int FontSheet::GetCharWidth() const
{
	return m_charWidth;
}

