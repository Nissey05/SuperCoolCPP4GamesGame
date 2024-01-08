#pragma once

#include "Enemy.hpp"

class Vorz : public Enemy {
public:
	Vorz() = default;
	Vorz(const glm::vec2& pos, Level* level, const std::string& type);
	
private:
	
};