#include "Projectile.h"



Projectile::Projectile() = default;

Projectile::Projectile( const Vec2f &StartPos,
						const Vec2f &StartVelocity,
						float Width, float Height,
						float TimeToLive,
						ParticleSetupDesc::DrawFunc Fn,
						Color C )
	:
	Particle( StartPos, StartVelocity, Width, Height, TimeToLive, Fn, C )
{
}

void Projectile::Draw( const Rectf & Viewport, Graphics & Gfx ) const
{
	const Rectf offset = GetRect().Translate( -Viewport.LeftTop() );
	Gfx.DrawRect( offset, m_color );
}
