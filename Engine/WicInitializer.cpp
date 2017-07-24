#include "WicInitializer.h"

using Microsoft::WRL::ComPtr;

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

IWICImagingFactory * WicInitializer::GetFactory() const
{
	return m_pFactory.Get();
}
