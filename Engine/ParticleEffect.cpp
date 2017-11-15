#include "ParticleEffect.h"

void ParticleEffect::Update( float DeltaTime, const Vec2f & BasePos )
{
	Spawn( DeltaTime, BasePos );
	Remove();
	Collect();
}


