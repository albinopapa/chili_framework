#include "Scene_FontRendering.h"
#include "Logger.h"
#include <sstream>
#include <algorithm>

Scene_FontRendering::Scene_FontRendering( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx )
{
	TextFormat::Properties props;
	props.fontname = L"Consolas";
	props.size = 18.f;
	m_consola = Font{ props };

	const auto longString = std::string( "\tI'm going to write a long string and try to word wrap this shit, like it's as easy as fucking a whore.\n\nI hope this works." );

	Text::TextLayout layout;
	layout.align_v = Text::Alignment_V::Middle;
	m_text = Text( longString, layout, Rectf( 0.f, 0.f, Sizef( 300.f, 600.f ) ), m_consola );
		
	{
		m_marqueePos = Vec2f( 800.f, static_cast< float >( Graphics::ScreenHeight - m_consola.GetCharSize().height ) );
		const std::string marqueeStr = "This is some scrolling text along the bottom to test out text clipping capabilities.";
		const auto rect = Text::EstimateMinRect( marqueeStr, m_consola );

		auto layout = Text::TextLayout();
		layout.endofline = Text::EndOfLine::None;
		
		m_marqueeText = Text( marqueeStr, layout, rect, m_consola );
	}

}

void Scene_FontRendering::Update( float DeltaTime )
{
	fps = 1.f / DeltaTime;

	const auto strPixelLen = m_marqueeText.GetActualBoundary().GetWidth();
	const auto speed = 200.f;
	m_marqueePos.x -= speed * DeltaTime;
	const auto strEnd = m_marqueePos.x + strPixelLen;

	if( strEnd <= 0.f )
	{
		m_marqueePos.x = 800.f;
	}
}

void Scene_FontRendering::Draw() const
{
	// Marquee ( scrolling text ) works with the Text class if you use EndOfLine::None
	m_marqueeText.Draw( m_marqueePos, Colors::White, m_graphics );

	// Recommended approach, make one instance and reuse.
	m_text.Draw( { 0.f, 0.f }, Colors::Green, m_graphics );

	// Not recommended for long strings, but might be ok for short strings.
	{
		std::stringstream ss;
		ss << "FPS: " << fps;

		const auto fpsStr = ss.str();
		const auto rect = Text::EstimateMinRect( fpsStr, m_consola );

		const Vec2f pos = { 0.f, 0.f };

		Text::TextLayout layout;
		layout.endofline = Text::EndOfLine::None;
		Text text( fpsStr, layout, rect, m_consola );
		text.Draw( { pos.x, pos.y + m_consola.GetCharSize().height }, Colors::Yellow, m_graphics );

	}
}

