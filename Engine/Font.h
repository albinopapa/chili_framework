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
	int MaxCharsPerRow( int LineWidth )const;
	int MaxCharsPerColumn( int ColumnHeight )const;

	void DrawChar( float X, float Y, char C, Color Clr, Graphics &Gfx )const;
	void DrawChar( const Vec2f &Pos, char C, Color Clr, Graphics &Gfx )const;
	void DrawString( float X, float Y, const std::string &Str, Color Clr, Graphics &Gfx )const;
	void DrawString( const Vec2f &Pos, const std::string &Str, Color Clr, Graphics &Gfx )const;
private:
	FontSheet m_fontsheet;
};