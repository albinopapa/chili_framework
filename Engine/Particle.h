#pragma once

#include "Colors.h"
#include "Graphics.h"
#include "Rect.h"
#include "Vec2.h"

struct ParticleSetupDesc
{
public:
	ParticleSetupDesc() = default;
	ParticleSetupDesc(
		float _speed,  
		float _width,  
		float _height, 
		float _timeToLive, 
		Color _color );

public:
	float speed = 16.f;
	float width = 4.f;
	float height = 1.f;
	float timeToLive = .1f;
	Color color = Colors::White;
};

class Particle;
struct DrawTransparentCircleEffect
{
	void operator()( const Recti& _rect, const Particle& _particle, Graphics& _graphics )const noexcept;
};
struct DrawTransparentRectEffect
{
	void operator()( const Recti& _rect, const Particle& _particle, Graphics& _graphics )const noexcept;
};
class Particle
{
public:
	Particle() = default;
	Particle( Vec2 const& _origin, Vec2 const& _direction, ParticleSetupDesc const& _desc )noexcept;

	void Update( float DeltaTime )noexcept;
	
	template<typename Effect>
	void Draw( const RectF& _viewport, Graphics& _graphics, Effect&& _effect )const noexcept
	{
		const Recti rect = static_cast< Recti >( GetRect() );
		_effect( rect, *this, _graphics );
	}

	void Position( const Vec2f &Pos )noexcept { m_position = Pos; }
	void Velocity( const Vec2 &Vel )noexcept { m_velocity = Vel; }

	auto Position()const noexcept { return m_position; }
	auto Velocity()const noexcept { return m_velocity; }
	auto GetRect()const noexcept;
	auto GetColor()const noexcept;
	auto IsDead()const noexcept;
private:
	Vec2f m_position, m_velocity;
	float m_timeToLive = 0.f, m_liveCounter = 0.f;
	float m_width = 0.f, m_height = 0.f;
	Color m_color = Colors::White;
};
