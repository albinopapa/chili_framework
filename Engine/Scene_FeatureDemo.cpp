#include "Scene_FeatureDemo.h"
#include <algorithm2d.h>
#include "Physics.h"

Scene_FeatureDemo::Scene_FeatureDemo( Keyboard &Kbd, Graphics &Gfx ) noexcept
	:
	Scene( Kbd, Gfx ),
	m_maze( { maze_width - 1, maze_height / 2 } )
{
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
	const auto right = maze_pixel_width - m_camera.GetRect().GetWidth();
	const auto bottom = maze_pixel_height - m_camera.GetRect().GetHeight();
	m_camera.ClampTo( { left, top, right, bottom } );
}

void Scene_FeatureDemo::Draw() const
{
	constexpr auto room_size = Sizef( float( room_pixel_width ), float( room_pixel_height ) );
	constexpr auto tile_size = Sizef( float( tile_width ), float( tile_height ) );

	auto& rooms = m_maze.GetRooms();

	auto view = m_camera.GetRect().Translate( m_camera.GetPosition() );

	view.left   = std::floorf( view.left / room_pixel_width );
	view.top    = std::floorf( view.top / room_pixel_height );
	view.right  = std::ceilf( view.right / room_pixel_width );
	view.bottom = std::ceilf( view.bottom / room_pixel_height );

	const auto view_offset = dim2d::offset{ int( view.left ), int( view.top ) };
	const dim2d::surface_wrapper<const dim2d::grid<Room, 25, 25>> wrapper(
		view_offset,
		int( view.GetWidth() ),
		int( view.GetHeight() ),
		rooms.columns(),
		rooms );

	dim2d::for_each( wrapper.begin(), wrapper.end(),
		[ & ]( const dim2d::index _idx, const Room& _room )
	{
		const auto room_idx = view_offset + _idx;
		const auto room_offset = m_camera.WorldToScreen( {
			float( room_idx.x * room_pixel_width ),
			float( room_idx.y * room_pixel_height )
			} );

		if( Graphics::GetRect<float>().Overlaps( RectF( room_offset, room_size ) ) )
		{
			auto const& tiles = _room.GetTiles();

			dim2d::for_each(tiles.begin(),tiles.end(),
				[ & ]( dim2d::index tile_index, Tile const& _tile )
			{
				const auto tile_offset = Vec2f(
					room_offset.x + float( tile_index.x * tile_width ),
					room_offset.y + float( tile_index.y * tile_height )
				);

				const auto dst = RectF( tile_offset, tile_size );

				auto const& sprite = *_tile;
				m_graphics.DrawSprite( sprite.GetRect(), dst, *_tile );
				m_graphics.DrawRect( dst, Colors::Red );
			} );
		}
	} );

	{
		auto rect = m_ranger.GetSpriteRect()
			.Translate( m_camera.WorldToScreen( m_ranger.GetPosition() ) );
		m_graphics.DrawSprite( rect, m_ranger.GetSprite() );
	}
	{

		const auto rect = m_ranger.GetRect().Translate( m_camera.WorldToScreen( m_ranger.GetPosition() ) );
		m_graphics.DrawRect( rect, Colors::Red );
	}

}

RectI Scene_FeatureDemo::CalculateBoundingRegion( RectF const& hero_rect, dim2d::offset pixel_position ) const noexcept
{
	return RectI(
		int( std::floorf( ( hero_rect.Left() - pixel_position.x ) / tile_width ) ),
		int( std::floorf( ( hero_rect.Top() - pixel_position.y ) / tile_height ) ),
		int( std::ceilf( ( hero_rect.Right() - pixel_position.x ) / tile_width ) ),
		int( std::ceilf( ( hero_rect.Bottom() - pixel_position.y ) / tile_height ) )
	);
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
			PhysicsPropertiesIn hero_props = {
				m_ranger.GetPosition(),
				m_ranger.GetVelocity(),
				hero_rect,
				true
			};
			PhysicsPropertiesIn tile_props = {
				Vec2f( 0.f,0.f ),
				Vec2f( 0.f,0.f ),
				*tile_rect,
				false
			};

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

Hero::Hero()
	:
	m_sprite( "Images\\RangerStandCentered00.png" )
{
	m_position.x = ( ( maze_width - 1 ) * room_pixel_width ) + ( room_pixel_width / 2 );
	m_position.y = ( ( maze_height / 2 ) * room_pixel_height ) + ( room_pixel_height / 2 );
}

void Hero::Update( Keyboard& _keyboard, float dt )
{
	const Vec2f delta = {
		_keyboard.KeyIsPressed( VK_LEFT ) ?
		-1.f : _keyboard.KeyIsPressed( VK_RIGHT ) ?
		1.f : 0.f,
		_keyboard.KeyIsPressed( VK_UP ) ?
		-1.f : _keyboard.KeyIsPressed( VK_DOWN ) ?
		1.f : 0.f 
	};

	dt = std::min( dt, .016f );
	m_velocity = delta.Normalize() * ( speed * dt );

	m_position += m_velocity;
}

Vec2f const & Hero::GetPosition() const noexcept
{
	return m_position;
}

Vec2f const & Hero::GetVelocity() const noexcept
{
	return m_velocity;
}

RectF Hero::GetRect() const noexcept
{
	return m_rect;
}

RectF Hero::GetSpriteRect() const noexcept
{
	return RectF( m_sprite.GetRect() ).Translate( m_sprite_rect_offset );
}

Sprite const & Hero::GetSprite() const noexcept
{
	return m_sprite;
}

void Hero::SetPosition( Vec2f _position ) noexcept
{
	m_position = _position;
}

void Hero::SetVelocity( Vec2f _velocity ) noexcept
{
	m_velocity = _velocity;
}
