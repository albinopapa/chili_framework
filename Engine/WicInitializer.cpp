#include "WicInitializer.h"
#include "DXException.h"
#include "TraceLog.h"

using Microsoft::WRL::ComPtr;
std::unique_ptr<WicInitializer> WicInitializer::s_pInstance;

#define WIC_EXCEPTION(hr, note)DXException(hr, note, _CRT_WIDE( __FILE__), __LINE__);

ComPtr<IWICImagingFactory> CreateFactory()
{
	AutoLogger logger( GetFunctionName() );
	ComPtr<IWICImagingFactory> pFactory;
	HRESULT hr = S_OK;
	if( FAILED( hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS( &pFactory )
	) ) )
	{		
		throw WIC_EXCEPTION( hr, L"Failed to create WIC imaging factory." );
	}

	return pFactory;
}

WicInitializer::WicInitializer()
	:
	m_pFactory( CreateFactory() )
{
}

const WicInitializer & WicInitializer::Instance()
{
	AutoLogger logger( GetFunctionName() );
	if( !s_pInstance )
	{
		s_pInstance = std::make_unique<WicInitializer>();
	}

	return *s_pInstance;
}

IWICImagingFactory * WicInitializer::GetFactory() const
{
	return m_pFactory.Get();
}
