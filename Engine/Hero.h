#pragma once

#include "Entity.h"

class Hero :public Entity
{
public:
	Hero();

	void Update( float DeltaTime )override;
	void Draw( const Rectf &Viewport, Graphics &Gfx )const override;

	void ChangeDirection( const Vec2f &Direction );
	bool Fire();

private:
	static constexpr float m_speed = 100.f;

	static constexpr float m_fireDelay = .5f;

	static constexpr float m_leftShoulderOffset = -5.f;
	static constexpr float m_rightShoulderOffset = 5.f;
	static constexpr float m_torsoThickness = 5.f;
	static constexpr float m_headRadius = 6.f;

	float m_fireDelayCounter = 0.f;
};

