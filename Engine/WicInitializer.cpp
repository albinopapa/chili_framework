#include "WicInitializer.h"

using Microsoft::WRL::ComPtr;
std::unique_ptr<WicInitializer> WicInitializer::s_pInstance;

ComPtr<IWICImagingFactory> CreateFactory()
{
	ComPtr<IWICImagingFactory> pFactory;
	CoCreateInstance(
		CLSID_WICImagingFactory,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_PPV_ARGS(&pFactory)
	);

	return pFactory;
}

WicInitializer::WicInitializer()
	:
	m_pFactory( CreateFactory() )
{

}

const WicInitializer & WicInitializer::Instance()
{
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
