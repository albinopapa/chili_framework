#include "ComponentInitter.h"
#include "DXException.h"

#pragma comment(lib, "windowscodecs.lib")
#pragma comment(lib, "dwrite.lib")

std::unique_ptr<ComponentInitter<IWICImagingFactory>> WicInitter::s_pInstance;
std::unique_ptr<ComponentInitter<IDWriteFactory>> DWriteInitter::s_pInstance;

/////// WIC ComponentInitter
WicInitter::WicInitter()
{
	AutoLogger logger( GetFunctionName() );
	//ComPtr<IWICImagingFactory> pFactory;
	HRESULT hr = S_OK;
	if( FAILED( hr = CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS( &m_pFactory )
	) ) )
	{
		throw WIC_EXCEPTION( hr, L"Failed to create WIC imaging factory." );
	}
}
///////   End 

/////// DWrite ComponentInitter
DWriteInitter::DWriteInitter()
{
	HRESULT hr = S_OK;
	if( FAILED( hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( IDWriteFactory ),
		reinterpret_cast< IUnknown** >( m_pFactory.GetAddressOf() ) ) ) )
	{
		throw DWRITE_EXCEPTION( hr, L"Failed to initialize DWrite factory." );
	}
}
///////   End 
