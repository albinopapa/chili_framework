#pragma once

#include "Vec2.h"
#include "Rect.h"

class Camera
{
public:
	Camera() = default;
	Camera( Vec2f const& _origin, RectF const& _viewrect )noexcept;

	Vec2f const& GetPosition()const noexcept;
	RectF const& GetRect()const noexcept;
	Vec2f WorldToScreen( Vec2f const& _point )const noexcept;
	Vec2f ScreenToWorld( Vec2f const& _point )const noexcept;

	void SetPosition(const Vec2f& _position)noexcept;
	void ClampTo( const Rectf &Boundary )noexcept;


protected:
	Vec2f m_position;
	RectF m_viewrect;
};
