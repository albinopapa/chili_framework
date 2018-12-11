#pragma once

#include "AlphaSprite.h"
#include "Entity.h"

struct ParticleSetupDesc
{
	enum DrawFunc
	{
		Circle, CircleAlpha,
		RectAlpha
	};
public:
	ParticleSetupDesc() = default;
	ParticleSetupDesc(
		float _minSpeed,  float _maxSpeed, 
		float _minWidth,  float _maxWidth,
		float _minHeight, float _maxHeight,
		float _minTimeToLive, float _maxTimeToLive,
		Color _color );

public:
	float minSpeed = 16.f, maxSpeed = 32.f;
	float minWidth = 4.f, maxWidth = 16.f;
	float minHeight = 1.f, maxHeight = 15.f;
	float minTimeToLive = .1f, maxTimeToLive = 2.f;
	DrawFunc drawFunc = CircleAlpha;
	Color color = Colors::White;
};

struct DrawTransparentCircleEffect
{
	void operator()( const Recti& _rect, const Particle& _particle, Graphics& _graphics );
};
struct DrawTransparentRectEffect
{
	void operator()( const Recti& _rect, const Particle& _particle, Graphics& _graphics );
};
class Particle
{
public:
	Particle() = default;
	Particle(
		const Vec2f &StartPos,
		const Vec2f &StartVelocity,
		float Width, float Height,
		float TimeToLive,
		Color C );

	void Update( float DeltaTime );
	
	template<typename Effect>
	void Draw( const RectF& _viewport, Graphics& _graphics, Effect&& _effect )const
	{
		const auto offset = static_cast< Vec2i >( m_position - Viewport.LeftTop() );

		const float currentStep = Clamp( ( m_liveCounter / m_timeToLive ) * 2.f, 0.f, 1.f );
		const auto charStep = static_cast< unsigned char >( currentStep * 255.f );

		const Recti rect = static_cast< Recti >( GetRect() );
		_effect( rect, *this, _graphics );
	}

	void Position( const Vec2f &Pos ) { m_position = Pos; }
	Vec2f Position()const { return m_position; }

	void Velocity( const Vec2 &Vel ) { m_velocity = Vel; }
	Vec2f Velocity()const { return m_velocity; }

	Rectf GetRect()const;
	Color GetColor()const;
	bool IsDead()const;
private:
	Vec2f m_position, m_velocity;
	float m_timeToLive = 0.f, m_liveCounter = 0.f;
	float m_width, m_height;
	Color m_color;
};
