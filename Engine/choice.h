#pragma once

#include <type_traits>	// for std::is_same_v
#include <utility>		// for std::forward and std::move
#include <variant>		// for std::variant, std::visit

struct TrueType { static constexpr bool value = true; };
struct FalseType { static constexpr bool value = false; };
using BoolType = std::variant<TrueType, FalseType>;


// struct choice:	structure to store functors to be called.
// CheckFunc check:	callable/invokable non-member object used to determine if doFunc is to be called
// Func dFunc:		callable/invokable non-member object called if choice::check() return true
template<typename CheckFunc, typename Func>
struct choice
{
	constexpr choice( CheckFunc _check, Func _func )
		:
		check( _check ),
		doFunc( std::move( _func ) )
	{}

	CheckFunc check;
	Func doFunc;
};

// template argument deduction guide, helps compiler figure out the template parameters for class types
// without the need for a make_* function
template<typename CheckFunc, typename Func> choice( CheckFunc, Func )->choice<CheckFunc, Func >;

// void choose() calls _cond.check() and if true, runs _cond.doFunc() recursively until 
// no more choices are available or the _cond.doFunc() function has been called.
template<template<typename, typename> typename Cond, typename CheckFunc, typename Func, typename...Rest>
void choose( const Cond<CheckFunc, Func> _cond, Rest&&... _rest )
{
	auto check_condition = [ & ]( auto _condition )
	{
		if constexpr( std::is_same_v<decltype( _condition ), TrueType> )
		{
			_cond.doFunc();
		}
		else if constexpr( sizeof...( Rest ) > 0 )
		{
			choose( std::forward<Rest>( _rest )... );
		}
	};

	std::visit( check_condition, _cond.check() ? BoolType{ TrueType{} } : BoolType{ FalseType{} } );
}

