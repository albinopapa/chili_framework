#pragma once

#include <memory>
#include <dwrite.h>
#include <wrl/client.h>
#pragma comment(lib, "dwrite.lib")

class DWriteInitalizer
{
public:
	DWriteInitalizer();
	static const DWriteInitalizer &Instance();
	IDWriteFactory *operator->()const;
private:
	Microsoft::WRL::ComPtr<IDWriteFactory> m_pFactory;
	static std::unique_ptr<DWriteInitalizer> s_pInstance;
};
