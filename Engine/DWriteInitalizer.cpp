#include "DWriteInitalizer.h"
#include "DXException.h"

#define DWRITE_EXCEPTION( hr,note ) DXException( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

std::unique_ptr<DWriteInitalizer> DWriteInitalizer::s_pInstance;

DWriteInitalizer::DWriteInitalizer()
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

const DWriteInitalizer & DWriteInitalizer::Instance()
{
	if( !s_pInstance )
	{
		s_pInstance = std::make_unique<DWriteInitalizer>();
	}

	return *s_pInstance;
}

IDWriteFactory * DWriteInitalizer::operator->()const
{
	return m_pFactory.Get();
}
