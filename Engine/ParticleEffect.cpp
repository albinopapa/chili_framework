#include "ParticleEffect.h"

void ParticleEffect::RemoveFrom( ParticleVector & pParticles )
{
	auto endIt = std::remove_if( pParticles.begin(), pParticles.end(),
								 []( const std::unique_ptr<Particle> &pParticle )
	{
		return pParticle->IsDead();
	} );

	pParticles.erase( endIt, pParticles.end() );
}
