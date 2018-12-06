#pragma once


template<typename T, typename Block>
void nested_for_each( T _xmin, T _ymin, T _xmax, T _ymax, Block&& _block )
{
	for( T y = _ymin; y < _ymax; y += T( 1 ) )
	{
		for( T x = _xmin; x < _xmax; x += T( 1 ) )
		{
			_block( x, y );
		}
	}
}

template<template<typename> typename RectType, typename T, typename Block>
void nested_for_each( const RectType<T>& _boundary, Block&& _block )
{
	for( T y = _boundary.top; y < _boundary.bottom; y += T( 1 ) )
	{
		for( T x = _boundary.left; x < _boundary.right; x += T( 1 ) )
		{
			_block( x, y );
		}
	}
}