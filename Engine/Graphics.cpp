/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.cpp																		  *
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
#include "Graphics.h"
#include "DXErr.h"
#include "ChiliException.h"
#include <assert.h>
#include <string>
#include <array>
#include "SSE.h"

// Ignore the intellisense error "cannot open source file" for .shh files.
// They will be created during the build sequence before the preprocessor runs.
namespace FramebufferShaders
{
#include "FramebufferPS.shh"
#include "FramebufferVS.shh"
}

#pragma comment( lib,"d3d11.lib" )

#define CHILI_GFX_EXCEPTION( hr,note ) DXException( hr,note,_CRT_WIDE(__FILE__),__LINE__ )

using Microsoft::WRL::ComPtr;

RectI Graphics::screenRect = { 100,100,800,600 };

Graphics::Graphics( HWNDKey& key )
	:
	m_direct3d( key.hWnd, *this ),
	pSysBuffer( screenRect.GetWidth(), screenRect.GetHeight() )
{
}

void Graphics::EndFrame()
{
	m_direct3d.Present( *this );
}

void Graphics::BeginFrame( Color C )
{
	// clear the sysbuffer
	//pSysBuffer.Fill( C );
	//dim2d::fill( pSysBuffer.begin(), pSysBuffer.end(), Colors::Black );
	auto* pPixels = std::addressof( pSysBuffer[ dim2d::offset{ 0,0 } ] );
	memset( pPixels, 0, sizeof( decltype( *pPixels ) )*pSysBuffer.columns()*pSysBuffer.rows() );
}

void Graphics::PutPixel( int x, int y, int r, int g, int b )
{
	pSysBuffer[ {x, y} ] = Color( r, g, b );
}

void Graphics::PutPixel( int x, int y, Color c )
{
	pSysBuffer[ {x, y} ] = c;
}

dim2d::surface<Color>& Graphics::GetBuffer() noexcept
{
	return pSysBuffer;
}

void Graphics::PutPixelAlpha( int x, int y, Color c )
{
	pSysBuffer[ {x, y} ] = c.AlphaBlend( pSysBuffer[ {x, y} ] );
}

void Graphics::DrawCircle( const Vec2i & Center, int Radius, Color C )
{
	const auto sqRadius = sq( Radius );

	const Vec2i vRadius = { Radius, Radius };

	auto const circleRect = Recti( Center - vRadius, Center + vRadius );
	const auto bounds = Rectify( circleRect, GetRect<int>() ).Translate( Center );

	auto src = dim2d::surface_wrapper(
		dim2d::offset{ bounds.left,bounds.top },
		bounds.GetWidth(),
		bounds.GetHeight(),
		pSysBuffer.columns(),
		pSysBuffer
	);

	auto isInRadius = [ & ]( dim2d::index idx, const Color& color )->bool
	{
		return ( sq( idx.x - vRadius.x ) + sq( idx.y - vRadius.y ) ) < sqRadius;
	};

	dim2d::replace_if( src.begin(), src.end(), src.begin(), C, isInRadius );
	/*dim2d::for_each( src.begin(), src.end(), [ & ]( dim2d::index idx, Color& color )
	{
		const auto sqDist = sq( idx.x - vRadius.x ) + sq( idx.y - vRadius.y );

		if( sqDist < sqRadius )
		{
			color = C;
		}
	} );*/
}

void Graphics::DrawCircleAlpha( const Recti &Rect, Color C )
{
	const auto radius = Rect.GetWidth() / 2;
	const auto sqRadius = sq( radius );
	const auto sqInner = sq( std::max( radius >> 1, 1 ) );

	const auto vRadius = Vec2i{ radius, radius };
	const auto center = Rect.GetCenter();
	const auto circleRect = Recti( center - vRadius, center + vRadius );
	const auto bounds = Rectify( circleRect, GetRect<int>() ).Translate( center );

	const Color centerColor = Colors::White * ( C.Brightness() );
	auto beg = dim2d::index_iterator( bounds.left, bounds.top, bounds.right, bounds.bottom, pSysBuffer );
	auto end = beg + dim2d::offset{ bounds.GetWidth(),bounds.GetHeight() };

	auto generator = [ & ]( dim2d::index _idx, Color& _color )
	{
		const auto sqDist = sq( _idx.x ) + sq( _idx.y );

		if( sqDist < sqRadius )
		{
			const auto dist = sqrtf( static_cast< float >( sqDist ) );
			const auto step = dist / static_cast< float >( radius );

			const auto alpha =
				static_cast< unsigned char >( ( 1.f - step ) * 255.f );

			_color = ( sqDist < sqInner ) ?
				Color( centerColor, alpha ) : Color( C, alpha );
		}
	};

	dim2d::for_each( beg, end, generator );
}

void Graphics::DrawRectAlpha( const Recti & Rect, Color C )
{
	const auto bounds = Rectify( Rect, GetRect<int>() ).Translate( Rect.LeftTop() );

	auto src = dim2d::surface_wrapper<dim2d::surface<Color>>(
		dim2d::offset{ bounds.left,bounds.top },
		bounds.GetWidth(),
		bounds.GetHeight(),
		pSysBuffer.columns(),
		pSysBuffer
		);
	
	dim2d::transform( src.begin(), src.end(), src.begin(), [ & ]( dim2d::index idx, Color _color )
	{
		return C.AlphaBlend( _color );
	} );
}

void Graphics::DrawRect( const RectI & _rect, Color _color ) noexcept
{
	auto rectified = _rect.ClipTo( GetRect<int>() );
	rectified.bottom -= rectified.bottom == GetHeight<int>() ? 1 : 0;
	rectified.right -= rectified.right == GetWidth<int>() ? 1 : 0;

	if( IsInView( rectified ) )
	{
		if( rectified.GetHeight() > 0 )
		{
			for( int x = rectified.left; x < rectified.right; ++x )
			{
				PutPixel( x, rectified.top, _color );
				PutPixel( x, rectified.bottom, _color );
			}	
		}
		if( rectified.GetWidth() > 0 )
		{
			for( int y = rectified.top; y < rectified.bottom; ++y )
			{
				PutPixel( rectified.left, y, _color );
				PutPixel( rectified.right, y, _color );
			}
		}
	}
}

bool Graphics::IsInView( const Recti & _rect )
{
	return
		_rect.left < screenRect.GetWidth() && _rect.right >= 0 &&
		_rect.top < screenRect.GetHeight() && _rect.bottom >= 0;
}

void Graphics::DrawChar( float X, float Y, char C, Font const& font, Color color )
{
	const auto position = Vec2i( static_cast< int >( X ), static_cast< int >( Y ) );

	const auto sourceRect = static_cast< Recti >( font.GetRect( C ) );
	const auto destinationRect = Rectify(
		RectI( position, Sizei( sourceRect.GetSize() ) ),
		GetRect<int>()
	).Translate( position );

	if( IsInView( destinationRect ) )
	{
		const auto vBegOffset = sourceRect.LeftTop() + ( destinationRect.LeftTop() - position );

		const auto src =
			dim2d::surface_wrapper(
				dim2d::offset{ vBegOffset.x,vBegOffset.y },
				destinationRect.GetWidth(),
				destinationRect.GetHeight(),
				font.GetSurface().columns(),
				font.GetSurface()
			);
		auto dst =
			dim2d::surface_wrapper<dim2d::surface<Color>>(
				dim2d::offset{ destinationRect.left, destinationRect.top },
				destinationRect.GetWidth(),
				destinationRect.GetHeight(),
				screenRect.GetWidth(),
				pSysBuffer
				);

		auto is_black =
			[]( dim2d::index _idx, const Color& _font_color )
		{
			return _font_color == Colors::Black;
		};

		dim2d::replace_if( src.begin(), src.end(), dst.begin(), color, is_black );
	}
}

void Graphics::DrawChar( const Vec2f & Pos, char C, Font const& font, Color Clr )
{
	DrawChar( Pos.x, Pos.y, C, font, Clr );
}

void Graphics::DrawString( float X, float Y, Font const& font, const std::string & Str, Color Clr )
{
	if( Str.empty() ) return;

	const auto ix = static_cast< int >( X );
	const auto iy = static_cast< int >( Y );

	const auto chSize = font.GetCharSize();

	const auto maxCharsPerRow = font.MaxCharsPerRow( Graphics::GetWidth<int>() );
	const auto pixelWidth = static_cast< int >( ( Str.size() - 1 ) * chSize.width );

	const auto stringSize = Sizei( pixelWidth, chSize.height );

	auto stringRect = Rectify( Recti( ix, iy, stringSize ), Graphics::GetRect<int>() );

	const int startIndex = std::max( stringRect.left / chSize.width, 0 );
	const auto charCount = ( stringRect.right + chSize.width ) / chSize.width;
	const int endIndex = std::min( charCount, maxCharsPerRow );

	if( Graphics::IsInView( stringRect ) )
	{
		for( int i = 0, j = startIndex; j < endIndex; ++i, ++j )
		{
			const auto xPixelOffset = ( startIndex + i ) * chSize.width;
			//const auto xCharOffset = i * chSize.width;

			const float x = static_cast< float >( ix + xPixelOffset /*+ xCharOffset*/ );
			const float y = static_cast< float >( iy + stringRect.top );

			DrawChar( x, y, Str[ j ], font, Clr );
		}
	}
}

void Graphics::DrawString( const Vec2f & Pos, Font const& font, const std::string & Str, Color Clr )
{
	DrawString( Pos.x, Pos.y, font, Str, Clr );
}

void Graphics::DrawText( const Vec2f &Position, Text const& text, Color C )
{
	for( size_t i = 0; i < text.GetStrings().size(); ++i )
	{
		const auto position = static_cast< Vec2f >( text.GetStringPositions()[ i ] ) + Position;
		DrawString( position.x, position.y, text.GetFont(), text.GetStrings()[ i ], C );
	}
}

void Graphics::SetResolution( const RECT & WinRect )
{
	screenRect.left = WinRect.left;
	screenRect.top = WinRect.top;
	screenRect.right = WinRect.right;
	screenRect.bottom = WinRect.bottom;
}


Graphics::Direct3D::Direct3D( HWND WinHandle, Graphics & Parent )
	:
	m_parent( Parent )
{
	assert( WinHandle != nullptr );

	//////////////////////////////////////////////////////
	// create device and swap chain/get render target view
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferCount = 1;
	sd.BufferDesc.Width = screenRect.GetWidth();
	sd.BufferDesc.Height = screenRect.GetHeight();
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = WinHandle;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT				hr;
	UINT				createFlags = 0u;
#ifdef CHILI_USE_D3D_DEBUG_LAYER
#ifdef _DEBUG
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
#endif

	// create device and front/back buffers
	if( FAILED( hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwapChain,
		&pDevice,
		nullptr,
		&pImmediateContext ) ) )
	{
		if( FAILED( hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_WARP,
			nullptr,
			createFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&sd,
			&pSwapChain,
			&pDevice,
			nullptr,
			&pImmediateContext ) ) )
		{
			throw CHILI_GFX_EXCEPTION( hr, L"Creating device and swap chain" );
		}
	}

	// get handle to backbuffer
	ComPtr<ID3D11Resource> pBackBuffer;
	if( FAILED( hr = pSwapChain->GetBuffer(
		0,
		__uuidof( ID3D11Texture2D ),
		( LPVOID* )&pBackBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Getting back buffer" );
	}

	// create a view on backbuffer that we can render to
	if( FAILED( hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pRenderTargetView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating render target view on backbuffer" );
	}


	// set backbuffer as the render target using created view
	pImmediateContext->OMSetRenderTargets( 1, pRenderTargetView.GetAddressOf(), nullptr );


	// set viewport dimensions
	D3D11_VIEWPORT vp;
	vp.Width = float( screenRect.GetWidth() );
	vp.Height = float( screenRect.GetHeight() );
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pImmediateContext->RSSetViewports( 1, &vp );


	///////////////////////////////////////
	// create texture for cpu render target
	D3D11_TEXTURE2D_DESC sysTexDesc;
	sysTexDesc.Width = screenRect.GetWidth();
	sysTexDesc.Height = screenRect.GetHeight();
	sysTexDesc.MipLevels = 1;
	sysTexDesc.ArraySize = 1;
	sysTexDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sysTexDesc.SampleDesc.Count = 1;
	sysTexDesc.SampleDesc.Quality = 0;
	sysTexDesc.Usage = D3D11_USAGE_DYNAMIC;
	sysTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	sysTexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	sysTexDesc.MiscFlags = 0;
	// create the texture
	if( FAILED( hr = pDevice->CreateTexture2D( &sysTexDesc, nullptr, &pSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sysbuffer texture" );
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = sysTexDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	// create the resource view on the texture
	if( FAILED( hr = pDevice->CreateShaderResourceView( pSysBufferTexture.Get(),
		&srvDesc, &pSysBufferTextureView ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating view on sysBuffer texture" );
	}


	////////////////////////////////////////////////
	// create pixel shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreatePixelShader(
		FramebufferShaders::FramebufferPSBytecode,
		sizeof( FramebufferShaders::FramebufferPSBytecode ),
		nullptr,
		&pPixelShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating pixel shader" );
	}


	/////////////////////////////////////////////////
	// create vertex shader for framebuffer
	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateVertexShader(
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		nullptr,
		&pVertexShader ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex shader" );
	}


	//////////////////////////////////////////////////////////////
	// create and fill vertex buffer with quad for rendering frame
	const FSQVertex vertices[] =
	{
		{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 0.5f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, 1.0f, 0.5f, 0.0f, 0.0f },
		{ 1.0f, -1.0f, 0.5f, 1.0f, 1.0f },
		{ -1.0f, -1.0f, 0.5f, 0.0f, 1.0f },
	};

	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( FSQVertex ) * 6;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = vertices;
	if( FAILED( hr = pDevice->CreateBuffer( &bd, &initData, &pVertexBuffer ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating vertex buffer" );
	}


	//////////////////////////////////////////
	// create input layout for fullscreen quad
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// Ignore the intellisense error "namespace has no member"
	if( FAILED( hr = pDevice->CreateInputLayout( ied, 2,
		FramebufferShaders::FramebufferVSBytecode,
		sizeof( FramebufferShaders::FramebufferVSBytecode ),
		&pInputLayout ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating input layout" );
	}


	////////////////////////////////////////////////////
	// Create sampler state for fullscreen textured quad
	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	if( FAILED( hr = pDevice->CreateSamplerState( &sampDesc, &pSamplerState ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Creating sampler state" );
	}

}

Graphics::Direct3D::~Direct3D()
{
	// clear the state of the device context before destruction
	if( pImmediateContext ) pImmediateContext->ClearState();
}

void Graphics::Direct3D::Present( Graphics& gfx )
{
	HRESULT hr;

	// lock and map the adapter memory for copying over the sysbuffer
	if( FAILED( hr = pImmediateContext->Map( pSysBufferTexture.Get(), 0u,
		D3D11_MAP_WRITE_DISCARD, 0u, &mappedSysBufferTexture ) ) )
	{
		throw CHILI_GFX_EXCEPTION( hr, L"Mapping sysbuffer" );
	}

	// setup parameters for copy operation

	{
		auto dest = dim2d::raw_pointer_wrapper(
			dim2d::offset{ 0, 0 },
			gfx.pSysBuffer.columns(),
			screenRect.GetHeight(),
			mappedSysBufferTexture.RowPitch / sizeof( Color ),
			reinterpret_cast< Color* >( mappedSysBufferTexture.pData ) );

		dim2d::copy( gfx.pSysBuffer.begin(), gfx.pSysBuffer.end(), dest.begin() );
	}

	// release the adapter memory
	pImmediateContext->Unmap( pSysBufferTexture.Get(), 0u );

	// render offscreen scene texture to back buffer
	pImmediateContext->IASetInputLayout( pInputLayout.Get() );
	pImmediateContext->VSSetShader( pVertexShader.Get(), nullptr, 0u );
	pImmediateContext->PSSetShader( pPixelShader.Get(), nullptr, 0u );
	pImmediateContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	const UINT stride = sizeof( FSQVertex );
	const UINT offset = 0u;
	pImmediateContext->IASetVertexBuffers( 0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset );
	pImmediateContext->PSSetShaderResources( 0u, 1u, pSysBufferTextureView.GetAddressOf() );
	pImmediateContext->PSSetSamplers( 0u, 1u, pSamplerState.GetAddressOf() );
	pImmediateContext->Draw( 6u, 0u );

	// flip back/front buffers
	if( FAILED( hr = pSwapChain->Present( 1u, 0u ) ) )
	{
		if( hr == DXGI_ERROR_DEVICE_REMOVED )
		{
			throw CHILI_GFX_EXCEPTION( pDevice->GetDeviceRemovedReason(), L"Presenting back buffer [device removed]" );
		}
		else
		{
			throw CHILI_GFX_EXCEPTION( hr, L"Presenting back buffer" );
		}
	}
}
