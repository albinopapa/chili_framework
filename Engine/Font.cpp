#include "Font.h"
#include "Logger.h"
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

size_t Font::MaxCharsPerRow( size_t LineWidth ) const
{
	return LineWidth / GetCharSize().width;
}

size_t Font::MaxCharsPerColumn( size_t ColumnHeight ) const
{
	return ColumnHeight / GetCharSize().height;
}

void Font::DrawChar( float X, float Y, char C, Color Clr, Graphics & Gfx ) const
{
	const auto ix = static_cast< int >( X );
	const auto iy = static_cast< int >( Y );

	const auto fontRect = static_cast<Recti>( m_fontsheet.GetCharRect( C ) );
	const auto screenRect = 
		Gfx.Rectify( 
			fontRect.
			Translate( -fontRect.LeftTop() ).
			Translate( { ix, iy } ) 
		);

	for( int sy = screenRect.top, fy = fontRect.top + sy; sy < screenRect.bottom; ++sy, ++fy )
	{
		for( int sx = screenRect.left, fx = fontRect.left + sx; sx < screenRect.right; ++sx, ++fx )
		{
			const auto color = m_fontsheet.GetPixel( fx, fy );
			if( color == Colors::Black )
			{
				const auto x = sx + ix;
				const auto y = sy + iy;

				Gfx.PutPixel( x, y, Clr );
			}
		}
	}
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
	const auto maxCharsPerRow = static_cast<int>( MaxCharsPerRow( Graphics::ScreenWidth ) );
	const auto stringWidth = static_cast<int>( ( Str.size() - 1 ) * charWidth );
	const auto stringRect = Gfx.Rectify( Recti( ix, iy, Sizei( stringWidth, charHeight ) ) );

	if( ix < Graphics::ScreenWidth && ix + stringWidth >= 0 &&
		iy < Graphics::ScreenHeight && iy + charHeight >= 0 )
	{
		const int startIndex = std::max( stringRect.left / charWidth, 0 );
		const auto charCount = ( stringRect.right + charWidth ) / charWidth;
		const int endIndex = std::min( charCount, maxCharsPerRow );

		const auto substr = Str.substr( startIndex, endIndex );		

		for( int i = 0; i < static_cast<int>( substr.size() ); ++i )
		{
			const auto xPixelOffset = startIndex * charWidth;
			const auto xCharOffset = i * charWidth;

			const float x = static_cast< float >( ix + xPixelOffset + xCharOffset );
			const float y = static_cast< float >( iy + stringRect.top );

			DrawChar( x, y, substr[ i ], Clr, Gfx );
		}

#ifndef NDEBUG
		auto &logger = VarLogger::Instance( "varlog.txt" ).
			LogMessage( "Frame Vars\n", std::string( "=========================" ) ).
			LogMessage( "ix", ix ).
			LogMessage( "iy", iy ).
			LogMessage( "startIndex", startIndex ).
			LogMessage( "charCount", charCount ).
			LogMessage( "endIndex", endIndex ).
			LogMessage( "substr", substr );

		logger.Flush();
#endif
	}
}

void Font::DrawString( const Vec2f & Pos, const std::string & Str, Color Clr, Graphics & Gfx ) const
{
	DrawString( Pos.x, Pos.y, Str, Clr, Gfx );
}

