#pragma once

#include "COMInitializer.h"
#include <wincodec.h>
#include <wrl/client.h>
#pragma comment(lib, "windowscodecs.lib")

class WicInitializer
{
public:
	WicInitializer();

	IWICImagingFactory *GetFactory()const;
private:
	COMInitializer m_com;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_pFactory;
};

