#pragma once

// Demo and framework headers
#include "GlobalEnums.h"
#include "RandomNumber.h"
#include "Vec2.h"

CellGrid GenerateMaze( Vec2i startPos, Vec2i endPos, Random random );