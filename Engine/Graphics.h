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
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"

template<class T>
class aligned_ptr
{
public:
	aligned_ptr() = default;
	aligned_ptr( size_t CountElements, size_t Alignment )
		:
		m_count( CountElements ),
		m_alignment( Alignment ),
		m_pPtr( reinterpret_cast<T*>( _aligned_malloc( CountElements * sizeof( T ), Alignment ) ) )
	{
	}
	aligned_ptr( aligned_ptr &&Src )
	{
		*this = std::move( Src );
	}
	~aligned_ptr()
	{
		if( m_pPtr )
		{
			_aligned_free( m_pPtr );
			m_pPtr = nullptr;
		}
	}

	aligned_ptr &operator=( aligned_ptr &&Src )
	{
		m_count = Src.m_count;
		m_alignment = Src.m_alignment;
		m_pPtr = Src.m_pPtr;
		
		Src.m_alignment = 0u;
		Src.m_count = 0u;
		Src.m_pPtr = nullptr;

		return *this;
	}

	aligned_ptr( const aligned_ptr & ) = delete;
	aligned_ptr &operator=( const aligned_ptr & ) = delete;
	
	size_t Alignment()const
	{
		return m_alignment;
	}
	size_t Size()const
	{
		return m_count * sizeof( T );
	}
	size_t Count()const
	{
		return m_count;
	}

	operator bool()const
	{
		return m_pPtr != nullptr;
	}
	bool operator==( const aligned_ptr &Src )const
	{
		return m_pPtr == Src.m_pPtr;
	}
	bool operator!=( const aligned_ptr &Src )const
	{
		return !( *this == Src );
	}

	T& operator[]( size_t idx )
	{
		return *( m_pPtr + idx );
	}
	const T& operator[]( size_t idx )const
	{
		return *( m_pPtr + idx );
	}
	
	T* operator->()
	{
		return m_pPtr;
	}

	T& operator*( size_t idx )
	{
		return *m_pPtr;
	}
	const T& operator*( size_t idx )const
	{
		return *m_pPtr;
	}

	T* Get()
	{
		return m_pPtr;
	}
	const T* Get()const
	{
		return m_pPtr;
	}

	void Fill( int Val )
	{
		memset( m_pPtr, Val, Size() );
	}
	void Clear()
	{
		memset( m_pPtr, 0, Size() );
	}

private:
	T *m_pPtr;
	size_t m_count = 0u, m_alignment = 4u;
};

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};

private:
	class Direct3D
	{
	public:
		Direct3D( HWND WinHandle, Graphics &Parent );
		~Direct3D();

		void Present( const aligned_ptr<Color>& pSysBuffer );

	private:


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
	void BeginFrame();

	void PutPixel( int x, int y, int r, int g, int b );
	void PutPixel( int x,int y,Color c );
	
private:
	Direct3D			m_direct3d;
	aligned_ptr<Color>	pSysBuffer;

public:
	static constexpr int ScreenWidth = 800;
	static constexpr int ScreenHeight = 600;
};
