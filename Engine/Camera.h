#pragma once

#include "Vec2.h"
#include "Rect.h"

class Camera
{
public:
	Camera() = default;
	Camera( const Vec2f &StartPos );
	virtual~Camera() = default;

	const Vec2f &GetPosition()const;
	void ClampTo( const Sizef &ViewSize, const Rectf &Boundary );
	virtual void Update( float DeltaTime ) = 0;

protected:
	Vec2f m_position;
};

class StaticCamera :public Camera
{
public:
	using Camera::Camera;
	void Update( float DeltaTime ) {}
};

class TrackingCamera : public Camera
{
public:
	using Camera::Camera;
	void Update( float DeltaTime )override;
	void SetTarget( const Vec2f &TargetPosition );
	
private:
	Vec2f m_target;
};
