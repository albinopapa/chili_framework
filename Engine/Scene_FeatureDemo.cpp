#include "Scene_FeatureDemo.h"
#include "Chest.h"

#include "MathOps.h"
#include "Physics.h"
#include "SpriteResource.h"
#include <algorithm2d.h>

Scene_FeatureDemo::Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx ) noexcept
	:
	Scene( Kbd, Gfx ),
	m_consolas( TextFormat::Properties{} ),
	m_maze( { maze_size.width - 1, maze_size.height / 2 }, { 0, maze_size.height / 2 } )
{
	sprites.chest = Sprite( "Images\\Items\\chest.png" );
	sprites.key = Sprite( "Images\\Items\\key.png" );

	// Calculate starting position for hero
	start_index = { maze_size.width - 1, maze_size.height / 2 };
	end_index = { 0, maze_size.height / 2 };

	constexpr auto startPos = Vec2f(
		float( /*( maze_size.width - 1 )*/21 * room_pixel_size.width ),
		float( /*( maze_size.height / 2 ) */5* room_pixel_size.height )
	) + Vec2f{ float( room_pixel_size.width / 2  + 20), float( room_pixel_size.height / 2 ) };
	m_ranger.SetPosition( startPos );

	// Position camera centered on hero
	const auto halfSize = Graphics::GetRect<float>().GetSize() * .5f;
	m_camera = Camera( m_ranger.GetPosition() - halfSize, Graphics::GetRect<float>() );

	// Find all dead end rooms
	dim2d::for_each(m_maze.GetRooms().begin(),m_maze.GetRooms().end(),
		[ & ]( dim2d::index idx, Room const& room )
	{
		if( room.isDeadEnd )
			m_deadends.push_back( idx );
	} );

	// Randomly pick rooms to place key chests in
	std::mt19937 rng;
	std::array<int, 3> indices;
	
	auto remove_deadend = [&]( dim2d::index idx )
	{
		auto is_index = [&]( dim2d::index const& _idx )
		{
			return _idx.x == idx.x && _idx.y == idx.y;
		};
		if( auto it = std::find_if( m_deadends.begin(), m_deadends.end(), is_index );
			it != m_deadends.end() )
		{
			m_deadends.erase( it );
		}
	};

	remove_deadend( dim2d::index{ start_index.x, start_index.y } );
	remove_deadend( dim2d::index{ end_index.x, end_index.y } );

	for( auto& i : indices )
	{
		auto idx = [ & ] {
			do
			{
				std::uniform_int_distribution<int> idx_dist( 0, int( m_deadends.size() ) - 1 );
				auto idx = idx_dist( rng );
				if( auto it = std::find( indices.begin(), indices.end(), idx ); 
					it == indices.end() )
				{
					return idx;
				}
			} while( true );
		}( );

		i = idx;
	}

	// sort descending
	std::sort( indices.begin(), indices.end(), 
		[ & ]( int i, int j ){ return j < i; } 
	);

	for( auto i : indices )
	{
		// Calculate where to put the chests
		auto const room_idx = m_deadends[ i ];
		auto constexpr tile_idx = Vec2i{ room_size.width / 2, room_size.height / 2 };
		auto tile_position = 
			Vec2f( m_maze.GetTilePosition( Vec2i{ room_idx.x, room_idx.y }, tile_idx ) );

		tile_position.y += ( float( tile_size.height ) - sprites.chest.GetHeight() );

		// Place chest
		auto key = InventoryItem{ "key", std::addressof( sprites.key ) };
		auto& room = m_maze.GetRooms()[ room_idx ];
		room.AddItem(
			std::make_unique<Chest>( tile_position, sprites.chest, std::move( key ) ) 
		);

		// Remove chest rooms from dead end list
		m_deadends.erase( m_deadends.begin() + i );
	}
}

void Scene_FeatureDemo::Update( float dt )
{
	// Update entities
	m_ranger.Update( m_keyboard, dt );

	// Do collisions
	DoWallCollision();
	DoItemCollision();

	// Update camera
	{
		const auto offset = m_ranger.GetPosition() - m_camera.GetRect().GetCenter();
		auto camPos = ( m_camera.GetRect() + offset ).LeftTop();
		m_camera.SetPosition( camPos );
	}

	// Clamp camera to maze boundries
	{
		const auto left = 0.f;
		const auto top = 0.f;
		const auto right = maze_pixel_size.width - m_camera.GetRect().GetWidth();
		const auto bottom = maze_pixel_size.height - m_camera.GetRect().GetHeight();
		m_camera.ClampTo( { left, top, right, bottom } );
	}

	// Check if hero reaches end room
	{
		auto const hero_room_index = m_maze.GetRoomIndex( m_ranger.GetPosition() );
		if( hero_room_index.x == end_index.x && hero_room_index.y == end_index.y )
		{
			m_success = true;
		}
	}
}

void Scene_FeatureDemo::Draw() const
{
	constexpr auto room_size = Sizef( room_pixel_size );
	auto view = m_camera.GetRect() + m_camera.GetPosition();

	view.left   = std::floorf( view.left / room_size.width );
	view.top    = std::floorf( view.top  / room_size.height );
	view.right  = std::ceilf( view.right  / room_size.width );
	view.bottom = std::ceilf( view.bottom / room_size.height );

	DrawMaze( view );
	DrawHero( );
	DrawHUD( );

}

void Scene_FeatureDemo::DoWallCollision() noexcept
{
	const auto room_rect = m_maze.GetRoomRect( m_ranger.GetPosition() );
	auto hero_rect = ( m_ranger.GetRect() + m_ranger.GetPosition() ).ClipTo( room_rect );

	bool isColliding = false;
	do
	{
		isColliding = false;
		if( auto tile_rect = m_maze.GetTileRect( hero_rect, m_ranger.GetVelocity() ); tile_rect )
		{
			isColliding = true;
			const auto hero_props = 
				PhysicsPropertiesIn{ m_ranger.GetPosition(), m_ranger.GetVelocity(), hero_rect, true };

			const auto tile_props = 
				PhysicsPropertiesIn{ Vec2f( 0.f,0.f ), Vec2f( 0.f,0.f ), *tile_rect, false };

			if( auto properties = HandleCollision( hero_props, tile_props ); properties )
			{
				m_ranger.SetPosition( ( *properties ).position );
				m_ranger.SetVelocity( ( *properties ).velocity );
				hero_rect =
					( m_ranger.GetRect() + ( *properties ).position ).ClipTo( room_rect );
			}
		}
	} while( isColliding );
}

void Scene_FeatureDemo::DoItemCollision() noexcept
{
	auto const room_idx = m_maze.GetRoomIndex( m_ranger.GetPosition() );
	auto const& room = m_maze.GetRooms()[ dim2d::index{ room_idx.x,room_idx.y } ];
	auto const& items = room.GetItems();

	bool isColliding = false;
	do
	{
		auto const hero_rect = ( m_ranger.GetRect() + m_ranger.GetPosition() );

		isColliding = false;
		for( auto const& item : items )
		{
			auto const item_rect = item->GetRect() + item->GetPosition();
			if( item_rect.Overlaps( hero_rect ) )
			{
				isColliding = true;
				const auto hero_props =
					PhysicsPropertiesIn{ m_ranger.GetPosition(), m_ranger.GetVelocity(), hero_rect, true };

				const auto item_props =
					PhysicsPropertiesIn{ Vec2f( 0.f,0.f ), Vec2f( 0.f,0.f ), item_rect, false };

				if( auto properties = HandleCollision( hero_props, item_props ); properties )
				{
					m_ranger.SetPosition( ( *properties ).position );
					m_ranger.SetVelocity( ( *properties ).velocity );
					break;
				}
			}
		}
	} while( isColliding );

}

void Scene_FeatureDemo::DrawMaze( RectF const & view ) const noexcept
{
	auto& rooms = m_maze.GetRooms();

	const auto view_offset = dim2d::offset{ int( view.left ), int( view.top ) };
	const dim2d::surface_wrapper<const RoomGrid> wrapper(
		view_offset,
		int( view.GetWidth() ),
		int( view.GetHeight() ),
		rooms.columns(),
		rooms );

	// Draw tiles
	auto draw_rooms = [ & ]( const dim2d::index _idx, const Room& _room )
	{
		const auto room_idx = view_offset + _idx;

		const auto room_offset = m_camera.WorldToScreen( {
			float( room_idx.x * room_pixel_size.width ),
			float( room_idx.y * room_pixel_size.height )
			} );
		
		if( Graphics::GetRect<float>().Overlaps( RectF( room_offset, Sizef( room_pixel_size ) ) ) )
		{
			auto const& tiles = _room.GetTiles();

			auto draw_tile = [ & ]( dim2d::index tile_index, Tile const& _tile )
			{
				m_maze.GetTilePosition( Vec2i( room_offset ), Vec2i{ tile_index.x,tile_index.y } );
				const auto tile_offset = Vec2f(
					room_offset.x + float( tile_index.x * tile_size.width ),
					room_offset.y + float( tile_index.y * tile_size.height )
				);

				const auto dst = RectF( tile_offset, tile_size );

				auto const& sprite = *_tile;
				m_graphics.DrawSprite( sprite.GetRect(), dst, *_tile, CopyEffect{ m_graphics } );


			};

			dim2d::for_each( tiles.begin(), tiles.end(), draw_tile );
		}
	};

	dim2d::for_each( wrapper.begin(), wrapper.end(), draw_rooms );

	auto draw_items = [ & ]( dim2d::index idx, Room const& _room )
	{
		if( !_room.m_items.empty() )
		{
			for( auto const& item : _room.m_items )
			{
				auto item_position = m_camera.WorldToScreen( item->GetPosition() );

				auto item_rect = item->GetSpriteRect() + item_position;
				if( Graphics::GetRect<float>().Overlaps( item_rect ) )
				{
					m_graphics.DrawSprite(
						item_rect,
						item->GetSprite(),
						AlphaEffect{ m_graphics }
					);
				}
			}
		}
	};
	dim2d::for_each( wrapper.begin(), wrapper.end(), draw_items );
}

void Scene_FeatureDemo::DrawHero( ) const noexcept
{
	auto const rect = m_ranger.GetSpriteRect() + 
		m_camera.WorldToScreen( m_ranger.GetPosition() );

	m_graphics.DrawSprite( rect, m_ranger.GetSprite(), AlphaEffect{ m_graphics } );
}

void Scene_FeatureDemo::DrawHUD( ) const noexcept
{
	auto const charSize = m_consolas.GetCharSize();
	auto const& room_idx = m_maze.GetRoomIndex( m_ranger.GetPosition() );
	auto stringPos = Vec2f{
			float( m_ui_background.left + charSize.width ),
			float( m_ui_background.top + charSize.height )
	};
	auto const room_idx_string =
		std::string( "Room index: " ) +
		std::to_string( room_idx.x ) + ", " +
		std::to_string( room_idx.y );

	m_graphics.DrawRectAlpha( m_ui_background, Color( 127, 0, 0, 0 ) );
	m_graphics.DrawString( stringPos, m_consolas, room_idx_string, Colors::White );

	if( m_success )
	{
		stringPos.y += float( charSize.height );
		m_graphics.DrawString( stringPos, m_consolas, "Success, you found the end.", Colors::Yellow );
	}
}
