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

using Point = dim2d::offset;
constexpr int tile_width = 32;
constexpr int tile_height = 32;
constexpr int room_width = 10;
constexpr int room_height = 10;
constexpr int maze_width = 25;
constexpr int maze_height = 25;
constexpr int maze_pixel_width = maze_width * room_width * tile_width;
constexpr int maze_pixel_height = maze_height * room_height * tile_height;
constexpr int room_pixel_width = room_width * tile_width;
constexpr int room_pixel_height = room_height * tile_height;



constexpr Color wallColor = Colors::Gray;
constexpr Color floorColor = Colors::DarkGray;

struct cell
{
	Point pos;
	Wall wall = Wall::all;
	bool visited = false;
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
		->dim2d::grid<Tile, room_width, room_height>&;
	auto GetTiles()const
		->const dim2d::grid<Tile, room_width, room_height>&;

	dim2d::grid<Tile, room_width, room_height> m_tiles;
};

struct MazeGenerator
{
	static constexpr auto n_offset = dim2d::offset{ 0, -1 };
	static constexpr auto e_offset = dim2d::offset{ 1, 0 };
	static constexpr auto s_offset = dim2d::offset{ 0, 1 };
	static constexpr auto w_offset = dim2d::offset{ -1, 0 };

	dim2d::grid<cell, maze_width, maze_height> operator()( dim2d::offset startPos );
};

class Maze
{
public:
	Maze( dim2d::offset _startPos );

	auto GetRooms()
		->dim2d::grid<Room, maze_width, maze_height>&;
	auto GetRooms()const
		->dim2d::grid<Room, maze_width, maze_height> const&;

	dim2d::offset GetRoomIndex( const Vec2f& _position )const noexcept;
	Vec2i GetRoomPosition( dim2d::offset const& room_idx )const noexcept;
	template<typename T> _Rect<T> GetRoomRect( const _Vec2<T>& _position )const noexcept
	{
		const auto room_idx = GetRoomIndex( _position );
		const auto room_size = Size_t<T>( T( room_pixel_width ), T( room_pixel_height ) );
		const auto room_position =
			_Vec2<T>{ T( room_idx.x ) * room_size.width, T( room_idx.y ) * room_size.height };
		return _Rect<T>( room_position, room_size );
	}

	dim2d::offset GetTileIndex( const dim2d::offset& room_index, const Vec2f& _position, bool isBack = false )const noexcept;
	Vec2i GetTilePosition( dim2d::offset const & room_idx, dim2d::offset const& tile_index )const noexcept;
	std::optional<Rectf> GetTileRect( const Rectf& rect, const Vec2f& velocity )const noexcept;

private:
	dim2d::grid<Room, maze_width, maze_height> m_rooms;
	Sprite m_ground, m_northwall, m_eastwall, m_southwall, m_westwall;
};