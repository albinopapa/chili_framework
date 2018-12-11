#include "WaveBeam.h"



ParticleVector WaveBeam::Spawn( float DeltaTime, const Vec2f & BasePos )
{
	m_delayCounter -= DeltaTime;

	if( m_delayCounter <= 0.f )
	{
		ParticleSetupDesc desc( 200.f, 200.f, 20.f, 20.f, 20.f, 20.f, 1.f, 1.f, Colors::Magenta );
		m_delayCounter = m_delay;


		m_emitter.SpawnParticles( desc );
	}
}
