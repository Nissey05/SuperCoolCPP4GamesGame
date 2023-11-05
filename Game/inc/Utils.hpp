#pragma once
#include "Math/AABB.hpp"

namespace Utils {

	glm::vec2 GetCollisionCorrection(const Math::AABB& aabb1, const Math::AABB& aabb2);

}