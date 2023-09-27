#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
	
using namespace Graphics;

Player::Player() = default;

Player::Player(const glm::vec2& pos, const SpriteAnim& _sprite)
	: position{ pos }
	, sprite{_sprite}
	, aabb{ {8, 17, 0}, {23, 37, 0} }

{}

void Player::update(float deltaTime) {
	//position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	//position.y -= Input::getAxis("Vertical") * speed * deltaTime;

	auto initialPos = position;

	if (Input::getButton("Sprint")) {
		position.x += Input::getAxis("Horizontal") * runspeed * deltaTime;		
	}
	else {
		position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	}

	velocity = (position - initialPos) / deltaTime;

	if (velocity.length() > 0) {
		setState(State::Running);
	}
	else {
		setState(State::Idle);
	}

	position.y -= Input::getAxis("Jump");

	sprite.update(deltaTime);
}

void Player::draw(Graphics::Image& image) {
	switch (state)
	{
	case State::Idle:
		image.drawSprite(sprite, position.x, position.y);
		break;
	case State::Running:
		image.drawSprite(sprite, position.x, position.y);
		break;
	}
	
#if _DEBUG
	image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
#endif
}

void Player::setPosition(const glm::vec2& pos) {
	position = pos;
}

const glm::vec2& Player::getPosition() const {
	return position;
}

void Player::translate(const glm::vec2& t) {
	position += t;
}

const Math::AABB Player::getAABB() const {
	return aabb + glm::vec3{ position, 0 };
}

void Player::setState(State newState) {
	if (newState != state) {
		switch (newState) {
		case State::Idle:
			break;
		case State::Running:
			break;
		}
		state = newState;
	}
}