#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Font.hpp>
#include <Math/Camera2D.hpp>


#include <map>
#include <string>
	
using namespace Graphics;

static std::map < Player::State, std::string> g_stateMap = {
	{Player::State::None, "None" },
	{Player::State::Idle, "Idle" },
	{Player::State::Running, "Running" },
	{Player::State::Attack, "Attack" },
	{Player::State::Dead, "Dead" },

};

Player::Player() = default;

Player::Player(const glm::vec2& pos)
	: Entity{pos, { {8, 17, 0}, {23, 37, 0} } }
	

{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Idle.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Run.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	setState(State::Idle);
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


void Player::update(float deltaTime) {
	switch (state) {
	case State::Idle:
		doIdle(deltaTime);
		break;
	case State::Running:
		doRunning(deltaTime);
		break;
	}

}

void Player::draw(Graphics::Image& image, const glm::mat3& transform) {
	glm::mat3 t = {
		1, 0, 0,
		0, 1, 0,
		position.x, position.y, 1
	};

	t = transform * t;

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
	image.drawText(Font::Default, g_stateMap[state], position.x, position.y, Color::White);
#endif
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
	auto initialPos = position;

	if (Input::getButton("Sprint")) {
		position.x += Input::getAxis("Horizontal") * runspeed * deltaTime;
	}
	else {
		position.x += Input::getAxis("Horizontal") * speed * deltaTime;
	}

	velocity = (position - initialPos) / deltaTime;



	if (Input::getButton("Jump")) {
		position.y--;
		Jumping = true;
	}

	Gravity();
}


void Player::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::length(velocity) > 0) {
		setState(State::Idle);
	}

	idleAnim.update(deltaTime);
}

void Player::doRunning(float deltaTime) {
	doMovement(deltaTime);

	if (glm::length(velocity) == 0.0f) {
		setState(State::Running);
	}

	runAnim.update(deltaTime);
}

void Player::Gravity(bool coll) {
	if (coll == false && position.y < 600 - 44 && Jumping == false) {
		position.y++;
		Jumping = false;
	}
	else if (position.y > 600 - 44) {
		position.y = 600 - 44;

	}
}

