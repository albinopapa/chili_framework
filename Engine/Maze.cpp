#include "Maze.h"
#include "MathOps.h"
#include "SpriteResource.h"

CellGrid MazeGenerator::operator()( Vec2i _startPos, Vec2i _endPos )
{
	using cell_iterator = typename CellGrid::iterator;

	auto get_available_neighbors = [](cell_iterator cur, CellGrid& cells)
	{
		auto neighbors = std::array{
				( cur.Index() + n_offset ),
				( cur.Index() + e_offset ),
				( cur.Index() + s_offset ),
				( cur.Index() + w_offset )
		};

		auto available = std::vector<cell_iterator>();
		for( auto neighbor : neighbors )
		{
			constexpr auto mazeRect = RectI( 0, 0, maze_size );
			if( mazeRect.Contains( neighbor ) )
			{
				if( auto it = cells.begin() + dim2d::offset{ neighbor.x,neighbor.y }; 
					!it->visited )
				{
					available.push_back( it );
				}
			}
		}

		return available;
	};

	auto choose_next_cell = []( std::vector<cell_iterator> available, std::mt19937& rng )
	{
		const auto index =
			std::uniform_int_distribution<std::size_t>{ 0, available.size() - 1 }( rng );

		auto next = available[ index ];
		next->visited = true;
		return next;
	};

	auto clear_walls = []( cell_iterator cur, cell_iterator next )
	{
		dim2d::offset offset = next->pos - cur->pos;
		if(offset.x == -1)
		{
			next->wall &= ~Wall::east; 
			 cur->wall &= ~Wall::west;
		}
		else if( offset.x == 1 )
		{
			next->wall &= ~Wall::west; 
			 cur->wall &= ~Wall::east;
		}
		if( offset.y == -1 )
		{
			next->wall &= ~Wall::south;
			 cur->wall &= ~Wall::north;
		}
		else if( offset.y == 1 )
		{
			next->wall &= ~Wall::north;
			 cur->wall &= ~Wall::south;
		}
	};

	std::mt19937 rng;

	CellGrid cells;
	dim2d::generate( cells.begin(), cells.end(), cells.begin(),
		[ & ]( dim2d::index idx ) { return cell{ idx }; } );

	const auto startOffset = dim2d::offset{ _startPos.x,_startPos.y };
	std::vector<cell_iterator> visited;
	visited.reserve( maze_size.width * maze_size.height );
	visited.emplace_back( cells.begin() + startOffset )->visited = true;

	bool endFound = false;
	while( !visited.empty() )
	{
		auto cur = visited.back();

		bool cur_is_end = cur->pos.x == _endPos.x && cur->pos.y == _endPos.y;
		const auto available = get_available_neighbors( cur, cells );
		if( !available.empty() && !cur_is_end )
		{
			auto next = choose_next_cell( std::move( available ), rng );
			clear_walls( cur, next );
			visited.push_back( next );
		}
		else
		{
			visited.pop_back();
		}
	}

	return cells;
}

//Maze::Maze( Vec2i _startPos, Vec2i _endPos )
//{
//	
//
//	auto cells = MazeGenerator{}( _startPos, _endPos );
//
//	dim2d::transform( cells.begin(), cells.end(), m_rooms.begin(),
//		[ & ]( dim2d::index, cell const& _cell )
//	{
//		return Room( _cell );
//	} );
//
//}
//
//auto Maze::GetRooms()->dim2d::grid<Room, maze_size.width, maze_size.height>&
//{
//	return m_rooms;
//}
//
//auto Maze::GetRooms()const->dim2d::grid<Room, maze_size.width, maze_size.height> const&
//{
//	return m_rooms;
//}
//
//Vec2i Maze::GetRoomIndex( const Vec2f & _position ) const noexcept
//{
//	return Vec2i{
//		int( std::floorf( _position.x / room_pixel_size.width ) ),
//		int( std::floorf( _position.y / room_pixel_size.height ) )
//	};
//}
//
//Vec2i Maze::GetTileIndex( const Vec2i& room_index, const Vec2f& _position, bool isBack ) const noexcept
//{
//	const auto room_position = GetRoomPosition<int>( room_index );
//	auto tile_index = Vec2i{
//		( int( std::floorf( _position.x ) ) - room_position.x ) / tile_size.width,
//		( int( std::floorf( _position.y ) ) - room_position.y ) / tile_size.height
//	};
//	tile_index.x = std::clamp( tile_index.x, 0, room_size.width );
//	tile_index.y = std::clamp( tile_index.y, 0, room_size.height );
//
//	if( isBack )
//	{
//		const auto temp = GetTilePosition( room_index, tile_index );
//		if( float( temp.x ) == _position.x )
//			--tile_index.x;
//		if( float( temp.y ) == _position.y )
//			--tile_index.y;
//	}
//	return tile_index;
//}
//
//Vec2i Maze::GetTilePosition( Vec2i const & room_idx, Vec2i const& tile_index ) const noexcept
//{
//	auto const room_position = GetRoomPosition<int>( room_idx );
//	return ( tile_index * tile_size ) + room_position;
//}
//
//
//std::optional<Rectf> Maze::GetTileRect( const Rectf& rect, const Vec2f& velocity )const noexcept
//{
//	const auto room_idx			= GetRoomIndex( rect.GetCenter() );
//	const auto room_position	= GetRoomPosition<float>( room_idx );
//
//	auto check_for_wall =
//		[&]( dim2d::grid<Tile, room_size.width, room_size.height> const& tiles, dim2d::index const& tile_idx )
//	{
//		auto const rect = Recti(
//			GetTilePosition( room_idx, Vec2i{ tile_idx.x,tile_idx.y } ),
//			tile_size );
//
//		auto result = ( tiles[ tile_idx ].type == Tile::Type::wall ) ?
//			std::optional<Rectf>( rect ) : std::optional<Rectf>();
//
//		return result;
//	};
//
//
//	auto const left_top_idx		= GetTileIndex( room_idx, rect.LeftTop() );
//	auto const right_bottom_idx = GetTileIndex( room_idx, rect.RightBottom(), true );
//
//	auto const& tiles			= m_rooms[ dim2d::index{ room_idx.x,room_idx.y } ].GetTiles();
//
//	for( int y = left_top_idx.y; y <= right_bottom_idx.y; ++y )
//	{
//		if( velocity.x > 0.f )
//		{
//			for( int x = left_top_idx.x; x <= right_bottom_idx.x; ++x )
//			{
//				if( auto result	= check_for_wall( tiles, dim2d::index{ x,y } ); result.has_value() )
//				{
//					return result;
//				}
//			}
//		}
//		else
//		{
//			for( int x = right_bottom_idx.x; x >= left_top_idx.x; --x )
//			{
//				if( auto result = check_for_wall( tiles, dim2d::index{ x,y } ); result.has_value() )
//				{
//					return result;
//				}
//			}
//		}
//	}
//
//	return std::nullopt;
//}
//
//Room::Room( cell const& _cell )
//{
//	constexpr auto partialWallWidth = ( room_size.width - 2 ) / 2;
//	constexpr auto partialWallHeight = ( room_size.height - 2 ) / 2;
//
//	int wall_count = 0;
//	auto generate_cell = [ & ]( dim2d::index idx )
//	{
//		auto hasWall = []( Wall subject, Wall control )
//		{
//			return ( subject & control ) == control;
//		};
//
//		int count = 0;
//		if( hasWall( _cell.wall, Wall::north ) )
//			++count;
//		if( hasWall( _cell.wall, Wall::east ) )
//			++count;
//		if( hasWall( _cell.wall, Wall::south ) )
//			++count;
//		if( hasWall( _cell.wall, Wall::west ) )
//			++count;
//		isDeadEnd = count == 3;
//
//		if( idx.x > 0 && idx.x < room_size.width - 1 &&
//			idx.y > 0 && idx.y < room_size.height - 1 )
//		{
//			return Tile( Tile::Type::floor, sprites.m_ground );
//		}
//
//		if( idx.y == 0 )
//		{
//			if( hasWall( _cell.wall, Wall::north ) )
//			{
//				if( idx.x == room_size.width - 1 )
//					return Tile( Tile::Type::wall, sprites.m_corner );
//				else
//					return Tile( Tile::Type::wall, sprites.m_northwall );
//			}
//
//			if( idx.x >= 0 && idx.x < partialWallWidth || 
//				idx.x >= partialWallWidth + 2 && idx.x < room_size.width )
//			{
//				if( idx.x == room_size.width - 1 )
//					return Tile( Tile::Type::wall, sprites.m_corner );
//				else
//					return Tile( Tile::Type::wall, sprites.m_northwall );
//			}
//		}
//		else if( idx.y > 0 && idx.y < room_size.height - 1 )
//		{
//			if( idx.x == 0 )
//			{
//				if( hasWall( _cell.wall, Wall::west ) )
//				{
//					return Tile( Tile::Type::wall, sprites.m_westwall );
//				}
//
//				if( idx.y > 0 && idx.y < partialWallHeight || 
//					idx.y >= partialWallHeight + 2 && idx.y < room_size.height )
//				{
//					return Tile( Tile::Type::wall, sprites.m_westwall );
//				}
//			}
//			else if( idx.x == room_size.height - 1 )
//			{
//				if( hasWall( _cell.wall, Wall::east ) )
//				{
//					return Tile( Tile::Type::wall, sprites.m_eastwall );
//				}
//
//				if( idx.y > 0 && idx.y < partialWallHeight ||
//					idx.y >= partialWallHeight + 2 && idx.y < room_size.height )
//				{
//					return Tile( Tile::Type::wall, sprites.m_eastwall );
//				}
//			}
//		}
//		else if( idx.y == room_size.height - 1 )
//		{
//			if( hasWall( _cell.wall, Wall::south ) )
//			{
//				if( idx.x == 0 )
//					return Tile( Tile::Type::wall, sprites.m_corner );
//				else
//					return Tile( Tile::Type::wall, sprites.m_southwall );
//			}
//
//			if( idx.x >= 0 && idx.x < partialWallWidth ||
//				idx.x >= partialWallWidth + 2 && idx.x < room_size.width )
//			{
//				if( idx.x == 0 )
//					return Tile( Tile::Type::wall, sprites.m_corner );
//				else
//					return Tile( Tile::Type::wall, sprites.m_southwall );
//			}
//		}
//
//		return Tile( Tile::Type::floor, sprites.m_ground );
//	};
//
//	dim2d::generate( m_tiles.begin(), m_tiles.end(), m_tiles.begin(), generate_cell );
//}
//
//void Room::AddItem( std::unique_ptr<ItemBase> _item )
//{
//	m_items.push_back( std::move( _item ) );
//}
//
//RoomItemList & Room::GetItems() noexcept
//{
//	return m_items;
//}
//
//RoomItemList const & Room::GetItems() const noexcept
//{
//	return m_items;
//}
//
//auto Room::GetTiles()->TileGrid&
//{
//	return m_tiles;
//}
//
//auto Room::GetTiles()const->TileGrid const&
//{
//	return m_tiles;
//}
//
