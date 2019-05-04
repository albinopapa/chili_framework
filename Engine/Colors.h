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
	unsigned int dword = 255 << 24;
public:
	Color() = default;
	constexpr Color( unsigned int dw )noexcept
		:
		dword( dw )
	{}
	constexpr Color( unsigned int a, unsigned int r, unsigned int g, unsigned int b )noexcept
		:
		dword( (a << 24u) | (r << 16u) | (g << 8u) | b )
	{}
	constexpr Color( unsigned int r, unsigned int g, unsigned int b )noexcept
		:
		Color( 255u, r, g, b )
	{}
	constexpr Color( Color col, unsigned int x )noexcept
		:
		Color( ( x << 24u ) | ( col.dword & 0x00FFFFFF ) )
	{}

	constexpr unsigned char GetA() const noexcept
	{
		return dword >> 24u;
	}
	constexpr unsigned char GetR() const noexcept
	{
		return (dword >> 16u) & 0xFFu;
	}
	constexpr unsigned char GetG() const noexcept
	{
		return (dword >> 8u) & 0xFFu;
	}
	constexpr unsigned char GetB() const noexcept
	{
		return dword & 0xFFu;
	}

	void SetA( unsigned char x ) noexcept
	{
		dword = (dword & 0xFFFFFFu) | (x << 24u);
	}
	void SetR( unsigned char r )noexcept
	{
		dword = (dword & 0xFF00FFFFu) | (r << 16u);
	}
	void SetG( unsigned char g )noexcept
	{
		dword = (dword & 0xFFFF00FFu) | (g << 8u);
	}
	void SetB( unsigned char b )noexcept
	{
		dword = (dword & 0xFFFFFF00u) | b;
	}
	
	Color operator+( const Color &C )const noexcept
	{
		return Color( *this ) += C;
	}
	Color &operator+=( const Color &C )noexcept
	{
		SetR( std::min<unsigned char>( GetR() + C.GetR(), 255u ) );
		SetG( std::min<unsigned char>( GetG() + C.GetG(), 255u ) );
		SetB( std::min<unsigned char>( GetB() + C.GetB(), 255u ) );

		return *this;
	}
	Color operator*( unsigned char S )const noexcept
	{
		return Color( *this ) *= S;
	}
	Color &operator*=( unsigned char S ) noexcept
	{
		SetR( unsigned char( ( GetR() * S ) >> 8u ) );
		SetG( unsigned char( ( GetG() * S ) >> 8u ) );
		SetB( unsigned char( ( GetB() * S ) >> 8u ) );
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
		const auto rval = int{ GetR() } * 33 / 100;
		const auto gval = int{ GetG() + 1 } * 34 / 100;
		const auto bval = int{ GetB() } * 33 / 100;

		return rval + gval + bval;
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
	constexpr Color White		= Color( 255u,255u,255u );
	constexpr Color Black		= Color( 0u,0u,0u );
	constexpr Color Gray		= Color( 127u, 127u, 127u );
	constexpr Color LightGray	= Color( 192u, 192u, 192u );
	constexpr Color DarkGray	= Color( 96u, 96u, 96u );
	constexpr Color Red			= Color( 255u,0u,0u );
	constexpr Color Green		= Color( 0u,255u,0u );
	constexpr Color Blue		= Color( 0u,0u,255u );
	constexpr Color Yellow		= Color( 255u,255u,0u );
	constexpr Color Cyan		= Color( 0u,255u,255u );
	constexpr Color Magenta		= Color( 255u,0u,255u );
	constexpr Color Orange		= Color( 255, 128, 64 );
}