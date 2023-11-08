#include "Hulkazoid.hpp"

#include<Background.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Hulkazoid::Hulkazoid(const glm::vec2& pos, Background* backside)
	: Enemy{ {600, 600}, backside, {{0, 0, 0}, {32, 32, 0}} }
{

	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Pinky/HulkazoidIdle.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnimHulk = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet("assets/Pinky/HulkazoidRun.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	runAnimHulk = SpriteAnim{ runSprites, 6 };

}