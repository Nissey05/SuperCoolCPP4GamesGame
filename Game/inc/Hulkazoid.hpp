#pragma once

#include "Enemy.hpp"

class Hulkazoid : public Enemy {
public:
	Hulkazoid() = default;

	Hulkazoid(const glm::vec2& pos, Background* backside);

private:

	class Background* backside;
};