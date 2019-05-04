#include "Maze.h"

dim2d::grid<cell, maze_width, maze_height> MazeGenerator::operator()(dim2d::offset _startPos)
{
	using cell_iterator = typename dim2d::grid<cell, maze_width, maze_height>::iterator;
	auto get_available_neighbors = 
		[](cell_iterator cur, dim2d::grid<cell, maze_width, maze_height>& cells)
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
			constexpr auto mazeRect = RectI( 0, 0, maze_width, maze_height );
			if( mazeRect.Contains( Vec2i{ neighbor.x, neighbor.y } ) )
			{
				auto it = cells.begin() + neighbor;

				if( !it->visited )
				{
					available.push_back( it );
				}
			}
		}

		return available;
	};

	auto choose_next_cell = []( std::vector<cell_iterator> available, auto& rng )
	{
		const auto index =
			std::uniform_int_distribution<std::size_t>{ 0, available.size() - 1 }( rng );

		auto next = available[ index ];
		auto& nCell = *next;
		nCell.visited = true;
		return next;
	};

	auto clear_walls = []( cell& cur, cell& next )
	{
		dim2d::offset offset = next.pos - cur.pos;

		choose(
			choice(
			[ & ] { return offset.x == -1; },
			[ & ] { next.wall &= ~Wall::east; cur.wall &= ~Wall::west; } ),
			choice(
			[ & ] { return offset.x == 1; },
			[ & ] { next.wall &= ~Wall::west; cur.wall &= ~Wall::east; } )
		);
		choose(
			choice(
			[ & ] { return offset.y == -1; },
			[ & ] { next.wall &= ~Wall::south; cur.wall &= ~Wall::north; } ),
			choice(
			[ & ] { return offset.y == 1; },
			[ & ] { next.wall &= ~Wall::north; cur.wall &= ~Wall::south; } )
		);
	};

	std::mt19937 rng;

	dim2d::grid<cell, maze_width, maze_height> cells;
	dim2d::generate( cells.begin(), cells.end(), cells.begin(),
		[ & ]( dim2d::index idx ) { return cell{ idx }; } );

	std::vector<cell_iterator> visited;
	visited.reserve( maze_width * maze_height );
	visited.emplace_back( cells.begin() + _startPos )->visited = true;

	while( !visited.empty() )
	{
		auto cur = visited.back();
		auto& cCell = *cur;

		const auto available = get_available_neighbors( cur, cells );
		if( !available.empty() )
		{
			auto next = choose_next_cell( std::move( available ), rng );
			auto& nCell = *next;
			
			clear_walls( cCell, nCell );

			visited.push_back( next );
		}
		else
		{
			visited.pop_back();
		}
	}

	return cells;
}

Maze::Maze( dim2d::offset _startPos )
{
	std::mt19937 rng;
	std::uniform_int_distribution<int> valDist( 112, 144 );
	auto generate_sprite = []( auto GenFn )->Sprite
	{
		auto tile = dim2d::surface<Color>( tile_width, tile_height );
		dim2d::generate( tile.begin(), tile.end(), tile.begin(), GenFn );
		return Sprite{ std::move( tile ) };
	};

	m_ground	= generate_sprite( [ & ]( dim2d::index idx ) { return Colors::White * valDist( rng ); } );
	m_northwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::White; } );
	m_westwall	= generate_sprite( [ & ]( dim2d::index idx ) {return Colors::White; } );
	m_eastwall	= generate_sprite( [ & ]( dim2d::index idx ) {return Colors::Black; } );
	m_southwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::Black; } );

	auto cells = MazeGenerator{}( _startPos );

	dim2d::transform( cells.begin(), cells.end(), m_rooms.begin(),
		[ & ]( dim2d::index, cell const& _cell )
	{
		return Room( m_ground, m_northwall, m_eastwall, m_southwall, m_westwall, _cell );
	} );

}

auto Maze::GetRooms()
->dim2d::grid<Room, maze_width, maze_height>&
{
	return m_rooms;
}

auto Maze::GetRooms()const
->dim2d::grid<Room, maze_width, maze_height> const&
{
	return m_rooms;
}

dim2d::offset Maze::GetRoomIndex( const Vec2f & _position ) const noexcept
{
	return dim2d::offset{
		int( std::floorf( _position.x / room_pixel_width ) ),
		int( std::floorf( _position.y / room_pixel_height ) )
	};
}

Vec2i Maze::GetRoomPosition( dim2d::offset const & room_idx ) const noexcept
{
	return Vec2i(room_idx.x * room_pixel_width,room_idx.y * room_pixel_height);
}

dim2d::offset Maze::GetTileIndex( const dim2d::offset& room_index, const Vec2f& _position, bool isBack ) const noexcept
{
	const auto room_position = GetRoomPosition( room_index );
	auto tile_index = dim2d::offset{
		( int( std::floorf( _position.x ) ) - room_position.x ) / tile_width,
		( int( std::floorf( _position.y ) ) - room_position.y ) / tile_height
	};
	tile_index.x = std::clamp( tile_index.x, 0, room_width );
	tile_index.y = std::clamp( tile_index.y, 0, room_height );

	if( isBack )
	{
		const auto temp = GetTilePosition( room_index, tile_index );
		if( float( temp.x ) == _position.x )
			--tile_index.x;
		if( float( temp.y ) == _position.y )
			--tile_index.y;
	}
	return tile_index;
}

Vec2i Maze::GetTilePosition( dim2d::offset const & room_idx, dim2d::offset const& tile_index ) const noexcept
{
	auto const room_position = GetRoomPosition( room_idx );
	return Vec2i(
		( tile_index.x * tile_width ) + room_position.x,
		( tile_index.y * tile_height ) + room_position.y 
	);
}


std::optional<Rectf> Maze::GetTileRect( const Rectf& rect, const Vec2f& velocity )const noexcept
{
	const auto room_idx			= GetRoomIndex( rect.GetCenter() );
	const auto room_position	= GetRoomPosition( room_idx );

	auto check_for_wall =
		[&]( dim2d::grid<Tile, room_width, room_height> const& tiles, dim2d::index const& tile_idx )
	{
		auto const rect =
			Recti( GetTilePosition( room_idx, tile_idx ), Sizei{ tile_width, tile_height } );

		auto result = ( tiles[ tile_idx ].type == Tile::Type::wall ) ?
			std::optional<Rectf>( rect ) : std::optional<Rectf>();

		return result;
	};

	auto const left_top_idx		= GetTileIndex( room_idx, rect.LeftTop() );
	auto const right_bottom_idx = GetTileIndex( room_idx, rect.RightBottom(), true );

	auto const& tiles			= m_rooms[ room_idx ].GetTiles();

	for( int y = left_top_idx.y; y <= right_bottom_idx.y; ++y )
	{
		if( velocity.x > 0.f )
		{
			for( int x = left_top_idx.x; x <= right_bottom_idx.x; ++x )
			{
				if( auto result	= check_for_wall( tiles, dim2d::index{ x,y } ); result.has_value() )
				{
					return result;
				}
			}
		}
		else
		{
			for( int x = right_bottom_idx.x; x >= left_top_idx.x; --x )
			{
				if( auto result = check_for_wall( tiles, dim2d::index{ x,y } ); result.has_value() )
				{
					return result;
				}
			}
		}
	}

	return std::nullopt;
}

Room::Room(
	Sprite const& _ground,
	Sprite const& _northwall,
	Sprite const& _eastwall,
	Sprite const& _southwall,
	Sprite const& _westwall,
	cell const& _cell )
{
	auto generate_cell = [ & ]( dim2d::index idx )
	{
		if( idx.x > 0 && idx.x < room_width - 1 &&
			idx.y > 0 && idx.y < room_height - 1 )
		{
			return Tile( Tile::Type::floor, _ground );
		}

		if( idx.y == 0 )
		{
			if( ( _cell.wall & Wall::north ) == Wall::north )
			{
				return Tile( Tile::Type::wall, _northwall );
			}

			if( idx.x >= 0 && idx.x <= 3 || idx.x >= 6 && idx.x < 10 )
			{
				return Tile( Tile::Type::wall, _northwall );
			}
		}
		else if( idx.y > 0 && idx.y < room_height - 1 )
		{
			if( idx.x == 0 )
			{
				if( ( _cell.wall & Wall::west ) == Wall::west )
				{
					return Tile( Tile::Type::wall, _westwall );
				}

				if( idx.y >= 0 && idx.y <= 3 || idx.y >= 6 && idx.y < 10 )
				{
					return Tile( Tile::Type::wall, _eastwall );
				}
			}
			else if( idx.x == room_height - 1 )
			{
				if( ( _cell.wall & Wall::east ) == Wall::east && idx.x == room_width - 1 )
				{
					return Tile( Tile::Type::wall, _eastwall );
				}

				if( idx.y >= 0 && idx.y <= 3 || idx.y >= 6 && idx.y < 10 )
				{
					return Tile( Tile::Type::wall, _westwall );
				}
			}
		}
		else if( idx.y == room_height - 1 )
		{
			if( ( _cell.wall & Wall::south ) == Wall::south && idx.y == room_height - 1 )
			{
				return Tile( Tile::Type::wall, _southwall );
			}

			if( idx.x >= 0 && idx.x <= 3 || idx.x >= 6 && idx.x < 10 )
			{
				return Tile( Tile::Type::wall, _southwall );
			}
		}

		return Tile( Tile::Type::floor, _ground );
	};

	dim2d::generate( m_tiles.begin(), m_tiles.end(), m_tiles.begin(), generate_cell );
}

auto Room::GetTiles()
->dim2d::grid<Tile, room_width, room_height>&
{
	return m_tiles;
}

auto Room::GetTiles()const
->dim2d::grid<Tile, room_width, room_height>const&
{
	return m_tiles;
}

