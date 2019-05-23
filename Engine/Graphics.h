#pragma once

#include "ChiliWin.h"
#include "Colors.h"
#include "DXException.h"
#include "Font.h"
#include "Rect.h"
#include "Text.h"
#include "Sprite.h"
#include <algorithm2d.h>
#include <surface.h>
#include <d3d11.h>
#include <wrl.h>

struct AlphaEffect;

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

	dim2d::surface<Color>& GetBuffer()noexcept;
	dim2d::surface<float>& GetDBuffer()noexcept;

	void PutPixelAlpha( int X, int Y, Color C );
	void DrawCircle( const Vec2i &Center, int Radius, Color C );
	void DrawCircleAlpha( const Recti &Rect, Color C );
	void DrawRectAlpha( const Recti &Rect, Color C );
	void DrawRect( const RectI &_rect, Color _color )noexcept;
	
	static bool IsInView( const Recti& _rect );
	void DrawChar( float X, float Y, char C, Font const& font, Color Clr );
	void DrawChar( const Vec2f &Pos, char C, Font const& font, Color Clr );
	void DrawString( float X, float Y, Font const& font, const std::string &Str, Color Clr );
	void DrawString( const Vec2f &Pos, Font const& font, const std::string &Str, Color Clr );
	void DrawText( const Vec2f &Position, Text const& text, Color C );

	template<typename Effect>
	void DrawSprite( const Rectf &Dst, const Sprite& sprite, float base, Effect effect )
	{
		DrawSprite( sprite.GetRect(), Dst, sprite, base, std::move( effect ) );
	}
	template<typename Effect>
	void DrawSprite( const Rectf &Src, const Rectf &Dst, const Sprite& sprite, float base, Effect effect )
	{
		const auto rectified = Rectify( Dst, GetRect<float>() );
		const auto src = RectI( rectified ) + Vec2i( Src.LeftTop() );
		const auto dst = RectI( rectified ) + Vec2i( Dst.LeftTop() );

		// check if source and destination have size
		const auto src_has_size = src.GetSize().Area() != 0;
		const auto dst_has_size = dst.GetSize().Area() != 0;

		// check if sprite is in view and source and destination rects have size > 0
		if( IsInView( dst ) && src_has_size && dst_has_size )
		{
			effect( src, dst, sprite, base );
		}
	}

	static void SetResolution( const Recti &WinRect );
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
	dim2d::surface<float>   dBuffer;
	static Recti			screenRect;
};


struct SpriteEffect
{
	template<typename T>
	dim2d::surface_wrapper<T> make_wrapper( Recti const& rect, int stride, T& sprite )
	{
		return dim2d::surface_wrapper<T>(
			dim2d::offset{ rect.left, rect.top },
			rect.GetWidth(),
			rect.GetHeight(),
			stride,
			sprite
		);
	}
};

struct AlphaEffect : SpriteEffect
{
	AlphaEffect( Graphics& _graphics )
		:
		m_graphics( _graphics )
	{}
	void operator()( const Recti &Src, const Recti &Dst, const Sprite& sprite, float base )
	{
		const auto src = make_wrapper( Src, Src.GetWidth(), sprite );
		auto dst = make_wrapper( Dst, Graphics::GetWidth<int>(), m_graphics.GetBuffer() );
		auto& dbuffer = m_graphics.GetDBuffer();
		auto const offset = dim2d::offset{ Dst.left, Dst.top };

		dim2d::transform( src.begin(), src.end(), dst.begin(), dst.begin(), 
			[ & ]( dim2d::index src1idx, Color src1, dim2d::index src2idx, Color src2 )
		{
			auto& d = dbuffer[ src2idx + offset ];
			if( d < base )
			{
				d = base;
				return src1.AlphaBlend( src2 );
			}
			else
			{
				return src2;
			}
		} );
	}
	Graphics& m_graphics;
};

struct CopyEffect : SpriteEffect
{
	CopyEffect( Graphics& _graphics )
		:
		m_graphics( _graphics )
	{}
	void operator()( const Recti &Src, const Recti &Dst, const Sprite& sprite, float base )
	{
		const auto src = make_wrapper( Src, sprite.GetWidth(), sprite );
		auto dst = make_wrapper( Dst, Graphics::GetWidth<int>(), m_graphics.GetBuffer() );
		auto& dbuffer = m_graphics.GetDBuffer();
		auto const offset = dim2d::offset{ Dst.left, Dst.top };
		dim2d::transform( src.begin(), src.end(), dst.begin(), dst.begin(),
			[ & ]( dim2d::index src1idx, Color src1, dim2d::index src2idx, Color src2 )
		{
			auto& d = dbuffer[ src2idx + offset ];
			if( d < base )
			{
				d = base;
				return src1;
			}
			else
			{
				return src2;
			}
		} );
	}
	Graphics& m_graphics;

};

struct MirrorEffect : SpriteEffect
{
	MirrorEffect( Graphics& _graphics )
		:
		m_graphics( _graphics )
	{}
	void operator()( const Recti &Src, const Recti &Dst, const Sprite& sprite, float base )
	{
		const auto src = make_wrapper( Src, Src.GetWidth(), sprite );
		auto dst = make_wrapper( Dst, Graphics::GetWidth<int>(), m_graphics.GetBuffer() );
		auto const offset = dim2d::offset{ Dst.left, Dst.top };

		auto& dbuffer = m_graphics.GetDBuffer();
		dim2d::transform( src.mbegin(), src.mend(), dst.begin(), dst.begin(),
			[ & ]( dim2d::index src1idx, Color src1, dim2d::index src2idx, Color src2 )
		{
			auto& d = dbuffer[ src2idx + offset ];
			if( d < base )
			{
				d = base;
				return src1;
			}
			else
			{
				return src2;
			}
		} );
	}
	Graphics& m_graphics;
};