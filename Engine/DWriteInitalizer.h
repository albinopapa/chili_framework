#pragma once

#include <dwrite.h>
#include <wrl/client.h>
#pragma comment(lib, "dwrite.lib")

class DWriteInitalizer
{
public:
	DWriteInitalizer();
	IDWriteFactory *operator->();
private:
	Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;
};
