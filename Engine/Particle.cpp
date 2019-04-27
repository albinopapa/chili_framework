#include "Particle.h"
#include "ChiliMath.h"
#include "Scene.h"
#include "Visitor.h"

ParticleSetupDesc::ParticleSetupDesc( 
	float _speed, float _width, float _height, float _timeToLive, Color _color )
	:
	speed( _speed ),
	width( _width ),
	height( _height ),
	timeToLive( _timeToLive),
	color( _color )
{
}

Particle::Particle( Vec2 const& _origin, Vec2 const& _direction, ParticleSetupDesc const& _desc )noexcept
	:
	m_position( _origin ),
	m_velocity( _direction * _desc.speed ),
	m_timeToLive( _desc.timeToLive ),
	m_width( _desc.width ), 
	m_height( _desc.height ),
	m_liveCounter( _desc.timeToLive ),
	m_color( _desc.color )
{
}

auto Particle::GetRect()const noexcept
{
	return MakeRectFromCenter( m_position, Sizef{ m_width, m_height } );
}

auto Particle::GetColor()const noexcept
{
	return m_color;
}

auto Particle::IsDead()const noexcept
{
	return m_liveCounter <= 0.f;
}

void Particle::Update( float DeltaTime )noexcept
{
	if( IsDead() )return;

	m_position += ( m_velocity * DeltaTime );
	m_liveCounter -= DeltaTime;
}

void DrawTransparentCircleEffect::operator()( const Recti & _rect, const Particle & _particle, Graphics & _graphics )const noexcept
{
	_graphics.DrawCircleAlpha( _rect, _particle.GetColor() );
}

void DrawTransparentRectEffect::operator()( const Recti & _rect, const Particle & _particle, Graphics & _graphics )const noexcept
{
	_graphics.DrawRectAlpha( _rect, _particle.GetColor() );
}
