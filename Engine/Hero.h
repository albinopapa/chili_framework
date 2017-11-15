#pragma once

#include "Entity.h"

class Hero :public Entity
{
public:
	Hero();

	void Update( float DeltaTime )override;
	void Draw( const Rectf &Viewport, Graphics &Gfx )const override;

	void ChangeVelocity( const Vec2f &Direction );
	void ChangeOrientation( const Vec2f &Direction );
	Vec2f Orientation()const;
	bool Fire();

private:
	static constexpr float m_speed = 100.f;

	static constexpr float m_fireDelay = .25f;

	static constexpr float m_scale = 2.f;
	static constexpr float m_leftShoulderOffset = -5.f * m_scale;
	static constexpr float m_rightShoulderOffset = 5.f * m_scale;
	static constexpr float m_torsoThickness = 10.f * m_scale;
	static constexpr float m_headRadius = 6.f * m_scale;

	float m_fireDelayCounter = 0.f;
	float angle = 0.f;
	Vec2f m_orientation;
};

