#pragma once

#include "COMInitializer.h"
#include <memory>
#include <wincodec.h>
#include <wrl/client.h>
#pragma comment(lib, "windowscodecs.lib")

class WicInitializer
{
public:
	WicInitializer();
	static const WicInitializer &Instance();
	IWICImagingFactory *GetFactory()const;
private:
	COMInitializer m_com;
	Microsoft::WRL::ComPtr<IWICImagingFactory> m_pFactory;
	static std::unique_ptr<WicInitializer> s_pInstance;
};

