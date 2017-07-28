#include "DWriteInitalizer.h"

DWriteInitalizer::DWriteInitalizer()
{
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof( IDWriteFactory ),
		reinterpret_cast< IUnknown** >( m_pFactory.GetAddressOf() ) );
}

IDWriteFactory * DWriteInitalizer::operator->()
{
	return m_pFactory.Get();
}

