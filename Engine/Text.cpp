#include "Text.h"
#include "Graphics.h"


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
	const auto numLines = std::min( int( m_strings.size() ), maxLines );
	const auto charHeight = m_pFont->GetCharSize().height;
	const auto yStart = m_boundary.top;

	UpdatePositionsY( yStart, numLines, charHeight );
}

void Text::AlignMiddle()
{
	const auto maxLines = m_pFont->MaxCharsPerColumn( m_boundary.GetHeight() );
	const auto numLines = std::min( int( m_strings.size() ), maxLines );
	const auto charHeight = m_pFont->GetCharSize().height;
	const auto textHeight = numLines * charHeight;
	const auto yStart = ( m_boundary.bottom - textHeight ) / 2;

	UpdatePositionsY( yStart, numLines, charHeight );
}

void Text::AlignBottom()
{
	const auto maxLines = m_pFont->MaxCharsPerColumn( m_boundary.GetHeight() );
	const auto numLines = std::min( int( m_strings.size() ), maxLines );
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
	size_t maxWidth = std::numeric_limits<std::uint32_t>::max();

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

Rectf Text::EstimateMinRect( const std::string & Str, const Font & Fnt, RectF const& Bounds )
{
	const float strLen = static_cast< float >( Str.length() );
	const float maxCharsPerRow = 
		static_cast< float >( Fnt.MaxCharsPerRow( int( Bounds.GetWidth() ) ) );
	
	const float maxLen = std::min( strLen, maxCharsPerRow );
	const float maxHeight = ceilf( strLen / maxLen );

	const float strPixelLen = maxLen *
		static_cast< float >( Fnt.GetCharSize().width );
	const float strPixelHeight = maxHeight *
		static_cast< float >( Fnt.GetCharSize().height );

	return Rectf( 0.f, 0.f, strPixelLen, strPixelHeight );
}

Font const & Text::GetFont() const noexcept
{
	return *( m_pFont );
}

std::vector<Vec2i> const & Text::GetStringPositions() const noexcept
{
	return m_positions;
}

std::vector<std::string> const & Text::GetStrings() const noexcept
{
	return m_strings;
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
			str.append( 4, ' ' );
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

	m_strings.push_back( std::move( str ) );
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
			const auto spaceCount = 
				std::min( maxCharsPerRow - curCharPos, m_layout.tabSpaceCount );
			str.append( size_t( spaceCount ), ' ' );
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
				curCharPos = int( str.size() );
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
			m_strings.push_back( std::move( str ) );
			curCharPos = 0;
		}
		else if( c == '\t' )
		{
			str.append( m_layout.tabSpaceCount, ' ' );
			curCharPos += m_layout.tabSpaceCount;
		}
	}

	m_strings.push_back( str );
}

void Text::UpdatePositionsY( int yStart, int NumLines, int CharHeight )
{
	for( int i = 0; i < NumLines; ++i )
	{
		m_positions[ i ].y = yStart + ( i * CharHeight );
	}
}

