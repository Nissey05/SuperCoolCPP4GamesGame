#include "Utils.hpp"
#include <iostream>

namespace Utils {
	//Checks for collison, calculates overlap and returns correction
	glm::vec2 GetCollisionCorrection(const Math::AABB& aabb1, const Math::AABB& aabb2)
	{
		glm::vec2 correction { 0.f };
		// Checks if there is a intersection between aabb1 and aabb2
		if (aabb1.intersect(aabb2)) {
			// Calculates the overlap between the aabbs on both the X-axis and the Y-axis 
			float overlapX = glm::min(aabb1.max.x, aabb2.max.x) - glm::max(aabb1.min.x, aabb2.min.x);
			float overlapY = glm::min(aabb1.max.y, aabb2.max.y) - glm::max(aabb1.min.y, aabb2.min.y);

			if (overlapX < overlapY) {
				if (aabb1.min.x < aabb2.min.x)
					correction.x = -overlapX;
				else
					correction.x = overlapX;
			}
			else {
				if (aabb1.min.y < aabb2.min.y)
					correction.y = -overlapY;
				else
					correction.y = overlapY;
			}
		}

		return correction;
	}

}