#include "Scene_FeatureDemo.h"
#include "Chest.h"
#include "GlobalEnums.h"
#include "MathOps.h"
#include "Physics.h"
#include "SpriteResource.h"
#include <algorithm2d.h>

Scene_FeatureDemo::Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx ) noexcept
	:
	Scene( Kbd, Gfx ),
	m_consolas( TextFormat::Properties{} )/*,
	m_maze( { maze_size.width - 1, maze_size.height / 2 }, { 0, maze_size.height / 2 } )*/
{
	// Calculate starting position for hero
	start_index = { maze_size.width - 1, maze_size.height / 2 };
	end_index = { 0, maze_size.height / 2 };

	auto const cells = MazeGenerator{}( start_index, end_index );
	
	dim2d::transform( cells.begin(), cells.end(), m_tilemap.GetRooms().begin(),
		[ & ]( dim2d::index, cell const& _cell )
	{
		return Tilemap::Room( _cell, m_tilemap );
	} );

	constexpr auto room_pixel_size = Sizei{
		room_size.width * tile_size.width,
		room_size.height * tile_size.height
	};

	constexpr auto startPos = Vec2f(
		float( 21 * room_pixel_size.width ),
		float( 5 * room_pixel_size.height )
	) + Vec2f{ float( room_pixel_size.width / 2  + 20), float( room_pixel_size.height / 2 ) };
	m_ranger.SetPosition( startPos );

	// Position camera centered on hero
	const auto halfSize = Graphics::GetRect<float>().GetSize() * .5f;
	m_camera = Camera( m_ranger.GetPosition() - halfSize, Graphics::GetRect<float>() );

	// Find all dead end rooms
	dim2d::for_each(m_tilemap.GetRooms().begin(), m_tilemap.GetRooms().end(),
		[ & ]( dim2d::index idx, Tilemap::Room const& room )
	{
		if( room.IsDeadEnd() )
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
	
	auto constexpr half_size = room_size / 2;
	auto constexpr tile_idx = Vec2i{ half_size.width, half_size.height };
	for( auto i : indices )
	{
		// Calculate where to put the chests
		auto const room_idx = m_deadends[ i ];
		auto const room_position = Tilemap::room_to_world( Vec2i{ room_idx.x, room_idx.y } );
		auto const tile_position = 
			( Tilemap::tile_to_world( tile_idx ) + room_position ) +
			Vec2f( 0.f, float( tile_size.height ) - sprites.chest.GetHeight() );
	
		// Place chest
		auto key = InventoryItem{ "key", std::addressof( sprites.key ) };
		m_tilemap.GetRoom( room_idx ).AddItem(
			std::make_unique<Chest>( tile_position, sprites.chest, std::move( key ) )
		);
		
	
		// Remove chest rooms from dead end list
		m_deadends.erase( m_deadends.begin() + i );
	}
}

void Scene_FeatureDemo::Update( float dt )
{
	fps = 1.f / dt;
	// Update entities
	m_ranger.Update( m_keyboard, dt );

	// Do hero collisions
	// TODO: Gather all collidable items into vector
	{
		bool isColliding = false;
		do
		{
			auto hero_props = PhysicsPropertiesIn{
				m_ranger.GetPosition(),
				m_ranger.GetVelocity(),
				m_ranger.GetRect() + m_ranger.GetPosition(),
				true
			};

			if( auto props = DoWallCollision( hero_props ); props )
			{
				m_ranger.SetPosition( ( *props ).position );
				m_ranger.SetVelocity( ( *props ).velocity );
				isColliding = true;
			}
			else
			{
				isColliding = false;
			}
		} while( isColliding );
	}
	{
		bool isColliding = false;
		do
		{
			auto hero_props = PhysicsPropertiesIn{
				m_ranger.GetPosition(),
				m_ranger.GetVelocity(),
				m_ranger.GetRect() + m_ranger.GetPosition(),
				true
			};

			if( auto props = DoItemCollision( hero_props ); props )
			{
				m_ranger.SetPosition( ( *props ).position );
				m_ranger.SetVelocity( ( *props ).velocity );
				isColliding = true;
			}
			else
			{
				isColliding = false;
			}
		} while( isColliding );
	}
	// Do enemy collisions
	{
		
		// DoWallCollision( enemy_rect );
		// DoItemCollision( enemy_rect );
	}

	// Update camera
	{
		const auto offset = m_ranger.GetPosition() - m_camera.GetRect().GetCenter();
		auto camPos = ( m_camera.GetRect() + offset ).LeftTop();
		m_camera.SetPosition( camPos );
	}

	// Clamp camera to maze boundries
	{
		const auto position = Vec2f{ 0.f, 0.f };
		const auto size = Sizef{
			float( map_size.width * tile_size.width ) - m_camera.GetRect().GetWidth(),
			float( map_size.height * tile_size.height ) - m_camera.GetRect().GetHeight()
		};
		m_camera.ClampTo( { position, size } );
	}

	// Check if hero reaches end room
	{
		auto const hero_room_index = Tilemap::world_to_room( m_ranger.GetPosition() );
		
		if( IsAtEnd( hero_room_index ) )
		{
			m_success = true;
		}
	}
}

void Scene_FeatureDemo::Draw() const
{
	auto view = m_camera.GetRect() + m_camera.GetPosition();

	view.left   = std::floorf( view.left  / tile_size.width );
	view.top    = std::floorf( view.top   / tile_size.height );
	view.right  = std::ceilf( view.right  / tile_size.width );
	view.bottom = std::ceilf( view.bottom / tile_size.height );

	DrawMaze( view );
	DrawHero( );
	DrawHUD( );
}

std::optional<PhysicsPropertiesOut> 
Scene_FeatureDemo::DoWallCollision( PhysicsPropertiesIn const& props ) noexcept
{
	using TileType = Tilemap::Tile::Type;
	
	auto check_for_collision = [ & ]( int x, int y )
	{
		auto const tile_index = dim2d::index{ x,y };
		if( m_tilemap.GetTile( tile_index ).type == TileType::wall )
		{
			auto tile_position = Tilemap::tile_to_world( Vec2i{ tile_index.x, tile_index.y } );
			const auto tile_props = PhysicsPropertiesIn{
				Vec2f( 0.f,0.f ),
				Vec2f( 0.f,0.f ),
				Rectf( tile_position, Sizef( tile_size ) ),
				false
			};

			return HandleCollision( props, tile_props );
		}

		return std::optional<PhysicsPropertiesOut>{};
	};

	auto const left_top_idx = Tilemap::world_to_tile( props.boundingbox.LeftTop() );
	auto right_bottom_idx = Tilemap::world_to_tile( props.boundingbox.RightBottom() );
	{
		auto const right_bottom_position = Tilemap::tile_to_world( right_bottom_idx );
		if( right_bottom_position.x == props.boundingbox.right )
			--right_bottom_idx.x;
		if( right_bottom_position.y == props.boundingbox.bottom )
			--right_bottom_idx.y;
	}

	if( props.velocity.y > 0.f )
	{
		for( int y = left_top_idx.y; y <= right_bottom_idx.y; ++y )
		{
			if( props.velocity.x > 0.f )
			{
				for( int x = left_top_idx.x; x <= right_bottom_idx.x; ++x )
				{
					if( auto out_props = check_for_collision( x, y ); out_props )
					{
						return out_props;
					}
				}
			}
			else
			{
				for( int x = right_bottom_idx.x; x >= left_top_idx.x; --x )
				{
					if( auto out_props = check_for_collision( x, y ); out_props )
					{
						return out_props;
					}
				}
			}
		}
	}
	else
	{
		for( int y = right_bottom_idx.y; y >= left_top_idx.y; --y )
		{
			if( props.velocity.x > 0.f )
			{
				for( int x = left_top_idx.x; x <= right_bottom_idx.x; ++x )
				{
					if( auto out_props = check_for_collision( x, y ); out_props )
					{
						return out_props;
					}
				}
			}
			else
			{
				for( int x = right_bottom_idx.x; x >= left_top_idx.x; --x )
				{
					if( auto out_props = check_for_collision( x, y ); out_props )
					{
						return out_props;
					}
				}
			}
		}
	}

	return std::optional<PhysicsPropertiesOut>{};
}

std::optional<PhysicsPropertiesOut> Scene_FeatureDemo::DoItemCollision( PhysicsPropertiesIn const& props ) noexcept
{
	auto const room_idx = Tilemap::world_to_room( m_ranger.GetPosition() );
	auto const& room = m_tilemap.GetRoom( dim2d::index{ room_idx.x,room_idx.y } );
	auto const& items = room.GetItems();
	
	for( auto const& item : items )
	{
		const auto item_props = PhysicsPropertiesIn{
			Vec2f( 0.f,0.f ),
			Vec2f( 0.f,0.f ),
			item->GetRect() + item->GetPosition(),
			false
		};

		return HandleCollision( props, item_props );
	}

	return std::optional<PhysicsPropertiesOut>{};
}

void Scene_FeatureDemo::DrawMaze( RectF const & view ) const noexcept
{
	auto view_offset = dim2d::offset{ int( view.left ), int( view.top ) };
	{
		auto const& tiles = m_tilemap.GetTiles();
		const auto wrapper = dim2d::surface_wrapper<decltype(tiles)>(
			view_offset,
			int( view.GetWidth() ),
			int( view.GetHeight() ),
			tiles.columns(),
			tiles );

		// Draw tiles
		auto draw_tile = [ & ]( dim2d::index tile_index, Tilemap::Tile const& _tile )
		{
			tile_index = tile_index + view_offset;
			auto const tile_position = m_camera.WorldToScreen( 
				Tilemap::tile_to_world( Vec2i{ tile_index.x, tile_index.y } )
			);

			const auto dst = RectF( tile_position, Sizef{ tile_size } );

			auto const& sprite = *_tile;
			m_graphics.DrawSprite( dst, sprite, CopyEffect{ m_graphics } );
		};


		dim2d::for_each( wrapper.begin(), wrapper.end(), draw_tile );
	}

	{
		auto const& room =
			m_tilemap.GetRoom( Tilemap::world_to_room( m_ranger.GetPosition() ) );

		if( !room.GetItems().empty() )
		{
			for( auto const& item : room.GetItems() )
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
	}
}

void Scene_FeatureDemo::DrawHero( ) const noexcept
{
	auto const rect = m_ranger.GetSpriteRect() + 
		m_camera.WorldToScreen( m_ranger.GetPosition() );

	m_graphics.DrawSprite( rect, m_ranger.GetSprite(), AlphaEffect{ m_graphics } );
}

void Scene_FeatureDemo::DrawHUD( ) const noexcept
{
	// Draw dark translucent background
	m_graphics.DrawRectAlpha( m_ui_background, Color( 127, 0, 0, 0 ) );

	auto const charSize = m_consolas.GetCharSize();
	auto stringPos = Vec2f{
		float( m_ui_background.left + charSize.width ),
		float( m_ui_background.top + ( charSize.height ) )
	};

	// Draw frame rate
	{
		auto const frame_rate_string = "FPS: " + std::to_string( fps );
		m_graphics.DrawString( stringPos, m_consolas, frame_rate_string, Colors::White );
	}

	// Draw room index
	{
		stringPos.y += float( charSize.height );
		auto const& room_idx = Tilemap::world_to_room( m_ranger.GetPosition() );
		auto const room_idx_string =
			std::string( "Room index: " ) +
			std::to_string( room_idx.x ) + ", " +
			std::to_string( room_idx.y );

		m_graphics.DrawString( stringPos, m_consolas, room_idx_string, Colors::White );
	}

	if( m_success )
	{
		stringPos.y += float( charSize.height );
		m_graphics.DrawString( stringPos, m_consolas, "Success, you found the end.", Colors::Yellow );
	}


}

bool Scene_FeatureDemo::IsAtEnd( Vec2i const & room_idx ) const noexcept
{
	return room_idx.x == end_index.x && room_idx.y == end_index.y;
}
