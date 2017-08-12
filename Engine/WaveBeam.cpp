#include "WaveBeam.h"



WaveBeam::WaveBeam( Keyboard &Kbd )
	:
	m_keyboard( Kbd ),
	m_delay( .1f )
{
	const Vec2f basePos = Graphics::fScreenRect.GetCenter();
	const float launchCount = 5;
	const float preBufferCount = 4000;
	const Vec2f direction = { std::cosf( m_angle ), -std::sinf( m_angle ) };
	const float distanceToBase = 50.f;
	const float baseRadius = 10.f;

	m_emitter = ConicalEmitter(
		basePos,
		launchCount,
		preBufferCount,
		direction,
		distanceToBase,
		baseRadius );

	m_pParticles.push_back( &m_particles );
}

const std::vector<ParticleVector*>& WaveBeam::GetParticleVectors() const
{
	return m_pParticles;
}

void WaveBeam::Spawn( float DeltaTime, const Vec2f & BasePos )
{
	ParticleSetupDesc desc( 200.f, 20.f, 20.f, 20.f, 20.f, 1.f, 1.f, Colors::Magenta );

	m_delayCounter -= DeltaTime;

	const float angle = m_angle;
	const float rotSpeed = 100.f * DeltaTime;
	if( m_keyboard.KeyIsPressed( VK_LEFT ) )
	{
		m_angle += ToRadians( rotSpeed );
	}
	else if( m_keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		m_angle -= ToRadians( rotSpeed );
	}

	if( angle != m_angle )
	{
		m_angle = wrap_angle( m_angle );
		m_emitter.SetDirection( { std::cosf( m_angle ), -std::sinf( m_angle ) } );
	}

	if( m_keyboard.KeyIsPressed( VK_SPACE ) )
	{
		if( m_delayCounter <= 0.f )
		{
			m_delayCounter = m_delay;
			m_emitter.SpawnParticles( desc );
		}
	}
}

void WaveBeam::Remove()
{
	auto endIt = std::remove_if( m_particles.begin(), m_particles.end(), []( const Particle &P )
	{
		return P.IsDead();
	} );

	m_particles.erase( endIt, m_particles.end() );
}

void WaveBeam::Collect()
{
	auto &&particles = m_emitter.TakeParticles();
	for( auto &&particle : particles )
	{
		m_particles.emplace_back( std::move( particle ) );
	}
}
