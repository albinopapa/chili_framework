#pragma once

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

