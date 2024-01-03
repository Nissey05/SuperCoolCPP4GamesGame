#pragma once

#include "Entity.hpp"
#include <Graphics/SpriteAnim.hpp>

class Coin : public Entity {
public:

	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override;
	explicit Coin(const glm::vec2& pos, class Level* level);

protected:
	Coin() = default;
	

private:
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim pickupAnim;

	class Level* level;
};
