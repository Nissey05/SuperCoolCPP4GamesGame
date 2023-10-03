#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Font.hpp>


#include <map>
#include <string>
	
using namespace Graphics;

static std::map < Player::State, std::<string> g_stateMap = {
	{Player::State::None, "None" },
	{Player::State::Idle, "Idle" },
	{Player::State::Running, "Running" },
	{Player::State::Attack, "Attack" },
	{Player::State::Dead, "Dead" },

};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	: position{ pos }
	, aabb{ {8, 17, 0}, {23, 37, 0} }

{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Idle.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Run.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	setState(State::Idle)
}

//void Player::update(float deltaTime) {
//	//position.x += Input::getAxis("Horizontal") * speed * deltaTime;
//	//position.y -= Input::getAxis("Vertical") * speed * deltaTime;
//
//	auto initialPos = position;
//
//	if (Input::getButton("Sprint")) {
//		position.x += Input::getAxis("Horizontal") * runspeed * deltaTime;		
//	}
//	else {
//		position.x += Input::getAxis("Horizontal") * speed * deltaTime;
//	}
//
//	velocity = (position - initialPos) / deltaTime;
//
//	if (velocity.length() > 0) {
//		setState(State::Running);
//	}
//	else {
//		setState(State::Idle);
//	}
//
//	if (Input::getButton("Jump")) {
//		position.y--;
//	}
//	else {
//		if (position.y < 600 - 37) position.y += 180.0f * deltaTime;
//		else if (position.y >= 600 - 37) position.y = 563;
//	}
//
//	position.y -= Input::getAxis("Jump");
//
//	
//
//
//
//	sprite.update(deltaTime);
//}

void Player::draw(Graphics::Image& image) {
glm::mat3 t = {
	1, 0, 0
	0, 1, 0
	position.x, position.y, 1
};
	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleAnim, position.x, position.y);
		break;
	case State::Running:
		image.drawSprite(runAnim, position.x, position.y);
		break;
	}
	
#if _DEBUG
	image.drawAABB(getAABB(), Color::Yellow, {}, FillMode::WireFrame);
	image.drawText(Font::Default, g_stateMap[state], position.x, position.y, Color::White)
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

void Player::doMovement(float deltaTime) {

}


void Player::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::length(velocity) = 0) {
		setState(State::Idle);
	}

	idleAnim.update(deltaTime);
}

void Player::doRunning(float deltaTime) {
	doMovement(deltaTime);

	if (glm::length(velocity) > 0) {
		setState(State::Running)
	}

	runAnim.update(deltaTime);
}