#pragma once

#include "FontSheet.h"

class Font
{
public:
	using iterator = dim2d::const_index_iterator<dim2d::surface<Color>>;

public:
	Font() = default;
	Font( const TextFormat::Properties &Props );

	Sizei GetCharSize()const noexcept;
	int MaxCharsPerRow( int LineWidth )const noexcept;
	int MaxCharsPerColumn( int ColumnHeight )const noexcept;
	dim2d::surface<Color> const& GetSurface()const noexcept;
	RectF GetRect( char c )const noexcept;
private:
	FontSheet m_fontsheet;
};

