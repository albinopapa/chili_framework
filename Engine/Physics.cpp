#include "MathOps.h"
#include "Physics.h"

#include <cassert>


std::optional<PhysicsPropertiesOut> HandleCollision( PhysicsPropertiesIn const& _left, PhysicsPropertiesIn const& _right )noexcept
{
	Vec2f velocity = _left.can_move ? _left.velocity : _right.velocity;
	Vec2f position = _left.can_move ? _left.position : _right.position;

	Vec2f penetration;
	bool handled = false;

	if(_left.boundingbox.Overlaps(_right.boundingbox))
	{
		if( velocity.y == 0.f )
		{
			// Moving horizontal
			if( velocity.x < 0.f )
			{
				// hero left, wall right
				position.x += ( _right.boundingbox.right - _left.boundingbox.left );
				velocity.x = 0.f;
				handled = true;
			}
			else if( velocity.x > 0.f )
			{
				// hero right, wall left
				position.x += ( _right.boundingbox.left - _left.boundingbox.right );
				velocity.x = 0.f;
				handled = true;
			}
		}
		else
		{
			if( velocity.x < 0.f )
			{
				if( velocity.y > 0.f )
				{
					// Heading left and down
					penetration.x = _right.boundingbox.right - _left.boundingbox.left;
					penetration.y = _left.boundingbox.bottom - _right.boundingbox.top;
					if( penetration.x * velocity.y > penetration.y * -velocity.x )
					{
						position.y -= penetration.y;
						velocity.y = 0.f;
						handled = true;
					}
					else
					{
						position.x += penetration.x;
						velocity.x = 0.f;
						handled = true;
					}
				}
				else
				{
					// Heading left and up
					penetration.x = _right.boundingbox.right - _left.boundingbox.left;
					penetration.y = _right.boundingbox.bottom - _left.boundingbox.top;

					if( penetration.x * -velocity.y > penetration.y * -velocity.x )
					{
						position.y += penetration.y;
						velocity.y = 0.f;
						handled = true;
					}
					else
					{
						position.x += penetration.x;
						velocity.x = 0.f;
						handled = true;
					}
				}
			}
			else if( velocity.x > 0.f )
			{
				if( velocity.y > 0.f )
				{
					// Heading right and down
					penetration = _left.boundingbox.RightBottom() - _right.boundingbox.LeftTop();
					if( penetration.x * velocity.y > penetration.y * velocity.x )
					{
						// hero bottom, wall top
						position.y -= penetration.y;
						velocity.y = 0.f;
						handled = true;
					}
					else
					{
						// hero right, wall left
						position.x -= penetration.x;
						velocity.x = 0.f;
						handled = true;
					}
				}
				else if( velocity.y < 0.f )
				{
					// Heading right and up
					penetration.x = _left.boundingbox.right - _right.boundingbox.left;
					penetration.y = _right.boundingbox.bottom - _left.boundingbox.top;
					if( penetration.x * -velocity.y > penetration.y * velocity.x )
					{
						// hero top, wall bottom
						position.y += penetration.y;
						velocity.y = 0.f;
						handled = true;
					}
					else
					{
						// hero right, wall left
						position.x -= penetration.x;
						velocity.x = 0.f;
						handled = true;
					}
				}
			}
			else
			{
				// Moving vertical
				if( velocity.y < 0.f )
				{
					// hero top, wall bottom
					position.y += ( _right.boundingbox.bottom - _left.boundingbox.top );
					velocity.y = 0.f;
					handled = true;
				}
				else
				{
					// hero bottom, wall top
					position.y += ( _right.boundingbox.top - _left.boundingbox.bottom );
					velocity.y = 0.f;
					handled = true;
				}
			}
		}
	}

	if( handled )
	{
		PhysicsPropertiesOut result;
		result.position = position;
		result.velocity = velocity;
		return result;
	}
	else
	{
		return std::nullopt;
	}
}