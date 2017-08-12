#pragma once

#include "COMInitializer.h"
#include "Logger.h"
#include <wrl/client.h>
#include <memory>
#include <dwrite.h>
#include <wincodec.h>

#define WIC_EXCEPTION(hr, note)DXException(hr, note, _CRT_WIDE( __FILE__), __LINE__);
#define DWRITE_EXCEPTION( hr,note ) DXException( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

template<class Ty>
class ComponentInitter
{
public:
	Ty *GetFactory()const
	{
		return m_pFactory.Get();
	}
	Ty *operator->()
	{
		return GetFactory();
	}
	Ty *operator->()const
	{
		return GetFactory();
	}


protected:
	COMInitializer m_com;
	Microsoft::WRL::ComPtr<Ty> m_pFactory;
};

class WicInitter :public ComponentInitter<IWICImagingFactory>
{
public:
	WicInitter();
	static const WicInitter &Instance();
	static void Release();

private:
	static std::unique_ptr<WicInitter> s_pInstance;

};


class DWriteInitter :public ComponentInitter<IDWriteFactory>
{
public:
	DWriteInitter();
	static const DWriteInitter &Instance();
	static void Release();

private:
	static std::unique_ptr<DWriteInitter> s_pInstance;

};
