#include "MachineGun.h"



MachineGun::MachineGun( Keyboard & Kbd )
	:
	m_keyboard( Kbd )
{
	m_pParticles.push_back( &m_Particles );
}

const std::vector<ParticleVector*>& MachineGun::GetParticleVectors() const
{
	return m_pParticles;
}

void MachineGun::Spawn( float DeltaTime, const Vec2f & BasePos )
{
	if( m_keyboard.KeyIsPressed( VK_SPACE ) )
	{
		if( m_delayCounter <= 0.f )
		{
			m_delayCounter = m_delay;
		}
	}	
}

void MachineGun::Remove()
{
	RemoveFrom( m_Particles );
}

void MachineGun::Collect()
{
	auto&& particles = m_Emitter.TakeParticles();
	for( auto&& particle : particles )
	{
		m_Particles.push_back( std::move( particle ) );
	}
}
