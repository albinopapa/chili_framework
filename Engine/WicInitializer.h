#pragma once

#include <wincodec.h>
#include <wrl/client.h>
#pragma comment(lib, "windowscodec.lib")

class WicInitializer
{
public:
	WicInitializer();

	IWICImagingFactory *GetFactory()const;
private:
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_pFactory;
};

