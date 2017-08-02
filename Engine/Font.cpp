#ifndef WINLOADED
#define WINLOADED
#endif
#include "Font.h"

Font::Font( const TextFormat::Properties &Props )
	:
	m_fontsheet( TextFormat(Props) )
{
}

void Font::DrawChar( float X, float Y, char C, Color Clr, Graphics & Gfx ) const
{
	const auto rect = static_cast<Recti>( m_fontsheet.GetCharRect( C ) );

	for( int y = rect.top; y < rect.bottom; ++y )
	{
		for( int x = rect.left; x < rect.right; ++x )
		{
			const auto color = m_fontsheet.GetPixel( x, y );
			if( color == Colors::Black )
			{
				Gfx.PutPixel( static_cast< int >( X ) + ( x - rect.left ),
							  static_cast< int >( Y ) + ( y - rect.top ),
							  Clr );
			}
		}
	}
}

void Font::DrawString( float X, float Y, const std::string & Str, Color Clr, Graphics & Gfx ) const
{
	const auto charWidth = m_fontsheet.GetCharWidth();
	for( int i = 0; i < Str.size(); ++i )
	{
		DrawChar( X + ( i * charWidth ), Y, Str[ i ], Clr, Gfx );
	}
}

