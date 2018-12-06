#pragma once

#include "Vec2.h"
#include "Rect.h"

class Camera
{
public:
	Camera() = default;
	Camera( const Vec2f &StartPos );

	const Vec2f &GetPosition()const;
	void SetPosition(const Vec2f& _position);
	Vec2f Transform(const Vec2f& _point);
	void ClampTo( const Sizef &ViewSize, const Rectf &Boundary );

protected:
	Vec2f m_position;
};
