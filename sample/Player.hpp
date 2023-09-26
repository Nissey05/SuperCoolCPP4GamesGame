#pragma once

#include <glm/vec2.hpp>
#include <Graphics/Sprite.hpp>

class Player {
public:
	//Default constructor
	Player();

	explicit Player(const glm::vec2& pos);

private:
	glm::vec2 position;
	Sprite sprite;
};