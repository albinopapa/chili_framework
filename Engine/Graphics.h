/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
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
#pragma once

#include "aligned_ptr.h"
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "DXException.h"
#include "Colors.h"
#include "Rect.h"

class Graphics
{
private:
	class Direct3D
	{
	public:
		Direct3D( HWND WinHandle, Graphics &Parent );
		~Direct3D();

		void Present( const aligned_ptr<Color>& pSysBuffer );

	private:
		Graphics &m_parent;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
		Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
		D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	};

private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};

public:
	Graphics( class HWNDKey& key );

	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;

	void EndFrame();
	void BeginFrame( Color C = Colors::Black );

	void PutPixel( int x, int y, int r, int g, int b );
	void PutPixel( int x,int y,Color c );
	void PutPixelAlpha( int X, int Y, Color C );
	void DrawCircle( const Vec2i &Center, int Radius, Color C );
	void DrawCircleAlpha( const Recti &Rect, Color C );
	void DrawRectAlpha( const Recti &Rect, Color C );

	Recti Rectify( const Recti &Rect )const;
	Recti Rectify( int Left, int Width, int Top, int Height )const;

	static void SetResolution( const RECT &WinRect );
private:
	Direct3D			m_direct3d;
	aligned_ptr<Color>	pSysBuffer;

public:
	static int   ScreenWidth;
	static int   ScreenHeight;
	static float fScreenWidth;
	static float fScreenHeight;
	static Sizei ScreenSize;
	static Sizef fScreenSize;
	static Recti ScreenRect;
	static Rectf fScreenRect;
};
