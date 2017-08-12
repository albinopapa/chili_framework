#include "TextFormat.h"

TextFormat::TextFormat( const Properties & Props )
{
	DWriteInitter::Instance()->CreateTextFormat(
		Props.fontname.c_str(),
		nullptr,
		Props.weight,
		Props.style,
		Props.stretch,
		Props.size,
		Props.locale.c_str(),
		&m_pFormat
	);
}

IDWriteTextFormat * TextFormat::operator->()
{
	return m_pFormat.Get();
}

IDWriteTextFormat * TextFormat::operator->() const
{
	return m_pFormat.Get();
}

IDWriteTextFormat *TextFormat::GetFormat()
{
	return m_pFormat.Get();
}

IDWriteTextFormat * TextFormat::GetFormat() const
{
	return m_pFormat.Get();
}

