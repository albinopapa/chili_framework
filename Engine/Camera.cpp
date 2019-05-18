#include "Camera.h"
#include "Graphics.h"
#include "MathOps.h"

Camera::Camera( Vec2f const& _origin, RectF const& _viewrect )noexcept
	:
	m_position( _origin ),
	m_viewrect( _viewrect )
{}

Vec2f const& Camera::GetPosition()const noexcept
{
	return m_position;
}

RectF const& Camera::GetRect() const noexcept
{
	return m_viewrect;
}

Vec2f Camera::WorldToScreen( Vec2f const & _point ) const noexcept
{
	return _point - m_position;
}

Vec2f Camera::ScreenToWorld( Vec2f const & _point ) const noexcept
{
	return m_position + _point;
}

void Camera::SetPosition(const Vec2f & _position)noexcept
{
	m_position = _position;
}

void Camera::ClampTo( const Rectf & Boundary )noexcept
{
	m_position = Vec2f(
		std::clamp( m_position.x, Boundary.left, Boundary.right ),
		std::clamp( m_position.y, Boundary.top, Boundary.bottom )
	);
}
