#include "Text.h"



Text::Text()
{
}

Text::Text( const std::string & Str, const TextLayout & Layout, const Rectf &Boundary, const Font & Fnt )
	:
	m_pFont( &Fnt ),
	m_layout( Layout ),
	m_boundary( Boundary ),
	m_baseString( Str )
{
	if( Layout.endofline == EndOfLine::Truncate ) FormatTrunc();
	else if( Layout.endofline == EndOfLine::WordWrap ) FormatWrap();
	else FormatNone();
		

	m_positions.resize( m_strings.size() );

	switch( m_layout.align_v )
	{
		case Alignment_V::Top: AlignTop(); break;
		case Alignment_V::Middle: AlignMiddle(); break;
		case Alignment_V::Bottom: AlignBottom(); break;
	}
	switch( m_layout.align_h )
	{
		case Alignment_H::Left: AlignLeft(); break;
		case Alignment_H::Centered: AlignCenter(); break;
		case Alignment_H::Right: AlignRight(); break;
	}
}

void Text::AlignTop()
{
	const auto maxLines = m_pFont->MaxCharsPerColumn( m_boundary.GetHeight() );
	const auto numLines = std::min( m_strings.size(), maxLines );
	const auto charHeight = m_pFont->GetCharSize().height;
	const auto yStart = m_boundary.top;

	UpdatePositionsY( yStart, numLines, charHeight );
}

void Text::AlignMiddle()
{
	const auto maxLines = m_pFont->MaxCharsPerColumn( m_boundary.GetHeight() );
	const auto numLines = std::min( m_strings.size(), maxLines );
	const auto charHeight = m_pFont->GetCharSize().height;
	const auto textHeight = numLines * charHeight;
	const auto yStart = ( m_boundary.bottom - textHeight ) / 2;

	UpdatePositionsY( yStart, numLines, charHeight );
}

void Text::AlignBottom()
{
	const auto maxLines = m_pFont->MaxCharsPerColumn( m_boundary.GetHeight() );
	const auto numLines = std::min( m_strings.size(), maxLines );
	const auto charHeight = m_pFont->GetCharSize().height;
	const auto textHeight = numLines * charHeight;
	const auto yStart = ( m_boundary.bottom - textHeight );

	UpdatePositionsY( yStart, numLines, charHeight );
}

void Text::AlignLeft()
{
	for( auto & position : m_positions )
	{
		position.x = m_boundary.left;
	}	
}

void Text::AlignCenter()
{
	const auto charSize = m_pFont->GetCharSize();
	for( int i = 0; i < m_strings.size(); ++i )
	{
		const auto lineWidth = static_cast<int>( m_strings[ i ].size() ) * charSize.width;
		m_positions[ i ].x = ( m_boundary.right - lineWidth ) / 2;
	}
}

void Text::AlignRight()
{
	const auto charSize = m_pFont->GetCharSize();
	for( int i = 0; i < m_strings.size(); ++i )
	{
		const auto lineWidth = static_cast<int>( m_strings[ i ].size() ) * charSize.width;
		m_positions[ i ].x = m_boundary.right - lineWidth;
	}
}

Rectf Text::GetActualBoundary() const
{
	const auto charSize = m_pFont->GetCharSize();
	
	const size_t textHeight = m_strings.size() * static_cast< size_t >( charSize.height );
	size_t maxWidth = UINT_MAX;

	for( const auto &str : m_strings )
	{
		const size_t lineWidth = str.length() * static_cast<size_t>( charSize.width ) ;
		maxWidth = std::min( maxWidth, lineWidth );
	}

	return Rectf(
		0.f, 0.f, 
		static_cast< float >( maxWidth ), 
		static_cast< float >( textHeight )
	);
}

Rectf Text::EstimateMinRect( const std::string & Str, const Font & Fnt )
{
	const float strLen = static_cast< float >( Str.length() );
	const float maxCharsPerRow = static_cast< float >( Fnt.MaxCharsPerRow( Graphics::ScreenWidth ) );
	
	const float maxLen = std::min( strLen, maxCharsPerRow );
	const float maxHeight = ceilf( strLen / maxLen );

	const float strPixelLen = maxLen *
		static_cast< float >( Fnt.GetCharSize().width );
	const float strPixelHeight = maxHeight *
		static_cast< float >( Fnt.GetCharSize().height );

	return Rectf( 0.f, 0.f, strPixelLen, strPixelHeight );
}

void Text::Draw( const Vec2f &Position, Color C, Graphics & Gfx ) const
{	
	for( size_t i = 0; i < m_strings.size(); ++i )
	{
		const auto position = static_cast< Vec2f >( m_positions[ i ] ) + Position;
		m_pFont->DrawString( position.x, position.y, m_strings[ i ], C, Gfx );
	}
}

void Text::FormatTrunc()
{
	const auto maxCharsPerRow = m_pFont->MaxCharsPerRow( m_boundary.GetWidth() );
	const auto charSize = m_pFont->GetCharSize();

	constexpr auto nl = '\n';
	constexpr auto tab = '\t';

	int curCharPos = 0;
	std::string str;
	str.reserve( maxCharsPerRow );
	for( const auto &c : m_baseString )
	{
		if( c == nl )
		{
			m_strings.push_back( std::move( str ) );
		}
		else if( c == tab )
		{
			str.insert( curCharPos, 4, ' ' );
			curCharPos += 4;
		}

		if(curCharPos >= maxCharsPerRow )
		{
			curCharPos = 0;
			m_strings.push_back( std::move( str ) );
		}

		str.push_back( c );
		++curCharPos;
	}
}

void Text::FormatWrap()
{
	const auto maxCharsPerRow = m_pFont->MaxCharsPerRow( m_boundary.GetWidth() );
	const auto charSize = m_pFont->GetCharSize();

	constexpr auto nl = '\n';
	constexpr auto tab = '\t';

	int curCharPos = 0;
	int lastSpace = 0;
	std::string str;
	str.reserve( maxCharsPerRow );
	for( const auto &c : m_baseString )
	{
		if( c == nl )
		{
			m_strings.push_back( std::move( str ) );
			curCharPos = 0;
			continue;
		}
		else if( c == tab )
		{
			size_t spaceCount = std::min( maxCharsPerRow - curCharPos, m_layout.tabSpaceCount );
			str.insert( curCharPos, spaceCount, ' ' );
			curCharPos += spaceCount;
			continue;
		}


		if( c == ' ' )
		{
			if( curCharPos == 0 ) continue;
			lastSpace = curCharPos;
		}

		if( curCharPos >= maxCharsPerRow )
		{
			if( curCharPos != lastSpace )
			{
				m_strings.push_back( str.substr( 0, lastSpace ) );
				str = str.substr( lastSpace + 1, std::string::npos );
				curCharPos = str.size();
			}
			else
			{
				curCharPos = 0;
				m_strings.push_back( std::move( str ) );
			}
		}
		
		str.push_back( c );
		++curCharPos;
	}

	m_strings.push_back( std::move( str ) );
}

void Text::FormatNone()
{
	std::string str;
	str.reserve( m_baseString.size() );

	int curCharPos = 0;
	for( const auto c : m_baseString )
	{
		if( c >= ' ' && c <= '~' ) 
		{
			str.push_back( c );
			++curCharPos;
		}
		else if( c == '\n' )
		{
			m_strings.push_back( str );
			curCharPos = 0;
		}
		else if( c == '\t' )
		{
			str.insert( curCharPos, m_layout.tabSpaceCount, ' ' );
			curCharPos += m_layout.tabSpaceCount;
		}
	}

	m_strings.push_back( str );
}

void Text::UpdatePositionsY( size_t yStart, size_t NumLines, size_t CharHeight )
{
	for( size_t i = 0; i < NumLines; ++i )
	{
		m_positions[ i ].y = yStart + ( i * CharHeight );
	}
}

