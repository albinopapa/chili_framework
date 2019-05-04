#pragma once

#include "Rect.h"
#include <optional>

struct PhysicsPropertiesIn
{
	Vec2f position = { 0.f, 0.f };
	Vec2f velocity = { 0.f, 0.f };
	Rectf boundingbox = { 0.f, 0.f, 0.f, 0.f };
	bool can_move = false;
};

struct PhysicsPropertiesOut
{
	Vec2f position, velocity;
};

std::optional<PhysicsPropertiesOut> HandleCollision( PhysicsPropertiesIn const& _left, PhysicsPropertiesIn const& _right )noexcept;