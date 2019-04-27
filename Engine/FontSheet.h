#pragma once

#include "TextFormat.h"
#include "Colors.h"
#include "Rect.h"
#include "../../Includes/surface.h"
#undef GetCharWidth

class FontSheet
{
public:
	FontSheet() = default;
	FontSheet( const TextFormat &Format );

	Color GetPixel( int X, int Y )const;
	Rectf GetCharRect( const char C )const;
	int GetCharWidth()const;
	int GetCharHeight()const;
	const dim2d::surface<Color> & GetSurface()const noexcept;
private:
	friend class Font;
	int m_nCharsPerRow = 0, m_charWidth = 0, m_charHeight = 0;
	dim2d::surface<Color> m_pPixels;
};
