#include "SpriteResource.h"
#include "GlobalEnums.h"
#include <algorithm2d.h>
#include <random>

SpriteResource::SpriteResource()
{
	LoadHeroFrames();
	LoadObjectSprites();
	CreateWallSprites();
}

void SpriteResource::LoadHeroFrames()
{
	hero_idle[ int( Direction::Up ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\Back", "png" };
	hero_idle[ int( Direction::RightUp ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\BackRight", "png" };
	hero_idle[ int( Direction::Right ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\Right", "png" };
	hero_idle[ int( Direction::RightDown ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\FrontRight", "png" };
	hero_idle[ int( Direction::Down ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\Front", "png" };
	hero_idle[ int( Direction::LeftDown ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\FrontLeft", "png" };
	hero_idle[ int( Direction::Left ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\Left", "png" };
	hero_idle[ int( Direction::LeftUp ) ] =
		Frames{ 1, "Images\\Idle\\Hero\\BackLeft", "png" };

	hero_running[ int( Direction::Up ) ] =
		Frames{ 30, "Images\\Run\\Hero\\Back", "png" };
	hero_running[ int( Direction::RightUp ) ] =
		Frames{ 30, "Images\\Run\\Hero\\BackRight", "png" };
	hero_running[ int( Direction::Right ) ] =
		Frames{ 30, "Images\\Run\\Hero\\Right", "png" };
	hero_running[ int( Direction::RightDown ) ] =
		Frames{ 30, "Images\\Run\\Hero\\FrontRight", "png" };
	hero_running[ int( Direction::Down ) ] =
		Frames{ 30, "Images\\Run\\Hero\\Front", "png" };
	hero_running[ int( Direction::LeftDown ) ] =
		Frames{ 30, "Images\\Run\\Hero\\FrontLeft", "png" };
	hero_running[ int( Direction::Left ) ] =
		Frames{ 30, "Images\\Run\\Hero\\Left", "png" };
	hero_running[ int( Direction::LeftUp ) ] =
		Frames{ 30, "Images\\Run\\Hero\\BackLeft", "png" };
}

void SpriteResource::LoadObjectSprites()
{
	chest = Sprite( "Images\\Items\\chest.png" );
	key = Sprite( "Images\\Items\\key.png" );
}

void SpriteResource::CreateWallSprites()
{
	std::mt19937 rng;
	std::uniform_int_distribution<int> valDist( 48, 80 );
	auto generate_sprite = []( auto GenFn )->Sprite
	{
		auto tile = dim2d::surface<Color>( tile_size.width, tile_size.height );
		dim2d::generate( tile.begin(), tile.end(), tile.begin(), GenFn );
		return Sprite{ std::move( tile ) };
	};

	m_ground = generate_sprite( [ & ]( dim2d::index idx ) { return Colors::White * valDist( rng ); } );
	m_northwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::LightGray; } );
	m_westwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::LightGray; } );
	m_eastwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::DarkGray; } );
	m_southwall = generate_sprite( [ & ]( dim2d::index idx ) {return Colors::DarkGray; } );
	m_corner = generate_sprite( [ & ]( dim2d::index idx ) {
		return ( idx.x < ( tile_size.height - 1 ) - idx.y ) ?
			Colors::LightGray : Colors::DarkGray;
	} );
}
