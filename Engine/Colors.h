/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Colors.h																			  *
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
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <algorithm>

class Color
{
public:
	unsigned int dword;
public:
	constexpr Color() : dword() {}
	constexpr Color( const Color& col )
		:
		dword( col.dword )
	{}
	constexpr Color( unsigned int dw )
		:
		dword( dw )
	{}
	constexpr Color( unsigned char x,unsigned char r,unsigned char g,unsigned char b )
		:
		dword( (x << 24u) | (r << 16u) | (g << 8u) | b )
	{}
	constexpr Color( unsigned char r, unsigned char g, unsigned char b )
		:
		Color( 255u, r, g, b )
	{}
	constexpr Color( Color col,unsigned char x )
		:
		Color( ( x << 24u ) | ( col.dword & 0x00FFFFFF ) )
	{}
	Color& operator =( Color color )
	{
		dword = color.dword;
		return *this;
	}
	constexpr unsigned char GetX() const
	{
		return dword >> 24u;
	}
	constexpr unsigned char GetA() const
	{
		return GetX();
	}
	constexpr unsigned char GetR() const
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const
	{
		return (dword >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const
	{
		return dword & 0xFFu;
	}
	void SetX( unsigned char x )
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}
	void SetA( unsigned char a )
	{
		SetX( a );
	}
	void SetR( unsigned char r )
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG( unsigned char g )
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB( unsigned char b )
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
	
	Color operator+( const Color &C )const
	{
		return Color( *this ) += C;
	}
	Color &operator+=( const Color &C )
	{
		SetR( std::min<unsigned char>( GetR() + C.GetR(), 255u ) );
		SetG( std::min<unsigned char>( GetG() + C.GetG(), 255u ) );
		SetB( std::min<unsigned char>( GetB() + C.GetB(), 255u ) );

		return *this;
	}
	Color operator*( unsigned char S )const
	{
		return Color( *this ) *= S;
	}
	Color &operator*=( unsigned char S )
	{
		SetR( static_cast<unsigned char>( ( GetR() * S ) >> 8u ) );
		SetG( static_cast<unsigned char>( ( GetG() * S ) >> 8u ) );
		SetB( static_cast<unsigned char>( ( GetB() * S ) >> 8u ) );
		return *this;
	}
	Color AlphaBlend( Color Src )
	{
		const auto srcAlpha0 = GetA();
		const auto srcAlpha1 = 255u - srcAlpha0;

		return ( *this * srcAlpha0 ) + ( Src * srcAlpha1 );		
	}
	unsigned char Brightness()const
	{
		const auto m0 = std::max( GetR(), GetG() );
		const auto m1 = std::max( m0, GetB() );
		return static_cast< unsigned char >( m1 );
	}
	constexpr bool operator==( const Color C )const
	{
		return dword == C.dword;
	}
	constexpr bool operator!=( const Color C )const
	{
		return !( *this == C );
	}
};

namespace Colors
{
	constexpr Color MakeRGB( unsigned char r,unsigned char g,unsigned char b )
	{
		//return (r << 16) | (g << 8) | b;
		return Color( 255u, r, g, b );
	}
	constexpr Color White = MakeRGB( 255u,255u,255u );
	constexpr Color Black = MakeRGB( 0u,0u,0u );
	constexpr Color Gray = MakeRGB( 0x80u,0x80u,0x80u );
	constexpr Color LightGray = MakeRGB( 0xD3u,0xD3u,0xD3u );
	constexpr Color Red = MakeRGB( 255u,0u,0u );
	constexpr Color Green = MakeRGB( 0u,255u,0u );
	constexpr Color Blue = MakeRGB( 0u,0u,255u );
	constexpr Color Yellow = MakeRGB( 255u,255u,0u );
	constexpr Color Cyan = MakeRGB( 0u,255u,255u );
	constexpr Color Magenta = MakeRGB( 255u,0u,255u );
	constexpr Color Orange = Color( 255, 128, 64 );
}