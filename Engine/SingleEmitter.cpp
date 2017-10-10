#include "SingleEmitter.h"


SingleEmitter::SingleEmitter(
	EmitterData &&DataTemplate, const Vec2f & FacingDirection )
	:
	Emitter( std::move( DataTemplate ) ),
	m_direction( FacingDirection )
{
}

SingleEmitter::SingleEmitter( const Vec2f & Position, size_t LaunchCount,
							  size_t MaxParticles, const Vec2f & FacingDirection )
	:
	SingleEmitter( { Position, LaunchCount, MaxParticles }, FacingDirection )
{
}

void SingleEmitter::SetDirection( const Vec2f & Dir )
{
	m_direction = Dir;
}
