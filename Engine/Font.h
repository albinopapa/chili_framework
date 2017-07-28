#pragma once

#include "Colors.h"
#include "FontSheet.h"
#include "Graphics.h"
#include <string>

class Font
{
public:
	Font() = default;
	Font( FontSheet &&Sheet );

	void DrawChar( float X, float Y, char C, Color Clr, Graphics &Gfx )const;
	void DrawString( float X, float Y, const std::string &Str, Color Clr, Graphics &Gfx )const;

private:
	FontSheet m_fontsheet;
};