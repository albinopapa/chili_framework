#pragma once

// Demo and framework headers
#include "choice.h"
#include "Colors.h"
#include "GlobalEnums.h"
#include "Physics.h"
#include "Rect.h"
#include "Sprite.h"

// Dim2d headers
#include <algorithm2d.h>
#include <grid.h>

// STL headers
#include <array>
#include <optional>
#include <random>

constexpr Vec2i operator+( const Vec2i& _left, const dim2d::offset& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}
constexpr Vec2i operator+( const dim2d::offset& _left, const Vec2i& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}
template<typename T>
constexpr _Vec2<T> operator*( const Size_t<T>& _left, const _Vec2<T>& _right )noexcept
{
	return { _left.width * _right.x, _left.height * _right.y };
}
template<typename T>
constexpr _Vec2<T> operator*( const _Vec2<T>& _left, const Size_t<T>& _right )noexcept
{
	return { _left.x * _right.width, _left.y * _right.height };
}

using Point = dim2d::offset;

constexpr Sizei tile_size		= { 32,32 };
constexpr Sizei room_size		= { 10, 10 };
constexpr Sizei maze_size		= { 25, 25 };
constexpr Sizei maze_pixel_size = { 
	maze_size.width * room_size.width * tile_size.width,
	maze_size.height * room_size.height * tile_size.height
};
constexpr Sizei room_pixel_size = {
	room_size.width * tile_size.width,
	room_size.height * tile_size.height
};

constexpr Color wallColor		= Colors::Gray;
constexpr Color floorColor		= Colors::DarkGray;

struct cell
{
	Point pos;
	Wall wall					= Wall::all;
	bool visited				= false;
};

struct Tile
{
	enum class Type { wall, floor };

	Tile() = default;
	Tile( Type _type, Sprite const& _sprite )noexcept
		:
		m_pSprite( std::addressof( _sprite ) ),
		type( _type )
	{}
	Sprite const& operator*()const { return *m_pSprite; }
	void SetLightLevel( std::uint8_t _level )noexcept { m_lightlevel = _level; }

	Sprite const* m_pSprite = nullptr;
	std::uint8_t m_lightlevel = 0ui8;
	Type type = Type::floor;
};

struct Room
{
	Room() = default;
	Room(
		Sprite const& _ground,
		Sprite const& _northwall,
		Sprite const& _eastwall,
		Sprite const& _southwall,
		Sprite const& _westwall,
		cell const& _cell );

	auto GetTiles()
		->dim2d::grid<Tile, room_size.width, room_size.height>&;
	auto GetTiles()const
		->const dim2d::grid<Tile, room_size.width, room_size.height>&;

	dim2d::grid<Tile, room_size.width, room_size.height> m_tiles;
};

struct MazeGenerator
{
	static constexpr auto n_offset = Vec2i{ 0, -1 };
	static constexpr auto e_offset = Vec2i{ 1, 0 };
	static constexpr auto s_offset = Vec2i{ 0, 1 };
	static constexpr auto w_offset = Vec2i{ -1, 0 };

	dim2d::grid<cell, maze_size.width, maze_size.height> operator()( Vec2i startPos, Vec2i endPos );
};

class Maze
{
public:
	Maze( Vec2i _startPos, Vec2i endPos );

	auto GetRooms()
		->dim2d::grid<Room, maze_size.width, maze_size.height>&;
	auto GetRooms()const
		->dim2d::grid<Room, maze_size.width, maze_size.height> const&;

	Vec2i GetRoomIndex( const Vec2f& _position )const noexcept;
	template<typename T> _Vec2<T> GetRoomPosition( Vec2i const& room_idx )const noexcept
	{
		return _Vec2<T>(
			T( room_idx.x ) * T( room_pixel_size.width ),
			T( room_idx.y ) * T( room_pixel_size.height )
			);
	}
	template<typename T> _Rect<T> GetRoomRect( const _Vec2<T>& _position )const noexcept
	{
		const auto room_idx = GetRoomIndex( _position );
		const auto room_size = Size_t<T>( T( room_pixel_size.width ), T( room_pixel_size.height ) );
		const auto room_position =
			_Vec2<T>{ T( room_idx.x ) * room_size.width, T( room_idx.y ) * room_size.height };
		return _Rect<T>( room_position, room_size );
	}

	Vec2i GetTileIndex( const Vec2i& room_index, const Vec2f& _position, bool isBack = false )const noexcept;
	Vec2i GetTilePosition( Vec2i const & room_idx, Vec2i const& tile_index )const noexcept;
	std::optional<Rectf> GetTileRect( const Rectf& rect, const Vec2f& velocity )const noexcept;

private:
	dim2d::grid<Room, maze_size.width, maze_size.height> m_rooms;
	Sprite m_ground, m_northwall, m_eastwall, m_southwall, m_westwall;
};