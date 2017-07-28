#pragma once

#include "DWriteInitalizer.h"
#include <string>

class TextFormat
{
public:
	struct Properties
	{
		std::wstring fontname = L"Consolas";
		DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL;
		DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL;
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL;
		float size = 16.f;
		std::wstring locale = L"en-us";
	};
public:
	TextFormat() = default;
	TextFormat( const Properties &Props, DWriteInitalizer &DWrite );

	IDWriteTextFormat *operator->();
	IDWriteTextFormat *operator->()const;
	IDWriteTextFormat *GetFormat();
	IDWriteTextFormat *GetFormat()const;
private:
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_pFormat;
};
