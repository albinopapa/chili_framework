/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"

#include "ImageLoader.h"

constexpr float fScreenWidth = static_cast< float >( Graphics::ScreenWidth );
constexpr float fScreenHeight = static_cast< float >( Graphics::ScreenHeight );

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	m_cache( m_wic ),
	m_player( { 400.f, ( m_cache.m_background.GetHeight() * .667f ) },wnd.kbd, m_cache ),
	m_camera( m_player.GetPosition() ),
	m_levelrect( static_cast< Rectf >( m_cache.m_background.GetRect() ) ),
	m_screenrect( 0.f, 0.f, fScreenWidth, fScreenHeight )
{

}

void Game::Go()
{
	gfx.BeginFrame( Colors::Gray );
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	const auto dt = .016f;

	m_player.Update( dt );
	m_camera.Update( dt );
	m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
}

void Game::ComposeFrame()
{
	const auto viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );
	{
		m_cache.m_background.Draw( viewport, m_screenrect, gfx );
	}

	m_player.Draw( viewport, gfx );
}
