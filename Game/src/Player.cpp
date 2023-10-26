#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Font.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/Window.hpp>


#include <map>
#include <string>
	
using namespace Graphics;

static std::map < Player::State, std::string> g_stateMap = {
	{Player::State::None, "None" },
	{Player::State::Idle, "Idle" },
	{Player::State::Running, "Running" },
	{Player::State::Attack, "Attack" },
	{Player::State::Dead, "Dead" },
	{Player::State::Jumping, "Jumping" },
	{Player::State::Falling, "Falling" },

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
	case State::Jumping:
		doJump(deltaTime);
		break;
	case State::Falling:
		doFalling(deltaTime);
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
	case State::Jumping:
		image.drawSprite(idleAnim, camera * transform);
		break;
	case State::Falling:
		image.drawSprite(idleAnim, camera * transform);
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
		case State::Jumping:
			break;
		case State::Falling:
			break;
		}
		
		state = newState;
	}
}

void Player::doMovement(float deltaTime) {
	auto initialPos = transform.getPosition();
	auto newPos = initialPos;
	
	auto horizontalMove = Input::getAxis("Horizontal");
	if (glm::abs(horizontalMove) > 0.f) {
		if (Input::getButton("Sprint")) {
			velocity.x += horizontalMove * speed * 2.0f * deltaTime;
		}
		else {
			velocity.x += horizontalMove * speed * deltaTime;
		}
	}
	else {
		velocity.x *= 0.8f;
		if (velocity.x < 20.f) velocity.x = 0;
	}

	Gravity(deltaTime);

	velocity.x = glm::clamp(velocity.x, -200.f, 200.f);
	velocity.y = glm::clamp(velocity.y, jumpSpeed, -jumpSpeed);
	
	newPos += velocity * deltaTime;

	CheckBounds(newPos);

	deltaPos = newPos - initialPos;

	transform.setPosition(newPos);
}

void Player::doMove(float deltaTime) {
	
}


void Player::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(velocity.x) > 0) {
		setState(State::Running);
	}

	if (Input::getButton("Jump")) {
		velocity.y = jumpSpeed;
		setState(State::Jumping);
	}

	idleAnim.update(deltaTime);
}

void Player::doRunning(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(velocity.x) == 0.0f) {
		setState(State::Idle);
	}

	if (Input::getButton("Jump")) {
		velocity.y = jumpSpeed;
		setState(State::Jumping);
	}

	runAnim.update(deltaTime);
}

void Player::doJump(float deltaTime) {
	doMovement(deltaTime);
	if (velocity.y > 0.0f) setState(State::Falling);
	idleAnim.update(deltaTime);
}

void Player::doFalling(float deltaTime) {
	doMovement(deltaTime);
	if (deltaPos.y < 1.0f) setState(State::Idle);
	idleAnim.update(deltaTime);
}

void Player::Gravity(float deltaTime) {
	velocity.y += gravity * deltaTime;
}

void Player::CheckBounds(glm::vec2& pos) {
	if (pos.y < 0) pos.y = 0, velocity.y = 0.f;
	else if (pos.x < 0) pos.x = 0, velocity.x = 0.f;
	else if (pos.y > Backside->getHeight() - 32) pos.y = Backside->getHeight() - 32, velocity.y = 0.f;
	else if (pos.x > Backside->getWidth() - 32) pos.x = Backside->getWidth() - 32, velocity.y = 0.f;

	/*auto aabb = getAABB();
	glm::vec2 correction{ 0 };
	if (aabb.min.x < 0) {
		correction.x = -aabb.min.x;
	}
	if (aabb.min.y < 0) {
		correction.y = -aabb.min.y;
	}
	if (aabb.max.x >= Backside->getWidth()) {
		correction.x = Backside->getWidth() - aabb.max.x;
	}
	if (aabb.max.y >= Backside->getHeight()) {
		correction.y = Backside->getHeight() - aabb.max.y;

	}

	translate(correction);*/
}
