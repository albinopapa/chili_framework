#pragma once

#include "../../Includes/surface.h"
#include "ChiliWin.h"
#include "Colors.h"
#include "DXException.h"
#include "Font.h"
#include "Rect.h"
#include "Text.h"
#include "Sprite.h"
#include <d3d11.h>
#include <wrl.h>

class Graphics
{
private:
	class Direct3D
	{
	public:
		Direct3D( HWND WinHandle, Graphics &Parent );
		~Direct3D();

		void Present( Graphics& gfx );

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

	static bool IsInView( const Recti& _rect );
	void DrawChar( float X, float Y, char C, Font const& font, Color Clr );
	void DrawChar( const Vec2f &Pos, char C, Font const& font, Color Clr );
	void DrawString( float X, float Y, Font const& font, const std::string &Str, Color Clr );
	void DrawString( const Vec2f &Pos, Font const& font, const std::string &Str, Color Clr );
	void DrawText( const Vec2f &Position, Text const& text, Color C );
	void DrawSprite( const Rectf &Dst, const Sprite& sprite );
	void DrawSprite( const Rectf &Src, const Rectf &Dst, const Sprite& sprite );


	static void SetResolution( const RECT &WinRect );
	template<typename T>
	static T GetWidth()noexcept
	{
		return  static_cast< T >( screenRect.GetWidth() );
	}
	template<typename T> 
	static T GetHeight()noexcept
	{
		return static_cast< T >( screenRect.GetHeight() );
	}
	template<typename T>
	static Size_t<T> GetSize()noexcept
	{
		return { GetWidth<T>(), GetHeight<T>() };
	}
	template<typename T>
	static _Rect<T> GetRect()noexcept
	{
		if constexpr( std::is_same_v<T, int> )
		{
			return screenRect;
		}
		else
		{
			return static_cast< _Rect<T> >( screenRect );
		}
	}
private:
	Direct3D				m_direct3d;
	friend class Direct3D;
	dim2d::surface<Color>	pSysBuffer;
	static Recti			screenRect;
};
