#pragma once

#include "ChiliMath.h"
#include "Rect.h"
#include "Vec2.h"

template <typename T>
constexpr auto sq( const T& x )
{
	return x * x;
}

template<typename T>
constexpr T wrap_angle( T theta )
{
	const T modded = fmod( theta, ( T )2.0 * ( T )PI_D );
	return ( modded > ( T )PI_D ) ?
		( modded - ( T )2.0 * ( T )PI_D ) : modded;
}

constexpr float ToRadians( float A )
{
	return deg2rad * A;
}

constexpr float ToDegrees( float A )
{
	return rad2deg * A;
}


template <typename T>
constexpr auto operator+( const _Vec2<T>& lhs, const _Vec2<T> &rhs )noexcept
{
	return _Vec2<T>{ lhs.x + rhs.x, lhs.y + rhs.y };
}
template <typename T>
constexpr auto operator-( const _Vec2<T>& lhs, const _Vec2<T> &rhs )noexcept
{
	return _Vec2<T>{ lhs.x - rhs.x, lhs.y - rhs.y };
}
template <typename T>
constexpr auto operator*( const _Vec2<T>& lhs, const T &rhs )noexcept
{
	return _Vec2<T>{ lhs.x * rhs, lhs.y * rhs };
}
template <typename T>
constexpr auto operator*( const _Vec2<T>& lhs, const _Vec2<T> &rhs )noexcept
{
	return ( lhs.x * rhs.x ) + ( lhs.y * rhs.y );
}
template <typename T>
constexpr auto operator/( const _Vec2<T>& lhs, const T &rhs )noexcept
{
	return _Vec2<T>{ lhs.x /= rhs, lhs.y /= rhs };
}

template<typename T>
constexpr auto operator+( _Vec2<T> const& _lhs, Size_t<T> const& _rhs )noexcept
{
	return _Vec2<T>{_lhs.x + _rhs.width, _lhs.y + _rhs.height};
}

template<typename T>
constexpr auto operator+( Size_t<T> const& _lhs, _Vec2<T> const& _rhs )noexcept
{
	return _Vec2<T>{_lhs.width + _rhs.x, _lhs.height + _rhs.y};
}

template<typename T>
constexpr auto operator-( _Vec2<T> const& _lhs, Size_t<T> const& _rhs )noexcept
{
	return _Vec2<T>{_lhs.x - _rhs.width, _lhs.y - _rhs.height};
}

template <typename T>
constexpr Size_t<T> operator*( const Size_t<T>& lhs, const T &S )noexcept
{
	return { lhs.width * S, lhs.height * S };
}

template <typename T>
constexpr Size_t<T> operator*( Size_t<T> const& _lhs, const Size_t<T>& _rhs )noexcept
{
	return { _lhs.width * _rhs.width, _lhs.height * _rhs.height };
}

template <typename T>
constexpr Size_t<T> operator/( Size_t<T> const& _lhs, const T &S )noexcept
{
	return { _lhs.width / S, lhs.height / S };
}

template<class T>
constexpr _Rect<T> operator/( const _Rect<T>& _rect, const Size_t<T>& _size )noexcept
{
	return{
		_rect.left / _size.width,
		_rect.top / _size.height,
		_rect.right / _size.width,
		_rect.bottom / _size.height
	};
}

template<class T>
constexpr _Rect<T> operator+( const _Rect<T>& _lhs, const _Vec2<T>& _rhs )noexcept
{
	return {
		_lhs.left + _rhs.x,
		_lhs.top + _rhs.y,
		_lhs.right + _rhs.x,
		_lhs.bottom + _rhs.y
	};
}

template<class T>
constexpr _Rect<T> operator-( const _Rect<T>& _lhs, const _Vec2<T>& _rhs )noexcept
{
	return {
		_lhs.left - _rhs.x,
		_lhs.top - _rhs.y,
		_lhs.right - _rhs.x,
		_lhs.bottom - _rhs.y
	};
}

template<class T>
constexpr _Rect<T>& operator+=( _Rect<T>& _lhs, const _Vec2<T>& _rhs )noexcept
{
	_lhs.left += _rhs.x;
	_lhs.top += _rhs.y;
	_lhs.right += _rhs.x;
	_lhs.bottom += _rhs.y;

	return _lhs;
}

template<class T>
constexpr _Rect<T>& operator-=( _Rect<T>& _lhs, const _Vec2<T>& _rhs )noexcept
{
	_lhs.left - _rhs.x;
	_lhs.top - _rhs.y;
	_lhs.right - _rhs.x;
	_lhs.bottom - _rhs.y;
	
	return _lhs;
}

