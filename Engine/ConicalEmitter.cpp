#include "ConicalEmitter.h"
#include <algorithm>

ConicalEmitter::ConicalEmitter(
	EmitterData &&DataTemplate,
	const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius )
	:
	Emitter( std::move( DataTemplate ) ),
	m_distanceToBase( DistanceToBase ),
	m_baseRadius( BaseRadius )
{
	SetDirection( FacingDirection );
}

ConicalEmitter::ConicalEmitter(
	const Vec2f &Position, size_t LaunchCount, size_t MaxParticles,
	const Vec2 &FacingDirection, float DistanceToBase, float BaseRadius )
	:
	ConicalEmitter( { Position, LaunchCount, MaxParticles },
					FacingDirection, DistanceToBase, BaseRadius )
{
	SetDirection( FacingDirection );
}

void ConicalEmitter::SetDirection( const Vec2f & Direction )
{
	m_direction = ( Direction.LenSq() != 1.f ) ? Direction.Normalize() : Direction;

	const Vec2f center = position + ( m_direction * m_distanceToBase );
	const Vec2f leftRotatedDir = { -m_direction.y, m_direction.x };
	m_start = center + ( leftRotatedDir * m_baseRadius );
	m_end = center + ( ( -leftRotatedDir ) * m_baseRadius );
}
