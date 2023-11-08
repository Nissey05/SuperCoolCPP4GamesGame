#pragma once

#include "Enemy.hpp"

class Hulkazoid : public Enemy {
public:
	Hulkazoid() = default;

	Hulkazoid(const glm::vec2& pos, Background* backside);

private:

	SpriteAnim idleAnimHulk;
	SpriteAnim runAnimHulk;

	class Background* backside;
};