#include "Scene_Particles.h"


Scene_Particles::Scene_Particles( Keyboard & Kbd, Graphics & Gfx )
	:
	Scene( Kbd, Gfx ),
	m_camera( { 400.f, fScreenHeight - 1.f } ),
	m_levelrect( Graphics::fScreenRect ),
	m_screenrect( m_levelrect ),
	m_rng( std::random_device()( ) ),
	m_colorDist( 0, ( sizeof( m_colorPalette ) / sizeof( Color ) - 1 ) )
{
	Emitter::Description emDesc{};
	emDesc.m_position = { 400.f, fScreenHeight - 1.f };
	emDesc.m_direction = { 0.f, -1.f };
	emDesc.m_maxParticles = 100;
	emDesc.m_minDelay = .1f;
	emDesc.m_maxDelay = .2f;
	emDesc.m_infinite = true;

	Particle::Description ptDesc{};
	ptDesc.m_minParticleRadius = 5.f;
	ptDesc.m_maxParticleRadius = 5.f;
	ptDesc.m_minParticleTimeToLive = .25f;
	ptDesc.m_maxParticleTimeToLive = 1.f;
	ptDesc.m_particleColor = Colors::Orange;

	m_baseEmitter = Emitter( ptDesc, emDesc );
}

void Scene_Particles::Update( float DeltaTime )
{
	m_camera.Update( DeltaTime );
	m_camera.ClampTo( m_screenrect.GetSize(), m_levelrect );
	
	const auto viewport = MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );

	m_baseEmitter.Update( DeltaTime, viewport );
	const auto &deadParticlePositions = m_baseEmitter.GetPositionVector();
	static const Vec2f diag = Vec2f( 1.f, 1.f ).Normalize();
	static const Vec2f nond = Vec2f( 0.f, 1.f );
	
	for( const auto &position : deadParticlePositions )
	{
		const auto minTTL = .1f;
		const auto maxTTL = .3f;
		const auto minRad = 10.f;
		const auto maxRad = 15.f;
		const Color c = m_colorPalette[ m_colorDist( m_rng ) ];

		AddBurstEmitter( position, {  nond.x,  nond.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );		
		AddBurstEmitter( position, {  diag.x,  diag.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, {  nond.y,  nond.x }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, {  diag.x, -diag.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, {  nond.x, -nond.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, { -diag.x, -diag.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, { -nond.y,  nond.x }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
		AddBurstEmitter( position, { -diag.x,  diag.y }, 1, 0.f, 0.f, false, minRad, maxRad, minTTL, maxTTL, c );
	}
	m_baseEmitter.ClearPositionVector();

	for( auto &emitter : m_burstEmitters )
	{
		emitter.Update( DeltaTime, viewport );
	}

	auto remIt = std::remove_if( m_burstEmitters.begin(), m_burstEmitters.end(), []( const Emitter &Emitter )
	{
		return Emitter.IsAllDead();
	} );

	m_burstEmitters.erase( remIt, m_burstEmitters.end() );
}

void Scene_Particles::Draw() const
{
	const auto viewport = 
		MakeRectFromCenter( m_camera.GetPosition(), m_screenrect.GetSize() );
	m_baseEmitter.Draw( viewport, m_graphics );

	for( const auto &emitter : m_burstEmitters )
	{
		emitter.Draw( viewport, m_graphics );
	}	
}

void Scene_Particles::AddBurstEmitter( 
	const Vec2f & Position, 
	const Vec2 & Direction, 
	size_t MaxParticles,
	float MinEmitterDelay,
	float MaxEmitterDelay,
	bool Infinite, 
	float MinParticleRadius, 
	float MaxParticleRadius, 
	float MinParticleTimeToLive, 
	float MaxParticleTimeToLive,
	Color ParticleColor)
{
	Emitter::Description emDesc{};
	emDesc.m_position = Position;
	emDesc.m_direction = Direction;
	emDesc.m_maxParticles = MaxParticles;
	emDesc.m_minDelay = MinEmitterDelay;
	emDesc.m_maxDelay = MaxEmitterDelay;
	emDesc.m_infinite = Infinite;

	Particle::Description ptDesc{};
	ptDesc.m_minParticleRadius = MinParticleRadius;
	ptDesc.m_maxParticleRadius = MaxParticleRadius;
	ptDesc.m_minParticleTimeToLive = MinParticleTimeToLive;
	ptDesc.m_maxParticleTimeToLive = MaxParticleTimeToLive;
	ptDesc.m_particleColor = ParticleColor;

	m_burstEmitters.push_back( { ptDesc, emDesc } );
}
