#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
	
using namespace Graphics;

Player::Player()
	: position{ 0 }
{}

Player::Player(const glm::vec2& pos, const SpriteAnim& sprite)
	: position{ pos }

{}

void Player::update(float deltaTime) {
	position.x += Input::getAxis("Horizontal") * 60 * deltaTime;
	position.y += Input::getAxis("Vertical") * 60 * deltaTime;
}

void Player::draw(Graphics::Image& image) {
	image.drawSprite(sprite, position.x, position.y);
}

void Player::setPosition(const glm::vec2& pos) {
	position = pos;
}

