#include "StreamEmitter.h"


StreamEmitter::StreamEmitter(
	EmitterData && DataTemplate,
	const Vec2f &FacingDirection,
	float WidthOfStream )
	:
	Emitter( std::move( DataTemplate ) ),
	m_direction( FacingDirection ),
	m_width( WidthOfStream )
{
}

StreamEmitter::StreamEmitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles,
							  const Vec2f & FacingDirection, float WidthOfStream )
	:
	Emitter( Position, LaunchCount, MaxParticles ),
	m_direction( FacingDirection ),
	m_width( WidthOfStream )
{
}

void StreamEmitter::SetDirection( const Vec2f & Direction )
{
	m_direction = Direction;
}
