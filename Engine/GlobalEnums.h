#pragma once

#include "Rect.h"

#include <grid.h>

enum class Wall 
{ 
	none				= 0, 
	
	north				= 1, 
	east				= 2, 
	south				= 4, 
	west				= 8, 

	north_east			= north | east,
	north_south			= north | south,
	north_west			= north | west,
	east_west			= east  | west,
	south_east			= south | east,
	south_west			= south | west,

	north_east_south	= north | east | south,
	north_east_west		= north | east | west,
	north_south_west	= north | south | west,
	east_south_west		= east | south | west,

	all = north | east | south | west 
};
constexpr Wall operator|( Wall left, Wall right )noexcept { return Wall( int( left ) | int( right ) ); }
constexpr Wall operator&( Wall left, Wall right )noexcept { return Wall( int( left ) & int( right ) ); }
constexpr Wall operator~( Wall left )noexcept { return Wall( ~int( left ) ); }
constexpr Wall& operator|=( Wall& left, Wall right )noexcept
{
	left = left | right;
	return left;
}
constexpr Wall& operator&=( Wall& left, Wall right )noexcept
{
	left = left & right;
	return left;
}

enum class Direction {
	Up,
	RightUp,
	Right,
	RightDown,
	Down,
	LeftDown,
	Left,
	LeftUp
};

constexpr Sizei tile_size = { 32, 32 };
constexpr Sizei room_size = { 25, 25 };
constexpr Sizei maze_size = { 25, 25 };
constexpr Sizei map_size = { room_size.width * maze_size.width, room_size.height * maze_size.height };

constexpr auto n_offset = Vec2i{ 0, -1 };
constexpr auto e_offset = Vec2i{ 1, 0 };
constexpr auto s_offset = Vec2i{ 0, 1 };
constexpr auto w_offset = Vec2i{ -1, 0 };

struct cell
{
	dim2d::offset pos;
	Wall wall = Wall::all;
	bool visited = false;
};

using CellGrid = dim2d::grid<cell, maze_size.width, maze_size.height>;