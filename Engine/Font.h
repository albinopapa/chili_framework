#pragma once

#include "Graphics.h"
#include "FontSheet.h"
#include <string>

class Font
{
public:
	Font() = default;
	Font( const TextFormat::Properties &Props );

	Sizei GetCharSize()const;
	size_t MaxCharsPerRow( size_t LineWidth )const;
	size_t MaxCharsPerColumn( size_t ColumnHeight )const;

	void DrawChar( float X, float Y, char C, Color Clr, Graphics &Gfx )const;
	void DrawString( float X, float Y, const std::string &Str, Color Clr, Graphics &Gfx )const;

private:
	FontSheet m_fontsheet;
};