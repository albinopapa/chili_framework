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

#include "Scene_Particles.h"
#include "Scene_Sprite.h"
#include "Scene_AnimatedSprite.h"
#include "Scene_Camera.h"
#include "Scene_FontRendering.h"
#include "Scene_Shooter.h"


Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd ),
	m_scene( InitScene<Scene_Shooter>() )
	//m_scene( InitScene<Scene_Particles>() )
	//m_scene( InitScene<Scene_Camera>() )
	//m_scene( InitScene<Scene_AnimatedSprite>() )
	//m_scene( InitScene<Scene_FontRendering>() )
	//m_scene( InitScene<Scene_Sprite>() )
{
}

 void Game::Go()
{
	gfx.BeginFrame( Colors::Black );
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	m_scene->Update( ft.Mark() );
}

void Game::ComposeFrame()
{
	m_scene->Draw();
}
