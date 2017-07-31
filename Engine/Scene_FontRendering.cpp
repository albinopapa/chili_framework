#include "Scene_FontRendering.h"


Scene_FontRendering::Scene_FontRendering( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx )
{
	TextFormat::Properties props;
	props.fontname = L"Consolas";
	props.size = 28.f;
	m_consola = Font{ props };
}

void Scene_FontRendering::Update( float DeltaTime )
{
}

void Scene_FontRendering::Draw() const
{
	m_consola.DrawString( 0.f, 300.f, "Your score: ", Colors::Red, m_graphics );
}

