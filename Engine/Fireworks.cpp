#include "Fireworks.h"


Fireworks::Fireworks()
	:
	m_burstEmitter( { 0.f, 0.f }, 100, 4000 ),
	m_rng( std::random_device()( ) ),
	m_colorDist( 0, ( sizeof( m_colorPalette ) / sizeof( Color ) - 1 ) ),
	m_delayDist( .8f, 1.8f )	
{
	m_pParticles = std::vector<ParticleVector *>{ &m_primary, &m_secondary };

	const Vec2f emitPos = { Graphics::fScreenWidth * .5f, Graphics::fScreenHeight - 1.f };
	m_baseEmitter =
		SingleEmitter( emitPos, 1, 1, Vec2f{ 0.f, -1.f } );

	const float divisions = static_cast<float>( TimeSpawned::m_count + 1 );
	const float spacing = Graphics::fScreenWidth / divisions;
	const float baseXStart = spacing;
	for( size_t i = 0; i < TimeSpawned::m_count; ++i )
	{
		m_baseEmitterData.m_currentDelay[ i ] = m_delayDist( m_rng );
		const float offsetX = spacing * static_cast<float>( i );
		m_baseEmitterData.m_positions[i] = { baseXStart + offsetX, emitPos.y };
	}
}

const std::vector<ParticleVector*>& Fireworks::GetParticleVectors() const
{
	return m_pParticles;
}

void Fireworks::Spawn( float DeltaTime, const Vec2f & BasePos )
{
	EmitPrimary( DeltaTime );
	EmitSecondary();
}

void Fireworks::Remove()
{
	RemoveFrom( m_primary );
	RemoveFrom( m_secondary );
}

void Fireworks::Collect()
{
	auto CollectParticles = [ this ]( Emitter &_Emitter, ParticleVector &Particles )
	{
		auto pParticles = _Emitter.TakeParticles();
		Particles.reserve( Particles.size() + pParticles.size() );

		for( auto &&pParticle : pParticles )
		{
			Particles.emplace_back( std::move( pParticle ) );
		}
	};
	CollectParticles( m_baseEmitter, m_primary );
	CollectParticles( m_burstEmitter, m_secondary );
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

			const float speed = 350.f;
			const float minWidth = 5.f;
			const float maxWidth= 5.f;
			const float minTTL = .016f * 40.f;
			const float maxTTL = .016f * 70.f;
			const Color color = Colors::Orange;

			m_baseEmitter.SetPosition( position );

			m_baseEmitter.SpawnParticles<Particle>( {
				speed, minWidth, maxWidth, minWidth, maxWidth, minTTL, maxTTL, color
			} );
		}
	}
}

void Fireworks::EmitSecondary()
{
	for( const auto &particle : m_primary )
	{
		if( particle->IsDead() )
		{
			const auto colorIndex = m_colorDist( m_rng );

			const float speed = 150.f;
			const float minRadius = 8.f;
			const float maxRadius = 12.f;
			const float minTTL = .016f * 80.f;
			const float maxTTL = .016f * 80.f;
			const Color color = m_colorPalette[ colorIndex ];

			m_burstEmitter.SetPosition( particle->Position() );
			m_burstEmitter.SpawnParticles<Particle>( {
				speed, minRadius, maxRadius, minRadius, maxRadius, minTTL, maxTTL, color }
			);
		}
	}
}
