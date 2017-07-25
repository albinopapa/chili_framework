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

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	m_walk( Frames::SpriteType::Alpha, 30, "Images/RangerWalk", ".png", m_wic ),
	m_stand( Frames::SpriteType::Alpha, 1, "Images/RangerStand", ".png", m_wic ),
	m_pAnimController( std::make_unique<AnimationController>( 30.f / 900.f, m_stand ) ),
	m_animController( 30.f / 900.f, m_stand )
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
	while( !wnd.kbd.KeyIsEmpty() )
	{
		const auto e = wnd.kbd.ReadKey();
		if( e.GetCode() == VK_RIGHT )
		{
			isFacingLeft = false;
			if( e.IsPress() )
			{
				m_animController = AnimationController( 30.f / 900.f, m_walk );
			}
			else if( e.IsRelease() )
			{
				m_animController = AnimationController( 30.f / 900.f, m_stand );
			}
		}
		else if( e.GetCode() == VK_LEFT )
		{
			isFacingLeft = true;

			if( e.IsPress() )
			{
				m_animController = AnimationController( 30.f / 900.f, m_walk );
			}
			else if( e.IsRelease() )
			{
				m_animController = AnimationController( 30.f / 900.f, m_stand );
			}
		}
	}

	m_animController.Advance( .016f );
}

void Game::ComposeFrame()
{
	const auto x = ( Graphics::ScreenWidth -  m_pAnimController->GetWidth() ) >> 1;
	const auto y = ( Graphics::ScreenHeight - m_pAnimController->GetHeight() ) >> 1;

	if( isFacingLeft )
	{
		m_animController.DrawReverse( x, y, gfx );
	}
	else
	{
		m_animController.Draw( x, y, gfx );
	}
}
