#include "Particle.h"
#include "ChiliMath.h"
#include "Scene.h"
#include "Visitor.h"
#include "Fireworks.h"

ParticleSetupDesc::ParticleSetupDesc(
	float Speed,
	float MinWidth, float MaxWidth,
	float MinHeight, float MaxHeight,
	float MinTimeToLive,
	float MaxTimeToLive, Color Clr )
	:
	speed( Speed ),
	minWidth( MinWidth ), maxWidth( MaxWidth ),
	minHeight( MinHeight ), maxHeight( MaxHeight ),
	minTimeToLive( MinTimeToLive ), maxTimeToLive( MaxTimeToLive ),
	color( Clr )
{
}

Particle::Particle(
	const Vec2f & StartPos,
	const Vec2f & StartVelocity,
	float Width, float Height,
	float TimeToLive,
	ParticleSetupDesc::DrawFunc Fn,
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
	if( Fn == ParticleSetupDesc::DrawFunc::CircleAlpha )
	{
		DrawFn = &Graphics::DrawCircleAlpha;
	}
	else if( Fn == ParticleSetupDesc::DrawFunc::RectAlpha )
	{
		DrawFn = &Graphics::DrawRectAlpha;
	}
	else if( Fn == ParticleSetupDesc::DrawFunc::Circle )
	{
		DrawFn = &Graphics::DrawCircle;
	}
	else
	{
		DrawFn = &Graphics::DrawRect;
	}
}

void Particle::Update( float DeltaTime )
{
	if( IsDead() )return;

	m_position += ( m_velocity * DeltaTime );
	m_liveCounter -= DeltaTime;
}

void Particle::Draw( const Rectf &Viewport, Graphics & Gfx ) const
{
	const auto offset = static_cast<Vec2i>( m_position - Viewport.LeftTop() );

	const float currentStep = Clamp( ( m_liveCounter / m_timeToLive ) * 2.f, 0.f, 1.f );
	const auto charStep = static_cast< unsigned char >( currentStep * 255.f );

	const Recti rect = static_cast< Recti >( GetRect() );
	(Gfx.*DrawFn)( rect, m_color * charStep );
}

Rectf Particle::GetRect() const
{
	return MakeRectFromCenter( m_position, Sizef{ m_width, m_height } );
}

bool Particle::IsDead()const
{
	return m_liveCounter <= 0.f;
}
