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
	constexpr operator Size_t<T2>()const
	{
		return Size_t<T2>(
			static_cast< T2 >( width ),
			static_cast< T2 >( height )
			);
	}

	constexpr operator _Vec2<T>()const
	{
		return{ width, height };
	}

	constexpr T Area()const
	{
		return width * height;
	}

	Size_t &operator*=( const T &S )
	{
		width *= S;
		height *= S;
		return *this;
	}
	Size_t operator*( const T &S )const
	{
		return Size_t( *this ) *= S;
	}

	Size_t &operator/=( const T &S )
	{
		width  /= S;
		height /= S;
		return *this;
	}
	Size_t operator/( const T &S )const
	{
		return Size_t( *this ) /= S;
	}

public:
	T width, height;
};

using Sizef = Size_t<float>;
using Sizei = Size_t<int>;

template < typename T >
class _Rect
{
public:
	_Rect() = default;
	constexpr _Rect( T Left, T Top, T Right, T Bottom )
		:
		left( Left ),
		top( Top ),
		right( Right ),
		bottom( Bottom )
	{
	}
	constexpr _Rect( const _Vec2<T> &LeftTop, const Size_t<T> &Size )
		:
		_Rect( LeftTop.x, LeftTop.y, LeftTop.x + Size.width, LeftTop.y + Size.height )
	{
	}
	constexpr _Rect( T Left, T Top, const Size_t<T> &Size )
		:
		_Rect( _Vec2<T>{ Left, Top }, Size )
	{
	}
	constexpr _Rect( const _Vec2<T> &p0, const _Vec2<T> &p1 )
		:
		_Rect( p0.x, p0.y, p1.x, p1.y )
	{
	}

	template <typename T2>
	constexpr operator _Rect<T2>() const
	{
		return{
			_Vec2<T2>{( T2 )left, ( T2 )top},
			_Vec2<T2>{( T2 )right, ( T2 )bottom }
		};
	}

	constexpr _Rect Translate( const _Vec2<T> &d )const
	{
		return _Rect( *this ).Translate( d );
	}
	_Rect &Translate( const _Vec2<T> &d )
	{
		*this = { LeftTop() + d, RightBottom() + d };
		return *this;
	}

	constexpr _Rect ClipTo( const _Rect& rect )const
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

	constexpr T Left()const noexcept
	{
		return left;
	}
	constexpr T Top()const noexcept
	{
		return top;
	}
	constexpr T Right()const noexcept
	{
		return right;
	}
	constexpr T Bottom()const noexcept
	{
		return bottom;
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
		return _Vec2<T>( left + ( GetWidth() / 2 ), top + ( GetHeight() / 2 ) );
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
			( ( p.y >= top ) && ( p.y < bottom ) ) &&
			( ( p.x >= left ) && ( p.x < right ) );
	}
	template <typename T2>
	constexpr bool Contains( const _Rect<T2> &p ) const
	{
		return
			( ( p.top >= top ) && ( p.bottom <= bottom ) ) &&
			( ( p.left >= left ) && ( p.right <= right ) );
	}

	constexpr _Rect FlipHorizontal()const
	{
		return _Rect( *this ).FlipHorizontal();
	}
	_Rect &FlipHorizontal()
	{
		std::swap( left, right );		
		return *this;
	}
	constexpr _Rect FlipVertical()const
	{
		return _Rect( *this ).FlipVertical();
	}
	_Rect &FlipVertical()
	{
		std::swap( top, bottom );
		return *this;
	}
public:
	T left = T( 0 ), top = T( 0 ), right = T( 0 ), bottom = T( 0 );
};

typedef _Rect< int > RectI;
typedef _Rect< float > RectF;
using Rectf = _Rect<float>;
using Recti = _Rect<int>;

template<class T>
_Rect<T> MakeRectFromCenter( const _Vec2<T> &Center, const Size_t<T> &Size )
{
	const _Rect<T> result( Center, Size );
	return result.Translate( -static_cast<_Vec2<T>>( Size / static_cast< T >( 2 ) ) );
}

inline Rectf RectF_FromInt( int Left, int Top, int Right, int Bottom )
{
	return
		static_cast< Rectf >( Recti( Left, Top, Right, Bottom ) );
}

template<class T>
_Rect<T> Rectify( const _Rect<T>& _source, const _Rect<T>& _bounds )
{
	return{
		std::max( -_source.left, _bounds.left ),
		std::max( -_source.top, _bounds.top ),
		std::min( _bounds.right - _source.left, _source.GetWidth() ),
		std::min( _bounds.bottom - _source.top, _source.GetHeight() )
	};
}