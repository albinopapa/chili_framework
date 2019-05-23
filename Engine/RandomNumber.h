#pragma once

#include <random>

class Random
{
public:
	Random( unsigned int seed = std::mt19937::default_seed )noexcept;

	template<typename T>
	T operator()( T _min_value, T _max_value )noexcept
	{
		if constexpr( std::is_floating_point_v<T> )
		{
			std::uniform_real_distribution<T> dist( _min_value, _max_value );
			return dist( rng );
		}
		else if constexpr( std::is_integral_v<T> )
		{
			std::uniform_int_distribution<T> dist( _min_value, _max_value );
			return dist( rng );
		}
	}

private:
	static std::mt19937 rng;
};

