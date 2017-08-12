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
	static const ComponentInitter &Instance()
	{
		AutoLogger logger( GetFunctionName() );
		if( !s_pInstance )
		{
			s_pInstance = std::make_unique<ComponentInitter<Ty>>();
		}

		return *s_pInstance;
	}

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
	static void Release()
	{
		s_pInstance.reset();
	}

protected:
	COMInitializer m_com;
	Microsoft::WRL::ComPtr<Ty> m_pFactory;
	static std::unique_ptr<ComponentInitter> s_pInstance;
};

class WicInitter :public ComponentInitter<IWICImagingFactory>
{
public:
	WicInitter();
};


class DWriteInitter :public ComponentInitter<IDWriteFactory>
{
public:
	DWriteInitter();
};

template<class InitterType>
class InitHandler
{
public:
	InitHandler()
	{
		InitterType::Instance();
	}
	~InitHandler()
	{
		InitterType::Release();
	}
};