#include "Flames.h"
#include <future>

constexpr size_t partCount = 100u;

Flames::Flames()
	:
	m_water( { Graphics::fScreenWidth * .5f, 0.f }, partCount, 4000u, { 0.f, 1.f }, 100.f ),
	m_fire( { Graphics::fScreenWidth * .5f, Graphics::fScreenHeight }, partCount, 4000u, { 0.f, -1.f }, 100.f )
{
	m_pParticles.push_back( &m_particles );

	Sprite skull( "Images/SkullMask.png" );

	const auto width = skull.GetWidth();
	const auto height = skull.GetHeight();
	for( int y = 0; y < height; ++y )
	{
		for( int x = 0; x < width; ++x )
		{
			if( skull.ComparePixel( x, y, Colors::White ) )
			{
				m_points.push_back( static_cast< Vec2f >( Vec2i( x, y ) ) );
			}
		}
	}
}

const std::vector<ParticleVector*>& Flames::GetParticleVectors() const
{
	return m_pParticles;
}

void Flames::Spawn( float DeltaTime, const Vec2& BasePos )
{
	auto SpawnStream = 
		[this]( StreamEmitter &Stream,
				Color C, 
				float MinWidth, float MaxWidth, 
				float MinHeight, float MaxHeight, 
				float Minttl, float Maxttl, 
				float Spd )
	{
		ParticleSetupDesc desc;
		desc.color = C;
		desc.minWidth = MinWidth;
		desc.maxWidth = MaxWidth;
		desc.minHeight = MinHeight;
		desc.maxHeight = MaxHeight;
		desc.minTimeToLive = Minttl;
		desc.maxTimeToLive = Maxttl;
		desc.speed = Spd;
		desc.drawFunc = ParticleSetupDesc::RectAlpha;

		Stream.SpawnParticles( desc );
	};

	const float width = 4.f;
	const float height = width * 2.f;
	const float speed = 50.f;
	const float baseMinttl = .05f;
	const float baseMaxttl = baseMinttl * 2.f;
	const float minttl[] = { baseMinttl * 1.f, baseMinttl * 4.f, baseMinttl * 2.f };
	const float maxttl[] = { baseMaxttl * 3.f, baseMaxttl * 5.f, baseMaxttl * 4.f };
	const Color firePal[] = {		
		Color( Colors::Orange * 128ui8, 255ui8 ),
		Color( Colors::Yellow * 128ui8, 127ui8 ),
		Color( Colors::Red * 128ui8, 64ui8 )
	};

	m_fire.SetLaunchCount( 1 );

	for( int i = 0; i < m_points.size(); i += 4 )
	{
		const Vec2f offset = Vec2f( BasePos.x, BasePos.y - 2.f ) + m_points[ i ];
		m_fire.SetPosition( offset );
		for( int i = 0; i < 3; ++i )
		{
			SpawnStream(
				m_fire, firePal[ i ], width, width, height, height,
				minttl[ i ], maxttl[ i ], speed
			);
		}
	}
}

void Flames::Collect()
{
	auto CollectParticles = [this]( StreamEmitter &Stream )
	{
		auto particles = Stream.TakeParticles();

		for( auto &&particle : particles )
		{
			m_particles.emplace_back( std::move( particle ) );
		}
	};

	CollectParticles( m_water );
	CollectParticles( m_fire );
}

void Flames::Remove()
{
	auto remIt = std::remove_if( m_particles.begin(), m_particles.end(), []( const Particle &P )
	{
		return P.IsDead();
	} );

	m_particles.erase( remIt, m_particles.end() );
}
