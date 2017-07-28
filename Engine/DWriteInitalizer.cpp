#include "DWriteInitalizer.h"

std::unique_ptr<DWriteInitalizer> DWriteInitalizer::s_pInstance;

DWriteInitalizer::DWriteInitalizer()
{
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( IDWriteFactory ),
		reinterpret_cast< IUnknown** >( m_pFactory.GetAddressOf() ) );
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

