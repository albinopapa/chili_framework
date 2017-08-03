#include "Scene_FontRendering.h"
#include "TraceLog.h"
#include <algorithm>

Scene_FontRendering::Scene_FontRendering( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx )
{
	TextFormat::Properties props;
	props.fontname = L"Consolas";
	props.size = 36.f;
	m_consola = Font{ props };
}

void Scene_FontRendering::Update( float DeltaTime )
{
}

void Scene_FontRendering::Draw() const
{
	const std::string longString = 
		std::string( "I'm going to write a long string and try to word wrap this shit, like it's as easy as fucking a whore.  " ) +
		std::string( "I'mgoingtowritealongstringandtrytowordwrapthisshit,\nlikeit'saseasyasfuckingawhore." );
	//const std::string longString( "I'mgoingtowritealongstringandtrytowordwrapthisshit,likeit'saseasyasfuckingawhore." );
	const auto longStringSize = longString.size();
	const auto maxCharsPerRow = m_consola.MaxCharsPerRow(Graphics::ScreenWidth);
	std::vector<std::string> strings( ( longStringSize / maxCharsPerRow ) + 1 );
	
	
	// now find the last word that fits on the current row
	int placeHolder = 0;
	for( auto &line : strings )
	{
		auto phrase = longString.substr( placeHolder, maxCharsPerRow );
		size_t indexForNewLine = phrase.find_last_of( ' ' ) + 1;

		if( longString.size() - placeHolder <= maxCharsPerRow || indexForNewLine == 0 )
		{
			indexForNewLine = phrase.size();
		}

		phrase.erase( indexForNewLine, phrase.size() - indexForNewLine );
		placeHolder += phrase.size();

		line = std::move( phrase );
	}

	const auto charHeight = static_cast<float>( m_consola.GetCharSize().height );
	const auto numCharsPerColumn = m_consola.MaxCharsPerColumn( Graphics::ScreenHeight );
	const auto textHeight = charHeight * strings.size();

	const float baseY = static_cast<float>( ( Graphics::ScreenHeight - textHeight ) / 2 );

	float x = 0.f, y = baseY;
	float counter = 0.f;
	for( const auto &line : strings )
	{
		m_consola.DrawString( x, y + ( charHeight * counter ), line, Colors::Red, m_graphics );
		counter += 1.f;
	}

}

