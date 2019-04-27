#include "Font.h"
#include "Logger.h"
#include "algorithms.h"

#include <sstream>

Font::Font( const TextFormat::Properties &Props )
	:
	m_fontsheet( TextFormat(Props) )
{
}

Sizei Font::GetCharSize()const noexcept
{
	return m_fontsheet.GetCharRect(0).GetSize();
}

int Font::MaxCharsPerRow( int LineWidth )const noexcept
{
	return LineWidth / GetCharSize().width;
}

int Font::MaxCharsPerColumn( int ColumnHeight )const noexcept
{
	return ColumnHeight / GetCharSize().height;
}

std::pair<Font::iterator, Font::iterator> Font::make_char_iterator( char c )const noexcept
{
	const auto rect = RectI( m_fontsheet.GetCharRect( c ) );

	return {
		iterator(  rect.left,    rect.top, rect.right, rect.bottom, m_fontsheet.m_pPixels ),
		iterator( rect.right, rect.bottom, rect.right, rect.bottom, m_fontsheet.m_pPixels )
	};
}

dim2d::surface<Color> const & Font::GetSurface() const noexcept
{
	return m_fontsheet.GetSurface();
}

RectF Font::GetRect( char c )const noexcept
{
	return m_fontsheet.GetCharRect( c );
}
