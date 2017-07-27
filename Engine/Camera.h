#pragma once

#include "Vec2.h"
#include "Rect.h"

class Camera
{
public:
	Camera() = default;
	Camera( const Vec2f &StartPos );

	const Vec2f &GetPosition()const;
	void ClampTo( const Sizef &ViewSize, const Rectf &Boundary );
	virtual void Update( float DeltaTime ) = 0;
protected:
	Vec2f m_position;
};

class TrackingCamera : public Camera
{
public:
	TrackingCamera( const Vec2f &TrackMe );

	void Update( float DeltaTime )override;
private:
	const Vec2f &m_tracking_position;
};