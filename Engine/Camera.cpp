#include "Camera.h"
#include "Graphics.h"

Camera::Camera( const Vec2f &StartPos )
	:
	m_position( StartPos )
{

}

const Vec2f & Camera::GetPosition() const
{
	return m_position;
}

void Camera::SetPosition(const Vec2f & _position)
{
	m_position = _position;
}

Vec2f Camera::Transform(const Vec2f & _point)
{
	return _point - m_position;
}

void Camera::ClampTo( const Sizef &ViewSize, const Rectf & Boundary )
{
	m_position = Vec2f(
		Clamp( m_position.x, Boundary.left + ViewSize.width * .5f, Boundary.right -   ViewSize.width * .5f ),
		Clamp( m_position.y, Boundary.top +  ViewSize.height * .5f, Boundary.bottom - ViewSize.height * .5f )
	);
}

void Camera::Update(float DeltaTime)
{

}
