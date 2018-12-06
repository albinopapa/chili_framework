#include "Font.h"
#include "Logger.h"
#include "algorithms.h"

#include <sstream>

Font::Font( const TextFormat::Properties &Props )
	:
	m_fontsheet( TextFormat(Props) )
{
}

Sizei Font::GetCharSize() const
{
	return m_fontsheet.GetCharRect(0).GetSize();
}

int Font::MaxCharsPerRow( int LineWidth ) const
{
	return LineWidth / GetCharSize().width;
}

int Font::MaxCharsPerColumn( int ColumnHeight ) const
{
	return ColumnHeight / GetCharSize().height;
}

void Font::DrawChar( float X, float Y, char C, Color Clr, Graphics & Gfx ) const
{
	const auto ix = static_cast< int >( X );
	const auto iy = static_cast< int >( Y );

	const auto fontRect = static_cast<Recti>( m_fontsheet.GetCharRect( C ) );
	const auto screenRect = Rectify( 
		fontRect
		.Translate( -fontRect.LeftTop() )		// Translate to { 0, 0 }
		.Translate( { ix,iy } ),				// Translate to destination 
		Graphics::ScreenRect );					// Bound to screen rect

	nested_for_each( screenRect, [ & ]( int _x, int _y )
	{
		const int fx = ( _x - ix ) + fontRect.left;
		const int fy = ( _y - iy ) + fontRect.top;
		const auto color = m_fontsheet.GetPixel( fx, fy );
		if( color == Colors::Black )
		{
			const auto x = _x + ix;
			const auto y = _y + iy;

			Gfx.PutPixel( x, y, Clr );
		}
	} );
}

void Font::DrawChar( const Vec2f & Pos, char C, Color Clr, Graphics & Gfx ) const
{
	DrawChar( Pos.x, Pos.y, C, Clr, Gfx );
}

void Font::DrawString( float X, float Y, const std::string & Str, Color Clr, Graphics & Gfx ) const
{
	const auto ix = static_cast< int >( X );
	const auto iy = static_cast< int >( Y );
	const auto charWidth = m_fontsheet.GetCharWidth();
	const auto charHeight = m_fontsheet.GetCharHeight();
	const auto maxCharsPerRow = MaxCharsPerRow( Graphics::ScreenWidth );
	const auto stringWidth = static_cast<int>( ( Str.size() - 1 ) * charWidth );
	const auto stringRect = Rectify( 
		Recti( ix, iy, Sizei( stringWidth, charHeight ) ), Graphics::ScreenRect );

	if( Graphics::IsInView( { ix,iy,Sizei{stringWidth,charHeight} } ) )
	{
		const int startIndex = std::max( stringRect.left / charWidth, 0 );
		const auto charCount = ( stringRect.right + charWidth ) / charWidth;
		const int endIndex = std::min( charCount, maxCharsPerRow );

		for( int i = 0, j = startIndex; j < endIndex; ++i, ++j )
		{
			const auto xPixelOffset = startIndex * charWidth;
			const auto xCharOffset = i * charWidth;

			const float x = static_cast< float >( ix + xPixelOffset + xCharOffset );
			const float y = static_cast< float >( iy + stringRect.top );

			DrawChar( x, y, Str[ j ], Clr, Gfx );
		}
	}
}

void Font::DrawString( const Vec2f & Pos, const std::string & Str, Color Clr, Graphics & Gfx ) const
{
	DrawString( Pos.x, Pos.y, Str, Clr, Gfx );
}

