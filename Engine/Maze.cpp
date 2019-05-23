#include "Maze.h"
#include "MathOps.h"
#include "RandomNumber.h"
#include "SpriteResource.h"

// Dim2d headers
#include <algorithm2d.h>
#include <grid.h>

// STL headers
#include <array>
#include <vector>

constexpr Vec2i operator+( const Vec2i& _left, const dim2d::offset& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}

constexpr Vec2i operator+( const dim2d::offset& _left, const Vec2i& _right )noexcept
{
	return { _left.x + _right.x, _left.y + _right.y };
}

CellGrid GenerateMaze( Vec2i _startPos, Vec2i _endPos, Random random )
{
	using cell_iterator = typename CellGrid::iterator;

	auto remove_walls = [ & ]( cell_iterator cur, Wall dwall1, cell_iterator next, Wall dwall2 )
	{
		cur->wall &= ~dwall1;
		next->wall &= ~dwall2;
	};
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

	auto choose_next_cell = []( std::vector<cell_iterator> available, Random random )
	{
		auto next = available[ random( 0, int( available.size() ) - 1 ) ];
		next->visited = true;
		return next;
	};
	
	auto clear_walls = [&]( cell_iterator cur, cell_iterator next )
	{
		dim2d::offset offset = next->pos - cur->pos;
		if(offset.x == -1)
		{
			remove_walls( cur, Wall::west, next, Wall::east );
		}
		else if( offset.x == 1 )
		{
			remove_walls( cur, Wall::east, next, Wall::west );
		}
		if( offset.y == -1 )
		{
			remove_walls( cur, Wall::north, next, Wall::south );
		}
		else if( offset.y == 1 )
		{
			remove_walls( cur, Wall::south, next, Wall::north );
		}
	};

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
			auto next = choose_next_cell( std::move( available ), random );
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
