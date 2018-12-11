#include "ParticleEffect.h"

void ParticleEffect::Update( float DeltaTime, const Vec2f & BasePos )
{
	Remove();
	Spawn( DeltaTime, BasePos );
	Collect();
}