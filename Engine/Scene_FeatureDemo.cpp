#include "Scene_FeatureDemo.h"
#include <algorithm2d.h>
#include "Physics.h"

Scene_FeatureDemo::Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx ) noexcept
	:
	Scene( Kbd, Gfx ),
	m_maze( { maze_size.width - 1, maze_size.height / 2 }, { 0, maze_size.height / 2 } )
{
	start_index = { maze_size.width - 1, maze_size.height / 2 };
	end_index = { 0, maze_size.height / 2 };

	m_consolas = Font{ TextFormat::Properties{} };
	constexpr auto startPos = Vec2f(
		float( ( maze_size.width - 1 ) * room_pixel_size.width ),
		float( ( maze_size.height / 2 ) * room_pixel_size.height )
	) + Vec2f{ float( room_pixel_size.width / 2 ), float( room_pixel_size.height / 2 ) };
	m_ranger.SetPosition( startPos );

	const auto halfSize = Graphics::GetRect<float>().GetSize() * .5f;
	m_camera = Camera( m_ranger.GetPosition() - halfSize, Graphics::GetRect<float>() );
}

void Scene_FeatureDemo::Update( float dt )
{
	// Update hero
	m_ranger.Update( m_keyboard, dt );

	// Do collisions
	DoWallCollision();

	// Update camera
	const auto offset = m_ranger.GetPosition() - m_camera.GetRect().GetCenter();
	auto camPos = m_camera.GetRect().Translate( offset ).LeftTop();
	m_camera.SetPosition( camPos );

	// Clamp camera to maze boundries
	const auto left = 0.f;
	const auto top = 0.f;
	const auto right = maze_pixel_size.width - m_camera.GetRect().GetWidth();
	const auto bottom = maze_pixel_size.height - m_camera.GetRect().GetHeight();
	m_camera.ClampTo( { left, top, right, bottom } );

	// Check if hero reaches end room
	auto const hero_room_index = m_maze.GetRoomIndex( m_ranger.GetPosition() );
	if( hero_room_index.x == end_index.x && hero_room_index.y == end_index.y )
	{
		m_success = true;
	}
}

void Scene_FeatureDemo::Draw() const
{
	constexpr auto room_size = Sizef( room_pixel_size );
	auto view = m_camera.GetRect().Translate( m_camera.GetPosition() );

	view.left   = std::floorf( view.left / room_size.width );
	view.top    = std::floorf( view.top  / room_size.height );
	view.right  = std::ceilf( view.right  / room_size.width );
	view.bottom = std::ceilf( view.bottom / room_size.height );

	DrawMaze( view );
	DrawHero( view );
	DrawHUD( view );

}

void Scene_FeatureDemo::DoWallCollision() noexcept
{
	const auto room_rect = m_maze.GetRoomRect( m_ranger.GetPosition() );
	auto hero_rect = m_ranger.GetRect().Translate( m_ranger.GetPosition() ).ClipTo( room_rect );

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
					m_ranger.GetRect().Translate( ( *properties ).position ).ClipTo( room_rect );
			}
		}
	} while( isColliding );
}

void Scene_FeatureDemo::DrawMaze( RectF const & view ) const noexcept
{
	auto& rooms = m_maze.GetRooms();

	const auto view_offset = dim2d::offset{ int( view.left ), int( view.top ) };
	const dim2d::surface_wrapper<const dim2d::grid<Room, 25, 25>> wrapper(
		view_offset,
		int( view.GetWidth() ),
		int( view.GetHeight() ),
		rooms.columns(),
		rooms );

	// Draw tiles
	dim2d::for_each( wrapper.begin(), wrapper.end(),
		[ & ]( const dim2d::index _idx, const Room& _room )
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
	} );
}

void Scene_FeatureDemo::DrawHero( RectF const & view ) const noexcept
{
	auto rect = m_ranger.GetSpriteRect()
		.Translate( m_camera.WorldToScreen( m_ranger.GetPosition() ) );
	m_graphics.DrawSprite( rect, m_ranger.GetSprite(), AlphaEffect{ m_graphics } );
}

void Scene_FeatureDemo::DrawHUD( RectF const & view ) const noexcept
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
