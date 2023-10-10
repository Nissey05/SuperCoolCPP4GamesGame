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

Player::Player(const glm::vec2& pos, Sprite* Backside)
	: Entity{pos, { {0, 0, 0}, {32, 32, 0} } }, 
	Backside(Backside)
	

{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Pinky/Pinky.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
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

void Player::draw(Graphics::Image& image, const Math::Camera2D& camera) {
	

	switch (state)
	{
	case State::Idle:
		image.drawSprite(idleAnim, camera * transform);
		break;
	case State::Running:
		image.drawSprite(runAnim, camera * transform);
		break;
	}
	
#if _DEBUG
	image.drawAABB(camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
	auto pos = camera * transform;
	image.drawText(Font::Default, g_stateMap[state], pos[2][0], pos[2][1] - 15, Color::White);
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
	auto initialPos = transform.getPosition();
	auto newPos = initialPos;

	if (Input::getButton("Sprint")) {
		newPos.x += Input::getAxis("Horizontal") * runspeed * deltaTime;
		newPos.y -= Input::getAxis("Vertical") * runspeed * deltaTime;
	}
	else {
		newPos.x += Input::getAxis("Horizontal") * speed * deltaTime;
		newPos.y -= Input::getAxis("Vertical") * speed * deltaTime;
	}

	velocity = (newPos - initialPos) / deltaTime;

	

	if (Input::getButton("Jump")) {
		newPos.y--;
		Jumping = true;
	}
	else{
		Jumping = false;
	}

	Gravity(newPos);


	transform.setPosition(newPos);
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

void Player::Gravity(glm::vec2& newPos, bool coll) {
	if (coll == false && newPos.y < Backside->getHeight() - 32 && Jumping == false) {
		newPos.y++;
		
	}
	else if (newPos.y > Backside->getHeight() - 32) {
		newPos.y = Backside->getHeight() - 32;
		
	}
}

