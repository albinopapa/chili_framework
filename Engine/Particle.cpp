#include "Particle.h"
#include "ChiliMath.h"
#include "Scene.h"
#include "Visitor.h"
#include "Fireworks.h"

ParticleSetupDesc::ParticleSetupDesc(
	float _minSpeed, float _maxSpeed,
	float _minWidth, float _maxWidth,
	float _minHeight, float _maxHeight,
	float _minTimeToLive, float _maxTimeToLive,
	Color _color )
	:
	minSpeed( _minSpeed ), maxSpeed( _maxSpeed ),
	minWidth( _minWidth ), maxWidth( _maxWidth ),
	minHeight( _minHeight ), maxHeight( _maxHeight ),
	minTimeToLive( _minTimeToLive ), maxTimeToLive( _maxTimeToLive ),
	color( _color )
{
}

Particle::Particle(
	const Vec2f & StartPos,
	const Vec2f & StartVelocity,
	float Width, float Height,
	float TimeToLive,
	Color C )
	:
	m_position( StartPos ),
	m_velocity( StartVelocity ),
	m_timeToLive( TimeToLive ),
	m_width( Width ), 
	m_height( Height ),
	m_liveCounter( TimeToLive ),
	m_color( C )
{
}

void Particle::Update( float DeltaTime )
{
	if( IsDead() )return;

	m_position += ( m_velocity * DeltaTime );
	m_liveCounter -= DeltaTime;
}

Rectf Particle::GetRect() const
{
	return MakeRectFromCenter( m_position, Sizef{ m_width, m_height } );
}

Color Particle::GetColor() const
{
	return m_color;
}

bool Particle::IsDead()const
{
	return m_liveCounter <= 0.f;
}

void DrawTransparentCircleEffect::operator()( const Recti & _rect, const Particle & _particle, Graphics & _graphics )
{
	_graphics.DrawCircleAlpha( _rect, _particle.GetColor() );
}

void DrawTransparentRectEffect::operator()( const Recti & _rect, const Particle & _particle, Graphics & _graphics )
{
	_graphics.DrawRectAlpha( _rect, _particle.GetColor() );
}
