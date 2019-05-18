/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
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

#include "ChiliMath.h"

template <typename T>
class _Vec2
{
public:
	_Vec2() = default;
	constexpr _Vec2( T x,T y )noexcept
		:
		x( x ),
		y( y )
	{}
	template <typename T2>
	constexpr explicit operator _Vec2<T2>() const noexcept
	{
		return{ (T2)x,(T2)y };
	}

	// Unary operator
	constexpr _Vec2	operator-() const noexcept
	{
		return _Vec2( -x,-y );
	}

	// Assignment operators
	constexpr _Vec2&	operator+=( const _Vec2 &rhs )noexcept
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	constexpr _Vec2&	operator-=( const _Vec2 &rhs )noexcept
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	constexpr _Vec2&	operator*=( const T &rhs )noexcept
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	constexpr _Vec2&	operator/=( const T &rhs )noexcept
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}

	constexpr T		LenSq() const noexcept
	{
		return ( x * x ) + ( y * y );
	}
	constexpr T		Len() const noexcept
	{
		return sqrt( LenSq() );
	}

	_Vec2&	Normalize()noexcept
	{
		const auto len = Len();

		if( len == 0 )
			return *this;

		if constexpr(std::is_floating_point_v<T>)
		{
			auto const invLen = static_cast< T >( 1 ) / len;
			x *= invLen;
			y *= invLen;
		}
		else if constexpr( std::is_integral_v<T> )
		{
			x /= len;
			y /= len;
		}
		
		return *this;
	}
	constexpr _Vec2	Normalize() const noexcept
	{
		const auto len = Len();
		if( len == static_cast< T >( 0 ) )
			return *this;

		_Vec2 result;
		if constexpr( std::is_floating_point_v<T> )
		{
			auto const invLen = static_cast< T >( 1 ) / len;

			result.x *= invLen;
			result.y *= invLen;
		}
		else if constexpr( std::is_integral_v<T> )
		{
			result.x /= len;
			result.y /= len;
		}

		return result;
	}

	constexpr _Vec2	InterpolateTo( const _Vec2& dest,T alpha ) const noexcept
	{
		return ( *this + ( ( dest - *this )*alpha ) );
	}
	constexpr _Vec2 &InterpolateTo( const _Vec2 &dest, T alpha )noexcept
	{
		*this += ( ( dest - *this ) * alpha );
		return *this;
	}

public:
	T x = T( 0 );
	T y = T( 0 );
};

// Binary operators
template <typename T>
constexpr bool operator==( const _Vec2<T>& lhs, const _Vec2<T> &rhs )noexcept
{
	return ( lhs.x == rhs.x ) && ( lhs.y == rhs.y );
}

template <typename T>
constexpr bool operator!=( const _Vec2<T>& lhs, const _Vec2<T> &rhs )noexcept
{
	return !( *this == rhs );
}



template<class T>
_Vec2<T> Reflect( const _Vec2<T> &Direction, const _Vec2<T> &Normal ) noexcept
{
	return Direction - ( Normal * ( 2.f * ( Direction * Normal ) ) );
}

using Vec2f = _Vec2<float>;
using Vec2d = _Vec2<double>;
using Vec2i = _Vec2<int>;
