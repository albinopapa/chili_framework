#include "Fireworks.h"
#include "Scene.h"

template<class T, class U>
constexpr T scast( const U &V )
{
	return static_cast< T >( V ); 
}

Fireworks::Fireworks()
	:	
	m_burstEmitter( { 0.f, 0.f }, 100, 4000 ),
	m_rng( std::random_device()( ) ),
	m_colorDist( 0, ( sizeof( m_colorPalette ) / sizeof( Color ) - 1 ) ),
	m_delayDist( .8f, 1.8f ),	
	m_particles( std::vector<ParticleVector *>{ &m_primary, &m_secondary } )
{
	m_baseEmitter =
		SingleEmitter( { fScreenWidth * .5f, fScreenHeight - 1.f }, 1, 1, Vec2f{ 0.f, -1.f } );

	const float divisions = scast<float>( TimeSpawned::m_count + 1 );
	const float spacing = fScreenWidth / divisions;
	const float baseXStart = spacing;
	for( size_t i = 0; i < TimeSpawned::m_count; ++i )
	{
		m_baseEmitterData.m_currentDelay[ i ] = m_delayDist( m_rng );
		const float offsetX = spacing * scast<float>( i );
		m_baseEmitterData.m_positions[i] = { baseXStart + offsetX, fScreenHeight - 1.f };
	}
}

void Fireworks::Update( float DeltaTime )
{
	auto CollectParticles = [ this ]( Emitter &_Emitter, ParticleVector &Particles )
	{
		auto particles = _Emitter.TakeParticles();
		Particles.reserve( Particles.size() + particles.size() );

		for( auto &&particle : particles )
		{
			Particles.emplace_back( std::move( particle ) );
		}
	};
	auto RemoveDeadParticles = []( ParticleVector &Particles )
	{
		auto endIt = std::remove_if( Particles.begin(), Particles.end(), []( std::unique_ptr<Particle> &pParticle )
		{
			return pParticle->IsDead();
		} );

		Particles.erase( endIt, Particles.end() );
	};

	EmitPrimary( DeltaTime );
	EmitSecondary();

	CollectParticles( m_baseEmitter, m_primary );
	CollectParticles( m_burstEmitter, m_secondary );

	RemoveDeadParticles( m_primary );
	RemoveDeadParticles( m_secondary );
}

const std::vector<ParticleVector*>& Fireworks::GetParticleVectors() const
{
	return m_particles;
}

void Fireworks::EmitPrimary( float DeltaTime )
{
	for( size_t i = 0; i < TimeSpawned::m_count; ++i )
	{
		auto &launchCounter = m_baseEmitterData.m_launchCounter[ i ];
		auto &currentDelay =  m_baseEmitterData.m_currentDelay[ i ];
		auto &position = m_baseEmitterData.m_positions[ i ];

		launchCounter -= DeltaTime;
		if( launchCounter <= 0.f )
		{
			currentDelay = m_delayDist( m_rng );
			launchCounter = currentDelay;

			const float speed = 5.f;
			const float minRadius = 5.f;
			const float maxRadius = 5.f;
			const float minTTL = .016f * 40.f;
			const float maxTTL = .016f * 70.f;
			const Color color = Colors::Orange;

			m_baseEmitter.SetPosition( position );

			m_baseEmitter.SpawnParticles( {
				speed, minRadius, maxRadius, minTTL, maxTTL, color
			} );
		}
	}
}

void Fireworks::EmitSecondary()
{
	for( const auto &pParticle : m_primary )
	{
		if( pParticle->IsDead() )
		{
			const auto colorIndex = m_colorDist( m_rng );

			const float speed = 5.f;
			const float minRadius = 4.f;
			const float maxRadius = 6.f;
			const float minTTL = .016f * 80.f;
			const float maxTTL = .016f * 80.f;
			const Color color = m_colorPalette[ colorIndex ];

			m_burstEmitter.SetPosition( pParticle->Position() );
			m_burstEmitter.SpawnParticles( {
				speed, minRadius, maxRadius, minTTL, maxTTL, color }
			);
		}
	}
}
