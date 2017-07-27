#pragma once

#include "Vec2.h"
#include <algorithm>
#undef min
#undef max

template<class T>class Size_t
{
public: 
	constexpr Size_t()
		:
		width( 0 ), height( 0 )
	{
	}
	constexpr Size_t( T Width, T Height )
		:
		width( Width ), height( Height )
	{
	}
	template<class T2>
	operator Size_t<T2>()
	{
		return Size_t<T2>(
			static_cast< T2 >( width ),
			static_cast< T2 >( height )
			);
	}

	T width, height;
};

using Sizef = Size_t<float>;
using Sizei = Size_t<int>;

template < typename T >
class _Rect
{
public:
	constexpr _Rect() :left( 0 ), top( 0 ), right( 0 ), bottom( 0 ) {}
	constexpr _Rect( T Left,T Top,T Right,T Bottom)
		:
	top( Top ),
	bottom( Bottom ),
	left( Left ),
	right( Right )
	{}
	constexpr _Rect( const _Vec2<T> &LeftTop, const Size_t<T> &Size )
		:
		_Rect( LeftTop.x, LeftTop.y, LeftTop.x + Size.width, LeftTop.y + Size.height )
	{}
	constexpr _Rect( float Left, float Top, const Size_t<T> &Size )
		:
		_Rect( _Vec2<T>{ Left, Top }, _Vec2<T>{ Left + Size.width, Top + Size.height } )
	{}
	constexpr _Rect( const _Vec2<T> &p0, const _Vec2<T> &p1 )
		:
		_Rect( std::min( p0.x, p1.x ), std::min( p0.y, p1.y ),
			   std::max( p0.x, p1.x ), std::max( p0.y, p1.y ) )
	{}

	_Rect Translate( const _Vec2<T> &d )const
	{
		return _Rect( *this ).Translate( d );
	}
	_Rect &Translate( const _Vec2<T> &d )
	{
		*this = { LeftTop() + d, RightBottom() + d };
		return *this;
	}

	template <typename T2>
	operator _Rect<T2>() const
	{
		return{ ( T2 )left, ( T2 )top, ( T2 )right, ( T2 )bottom };
	}

	_Rect ClipTo( const _Rect& rect )const
	{
		return _Rect( *this ).ClipTo( rect );
	}

	_Rect& ClipTo( const _Rect& rect )
	{
		top = std::max( top, rect.top );
		bottom = std::min( bottom, rect.bottom );
		left = std::max( left, rect.left );
		right = std::min( right, rect.right );
		return *this;
	}

	constexpr _Vec2<T> LeftTop()const
	{
		return{ left, top };
	}
	constexpr _Vec2<T> RightTop()const
	{
		return{ right, top };
	}
	constexpr _Vec2<T> LeftBottom()const
	{
		return{ left, bottom };
	}
	constexpr _Vec2<T> RightBottom()const
	{
		return{ right, bottom };
	}

	constexpr T GetWidth() const
	{
		return right - left;
	}
	constexpr T GetHeight() const
	{
		return bottom - top;
	}
	constexpr _Vec2<T> GetCenter()const
	{
		return _Vec2<T>( left + ( GetWidth() * .5f ), top + ( GetHeight() * .5f ) );
	}
	constexpr Size_t<T> GetSize()const
	{
		return{ GetWidth(), GetHeight() };
	}

	constexpr bool Overlaps( const _Rect& rect ) const
	{
		return top < rect.bottom && bottom > rect.top && 
			left < rect.right && right > rect.left;
	}
	template <typename T2>
	constexpr bool Contains( const _Vec2<T2> &p ) const
	{
		return 
			( ( p.y >= top ) && ( p.y <= bottom ) ) &&
			( ( p.x >= left ) && ( p.x <= right ) );
	}
	template <typename T2>
	constexpr bool Contains( const _Rect<T2> &p ) const
	{
		return 
			( ( p.top >= top ) && ( p.bottom <= bottom ) ) &&
			( ( p.left >= left ) && ( p.right <= right ) );
	}
public:
	T left, top, right, bottom;
};

typedef _Rect< int > RectI;
typedef _Rect< float > RectF;
using Rectf = _Rect<float>;
using Recti = _Rect<int>;

template<class T>
_Rect<T> MakeRectFromCenter( const _Vec2<T> &Center, const Size_t<T> &Size )
{
	_Vec2<T> half = _Vec2<T>{ Size.width, Size.height } / 2.;
	return _Rect<T>( Center - half, Center + half );
}

inline Rectf RectF_FromInt(int Left, int Top, int Right, int Bottom)
{
	return 
		static_cast< Rectf >( Recti( Left, Top, Right, Bottom ) );
}