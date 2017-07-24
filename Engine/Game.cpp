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
	m_pBitmap( ImageLoader::Load( "Images/MrGodinD3DError.png", m_wic ) )
{

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
}

void Game::ComposeFrame()
{
	int width, height;
		
	m_pBitmap->GetSize( reinterpret_cast< UINT* >( &width ), reinterpret_cast< UINT* >( &height ) );
	UINT bufferSize = static_cast< UINT >( width * height ) * sizeof( Color );
	Microsoft::WRL::ComPtr<IWICBitmapLock> pLock;
	WICRect wr{ 0, 0, width, height };
	
	m_pBitmap->Lock( &wr, WICBitmapLockRead, &pLock );
	Color* pPixels = nullptr;
	UINT stride = 0u;
	pLock->GetStride( &stride );
	const auto testStride = width * sizeof( Color );
	pLock->GetDataPointer( &bufferSize, reinterpret_cast< WICInProcPointer* >( &pPixels ) );

	const auto x = ( Graphics::ScreenWidth - width ) >> 1;
	const auto y = ( Graphics::ScreenHeight - height ) >> 1;

	for( auto iy = 0; iy < height; ++iy )
	{
		for( auto ix = 0; ix < width; ++ix )
		{
			const Color color = pPixels[ ix + ( iy * width ) ];
			gfx.PutPixel( x + ix, y + iy, color );
		}
	}
}
