#include "Hulkazoid.hpp"

#include<Background.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>

using namespace Graphics;

Hulkazoid::Hulkazoid(const glm::vec2& pos, Background* backside)
	: Enemy( pos, backside, {{0, 0, 0}, {32, 32, 0}},
	"assets/Pinky/HulkazoidIdle.png", 
	"assets/Pinky/HulkazoidRun.png", 
	"assets/Pinky/HulkazoidFall.png")
{
	
	

}