#include "Hulkazoid.hpp"

#include <Level.hpp>
#include <Player.hpp>
#include <Utils.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace std::string_literals;
using namespace Graphics;

Hulkazoid::Hulkazoid(const glm::vec2& pos, Level* level)
	: Enemy(
		"hulkazoid_enemy"s,
		pos,
		level,
		{{0, 0, 0}, {32, 32, 0}},
		"assets/Pinky/HulkazoidIdle.png", 
		"assets/Pinky/HulkazoidRun.png", 
		"assets/Pinky/HulkazoidFall.png"
	)
{

}
