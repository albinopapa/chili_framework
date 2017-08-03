#include "ImageLoader.h"
#include "DXException.h"
#include "TraceLog.h"

#define WIC_EXCEPTION( hr,note ) DXException( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

ComPtr<IWICBitmap> ImageLoader::Load( const std::string & Filename )
{
	AutoLogger logger( GetFunctionName() );

	HRESULT hr = S_OK;
	ComPtr<IWICBitmapDecoder> pDecoder;
	if( FAILED( hr = WicInitializer::Instance().GetFactory()->CreateDecoderFromFilename(
		std::wstring( Filename.begin(), Filename.end() ).c_str(),
		nullptr,
		GENERIC_READ,
		WICDecodeMetadataCacheOnDemand,
		&pDecoder ) ) )
	{
		std::wstring wFilename( Filename.begin(), Filename.end() );
		throw WIC_EXCEPTION( hr, L"Failed to load image file: " + wFilename );
	}

	ComPtr<IWICBitmapFrameDecode> pFrame;
	if( FAILED( hr = pDecoder->GetFrame( 0, &pFrame ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to get the frame from image file." );
	}

	ComPtr<IWICFormatConverter> pConverter;
	if( FAILED( hr = WicInitializer::Instance().GetFactory()->CreateFormatConverter( &pConverter ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to create format converter." );
	}
	if(FAILED(hr = pConverter->Initialize(
		pFrame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		1.f,
		WICBitmapPaletteTypeCustom ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to initizlie format converter." );
	}

	ComPtr<IWICBitmap> pBitmap;
	if( FAILED( hr = WicInitializer::Instance().GetFactory()->CreateBitmapFromSource(
		pConverter.Get(),
		WICBitmapCacheOnDemand,
		&pBitmap ) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to create bitmap." );
	}

	return pBitmap;
}
