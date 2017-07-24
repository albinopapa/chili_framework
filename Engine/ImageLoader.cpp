#include "ImageLoader.h"

using Microsoft::WRL::ComPtr;

ComPtr<IWICBitmap> ImageLoader::Load( const std::string & Filename, const WicInitializer & Wic )
{
	ComPtr<IWICBitmapDecoder> pDecoder;
	Wic.GetFactory()->CreateDecoderFromFilename( 
		std::wstring( Filename.begin(), Filename.end() ).c_str(), 
		nullptr, 
		GENERIC_READ, 
		WICDecodeMetadataCacheOnDemand, 
		&pDecoder );

	ComPtr<IWICBitmapFrameDecode> pFrame;
	pDecoder->GetFrame( 0, &pFrame );

	ComPtr<IWICFormatConverter> pConverter;
	Wic.GetFactory()->CreateFormatConverter( &pConverter );
	pConverter->Initialize( 
		pFrame.Get(), 
		GUID_WICPixelFormat32bppPBGRA, 
		WICBitmapDitherTypeNone, 
		nullptr, 
		1.f, 
		WICBitmapPaletteTypeCustom );

	ComPtr<IWICBitmap> pBitmap;
	Wic.GetFactory()->CreateBitmapFromSource( pConverter.Get(), WICBitmapCacheOnDemand, &pBitmap );

	return pBitmap;
}
