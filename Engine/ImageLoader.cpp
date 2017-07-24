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

	ComPtr<IWICBitmap> pBitmap;
	Wic.GetFactory()->CreateBitmapFromSource( pFrame.Get(), WICBitmapCacheOnDemand, &pBitmap );

	return pBitmap;
}
