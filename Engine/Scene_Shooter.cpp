#include "Scene_Shooter.h"



Scene_Shooter::Scene_Shooter( Keyboard &Kbd, Graphics &Gfx )
	:
	Scene( Kbd, Gfx )
{
}

void Scene_Shooter::Update( float DeltaTime )
{
	DoHeroInput();
	m_hero.Update( DeltaTime );
	m_camera.SetTarget( m_hero.Position() );
	m_camera.Update( DeltaTime );
	RemoveDeadProjectiles();
	CollectNewProjectiles();
	UpdateProjectiles( DeltaTime );		
}

void Scene_Shooter::Draw() const
{
	const auto viewport = MakeRectFromCenter( m_camera.GetPosition(), Graphics::fScreenSize );
	for( auto &pParticle : m_projectiles )
	{
		pParticle->Draw( viewport, m_graphics );
	}
	m_hero.Draw( viewport, m_graphics );
}

void Scene_Shooter::DoHeroInput()
{
	Vec2f direction = { 0.f, 0.f };
	int key = 0;
	constexpr int mask = ( 1 << 1 ) | ( 1 );
	if( m_keyboard.KeyIsPressed( VK_UP ) )
	{
		direction.y = -1.f;
	}
	else if( m_keyboard.KeyIsPressed( VK_DOWN ) )
	{
		direction.y = 1.f;
		key <<= 1;
	}

	if( m_keyboard.KeyIsPressed( VK_LEFT ) )
	{
		direction.x = -1.f;
		key |= 1;
	}
	else if( m_keyboard.KeyIsPressed( VK_RIGHT ) )
	{
		direction.x = 1.f;
	}

	if( direction.LenSq() != 0.f )
	{
		direction.Normalize();
		m_hero.ChangeOrientation( direction );
	}
	m_hero.ChangeVelocity( direction );

	if( m_keyboard.KeyIsPressed( VK_SPACE ) )
	{
		if( m_keyboard.KeyIsPressed( VK_LEFT ) && m_keyboard.KeyIsPressed( VK_DOWN ) )
		{
			int a = 0;
		}
		if( m_hero.Fire() )
		{
			const auto herovelocity = m_hero.Velocity();
			const auto herodirection = herovelocity.Normalize();
			const auto herospeed = direction * herovelocity;

			ParticleSetupDesc desc{};
			desc.maxWidth = desc.maxHeight = 5;
			desc.speed = herospeed + Projectile::m_speed;
			
			m_ammoEmitter.SetPosition( m_hero.Position() );
			m_ammoEmitter.SetDirection( m_hero.Orientation() );
			m_ammoEmitter.SpawnParticles<Projectile>( desc );
		}
	}
}

void Scene_Shooter::RemoveDeadProjectiles()
{
	auto endIt = std::remove_if( 
		m_projectiles.begin(), m_projectiles.end(),
		[]( const std::unique_ptr<Particle> &pParticle )
	{
		return pParticle->IsDead();
	} );

	m_projectiles.erase( endIt, m_projectiles.end() );
}

void Scene_Shooter::CollectNewProjectiles()
{
	for( auto &pParticle : m_ammoEmitter.TakeParticles() )
	{
		m_projectiles.emplace_back( std::move( pParticle ) );
	}
}

void Scene_Shooter::UpdateProjectiles( float DeltaTime )
{
	for( auto &pParticle : m_projectiles )
	{
		pParticle->Update( DeltaTime );
	}
}
