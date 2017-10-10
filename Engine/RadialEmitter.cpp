#include "RadialEmitter.h"


RadialEmitter::RadialEmitter( EmitterData &&DataTemplate )
	:
	Emitter( std::move( DataTemplate ) )
{
	InitCommon();
}

RadialEmitter::RadialEmitter( const Vec2f & Position, size_t LaunchCount, size_t MaxParticles )
	:
	RadialEmitter( EmitterData{ Position, LaunchCount, MaxParticles } )
{}

void RadialEmitter::InitCommon()
{
	if( launchCount > maxParticles )
	{
		std::swap( launchCount, maxParticles );
	}

	m_bursts.reserve( launchCount );

	const float radianStep = ( 3.141592f / 180.f ) * ( 360.f / launchCount );
	for( size_t i = 0u; i < launchCount; ++i )
	{
		const auto angle = radianStep * i;
		m_bursts.emplace_back( std::cos( angle ), std::sin( angle ) );
	}
}
