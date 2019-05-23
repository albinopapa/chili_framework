#pragma once

#include "GlobalEnums.h"
#include "ItemBase.h"
#include "Maze.h"
#include "Sprite.h"
#include "SpriteResource.h"
#include "Vec2.h"

#include <algorithm2d.h>
#include <grid.h>

#include <vector>

class Tilemap
{
public:
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

		Sprite const* m_pSprite = nullptr;
		Type type = Type::floor;
	};

	using TileGrid = dim2d::grid<Tile, map_size.width, map_size.height>;
	using RoomItemList = std::vector<std::unique_ptr<ItemBase>>;

	class Room
	{
	public:
		Room() = default;
		Room( cell const& _cell, Tilemap& _tilemap )
			:
			m_tilemap( std::addressof( _tilemap ) ),
			m_offset( { _cell.pos.x * room_size.width, _cell.pos.y * room_size.height } )
		{
			constexpr auto partialWallSize = Sizei{
				( room_size.width - 2 ) / 2,
				( room_size.height - 2 ) / 2
			};

			int wall_count = 0;
			auto generate_cell = [ & ]( dim2d::index idx )
			{
				auto hasWall = []( Wall subject, Wall control )
				{
					return ( subject & control ) == control;
				};

				int count = 0;
				if( hasWall( _cell.wall, Wall::north ) )
					++count;
				if( hasWall( _cell.wall, Wall::east ) )
					++count;
				if( hasWall( _cell.wall, Wall::south ) )
					++count;
				if( hasWall( _cell.wall, Wall::west ) )
					++count;
				isDeadEnd = count == 3;

				if( idx.x > 0 && idx.x < room_size.width - 1 &&
					idx.y > 0 && idx.y < room_size.height - 1 )
				{
					return Tile( Tile::Type::floor, sprites.m_ground );
				}

				if( idx.y == 0 )
				{
					if( hasWall( _cell.wall, Wall::north ) )
					{
						if( idx.x == room_size.width - 1 )
							return Tile( Tile::Type::wall, sprites.m_corner );
						else
							return Tile( Tile::Type::wall, sprites.m_northwall );
					}

					if( idx.x >= 0 && idx.x < partialWallSize.width ||
						idx.x >= partialWallSize.width + 2 && idx.x < room_size.width )
					{
						if( idx.x == room_size.width - 1 )
							return Tile( Tile::Type::wall, sprites.m_corner );
						else
							return Tile( Tile::Type::wall, sprites.m_northwall );
					}
				}
				else if( idx.y > 0 && idx.y < room_size.height - 1 )
				{
					if( idx.x == 0 )
					{
						if( hasWall( _cell.wall, Wall::west ) )
						{
							return Tile( Tile::Type::wall, sprites.m_westwall );
						}

						if( idx.y > 0 && idx.y < partialWallSize.height ||
							idx.y >= partialWallSize.height + 2 && idx.y < room_size.height )
						{
							return Tile( Tile::Type::wall, sprites.m_westwall );
						}
					}
					else if( idx.x == room_size.height - 1 )
					{
						if( hasWall( _cell.wall, Wall::east ) )
						{
							return Tile( Tile::Type::wall, sprites.m_eastwall );
						}

						if( idx.y > 0 && idx.y < partialWallSize.height ||
							idx.y >= partialWallSize.height + 2 && idx.y < room_size.height )
						{
							return Tile( Tile::Type::wall, sprites.m_eastwall );
						}
					}
				}
				else if( idx.y == room_size.height - 1 )
				{
					if( hasWall( _cell.wall, Wall::south ) )
					{
						if( idx.x == 0 )
							return Tile( Tile::Type::wall, sprites.m_corner );
						else
							return Tile( Tile::Type::wall, sprites.m_southwall );
					}

					if( idx.x >= 0 && idx.x < partialWallSize.width ||
						idx.x >= partialWallSize.width + 2 && idx.x < room_size.width )
					{
						if( idx.x == 0 )
							return Tile( Tile::Type::wall, sprites.m_corner );
						else
							return Tile( Tile::Type::wall, sprites.m_southwall );
					}
				}

				return Tile( Tile::Type::floor, sprites.m_ground );
			};

			auto wrapper = dim2d::surface_wrapper<TileGrid>(
				m_offset, room_size.width, room_size.height, map_size.width,
				m_tilemap->m_tiles
				);

			dim2d::generate( wrapper.begin(), wrapper.end(), wrapper.begin(), generate_cell );
		}

		void AddItem( std::unique_ptr<ItemBase> _item )
		{
			m_items.push_back( std::move( _item ) );
		}

		RoomItemList & GetItems() noexcept
		{
			return m_items;
		}

		RoomItemList const & GetItems() const noexcept
		{
			return m_items;
		}

		bool IsDeadEnd()const noexcept { return isDeadEnd; }
	private:
		Tilemap* m_tilemap = nullptr;
		RoomItemList m_items;
		dim2d::offset m_offset = { 0, -1 };
		bool isDeadEnd = false;
	};

	using RoomGrid = dim2d::grid<Room, maze_size.width, maze_size.height>;

public:
	template<typename T>
	static constexpr Vec2i world_to_tile( _Vec2<T> const& _position )
	{
		return { 
			int( _position.x ) / tile_size.width, 
			int( _position.y ) / tile_size.height };
	}
	template<typename T>
	static constexpr Vec2f tile_to_world( _Vec2<T> const& _position )
	{
		return { 
			float( _position.x ) * float( tile_size.width ),
			float( _position.y ) * float( tile_size.height ) };
	}
	template<typename T>
	static constexpr Vec2i world_to_room( _Vec2<T> const& _position )noexcept
	{
		return {
			int( _position.x ) / ( room_size.width * tile_size.width ),
			int( _position.y ) / ( room_size.height * tile_size.height )
		};
	}
	template<typename T>
	static constexpr Vec2f room_to_world( _Vec2<T> const& _position )noexcept
	{
		return {
			float( _position.x ) * float( room_size.width ) * float( tile_size.width ),
			float( _position.y ) * float( room_size.height ) * float( tile_size.height )
		};
	}

	Tilemap() = default;
	Tilemap( Vec2i startPos, Vec2i endPos, Random random )
	{
		// Generate maze
		auto const cells = GenerateMaze( startPos, endPos, random );

		// Use maze to generate tilemap
		dim2d::transform( cells.begin(), cells.end(), m_rooms.begin(),
			[ & ]( dim2d::index, cell const& _cell )
		{
			return Tilemap::Room( _cell, *this );
		} );


	}
	Room& GetRoom( Vec2i const& _room_idx )noexcept
	{
		return GetRoom( dim2d::index{ _room_idx.x, _room_idx.y } );
	}
	Room const& GetRoom( Vec2i const& _room_idx )const noexcept
	{
		return GetRoom( dim2d::index{ _room_idx.x, _room_idx.y } );
	}
	Tile& GetTile( Vec2i const& _tile_idx )noexcept
	{
		return GetTile( dim2d::index{ _tile_idx.x, _tile_idx.y } );
	}
	Tile const& GetTile( Vec2i const& _tile_idx )const noexcept
	{
		return GetTile( dim2d::index{ _tile_idx.x, _tile_idx.y } );
	}

	Room& GetRoom( dim2d::index const& _room_idx )noexcept
	{
		return m_rooms[ _room_idx ];
	}
	Room const& GetRoom( dim2d::index const& _room_idx )const noexcept
	{
		return m_rooms[ _room_idx ];
	}
	Tile& GetTile( dim2d::index const& _tile_idx )noexcept
	{
		return m_tiles[ _tile_idx ];
	}
	Tile const& GetTile( dim2d::index const& _tile_idx )const noexcept
	{
		return m_tiles[ _tile_idx ];
	}

	TileGrid& GetTiles()noexcept
	{
		return m_tiles;
	}
	TileGrid const& GetTiles()const noexcept
	{
		return m_tiles;
	}
	RoomGrid& GetRooms()noexcept
	{
		return m_rooms;
	}
	RoomGrid const& GetRooms()const noexcept
	{
		return m_rooms;
	}
private:
	RoomGrid m_rooms;
	TileGrid m_tiles;
	
};