#include "RandomNumber.h"

std::mt19937 Random::rng;

Random::Random( unsigned int seed ) noexcept
{
	rng.seed( seed );
}