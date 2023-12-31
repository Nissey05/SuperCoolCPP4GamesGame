#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Font.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/Window.hpp>
#include <Level.hpp>

#include <iostream>
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

Player::Player(const glm::vec2& pos)
	: Entity("player", "player", pos, {{0, 0, 0}, {32, 32, 0}})
{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Pinky/Pinky.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet("assets/Pinky/pinkyRun.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	auto fallSprites = ResourceManager::loadSpriteSheet("assets/Pinky/pinkyFalling.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	fallAnim = SpriteAnim{ fallSprites, 6 };
	setState(State::Falling);
	auto jumpSprites = ResourceManager::loadSpriteSheet("assets/Pinky/pinkyJumping.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	jumpAnim = SpriteAnim{ jumpSprites, 6 };
	healthPoints = 1;
	transform.setAnchor({ 16, 32 });
}

void Player::init(std::shared_ptr<Level> level)
{
	this->level = level;
}

void Player::update(float deltaTime) {
	if (healthPoints <= 0) setState(State::Dead);
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
	case State::Dead:
		doDead(deltaTime);
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
		image.drawSprite(jumpAnim, camera * transform);
		break;
	case State::Falling:
		image.drawSprite(fallAnim, camera * transform);
		break;
	case State::Dead:
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
		case State::Dead:
			break;
		}

		state = newState;
	}
}

void Player::doMovement(float deltaTime) {
	auto initialPos = transform.getPosition();
	auto newPos = initialPos;
	
	auto horizontalMove = Input::getAxis("Horizontal");
	float accelMultiCopy = accelerationMultiplier;

	float scaledChangeDamp = pow(0.3f, deltaTime * referenceFPS);
	if (horizontalMove > 0 && velocity.x < 0) accelMultiCopy = 40, velocity.x *= scaledChangeDamp;
	if (horizontalMove < 0 && velocity.x > 0) accelMultiCopy = 40, velocity.x *= scaledChangeDamp;

	if (glm::abs(horizontalMove) > 0.f) {
		const bool isSprinting = Input::getButton("Sprint");
		maxHorizontal = isSprinting ? 600.f : 200.f;
		accelMultiCopy *= isSprinting ? 2.0f : 1.0f;
		acceleration.x = (horizontalMove * speed * accelMultiCopy);
	}
	else {
		if (state == State::Running) {
			float scaledAccelDamping = pow(referenceAccelDamping, deltaTime * referenceFPS);
			float scaledVelocDamping = pow(referenceVelocDamping, deltaTime * referenceFPS);
			velocity.x *= scaledVelocDamping;
			acceleration *= scaledAccelDamping;

			if (glm::abs(velocity.x) < 20.f) velocity.x = 0, acceleration.x = 0;
		}
	}

	Gravity(deltaTime);

	//acceleration.x = glm::clamp(acceleration.x, -120.f, 120.f);
	acceleration.y = glm::clamp(acceleration.y, -80.f, 80.f);

	velocity += acceleration * deltaTime;

	velocity.x = glm::clamp(velocity.x, -maxHorizontal, maxHorizontal);
	velocity.y = glm::clamp(velocity.y, jumpSpeed, -jumpSpeed);
	
	newPos += velocity * deltaTime;

	setPosition(newPos);

	level->resolveCollisionForLevel(this);

	level->resolveEntityCollision(this);

	CheckBounds();

	deltaPos = getPosition() - initialPos;
}

void Player::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(velocity.x) > 0) {
		setState(State::Running);
	}
	if (velocity.y > 0) {
		setState(State::Falling);
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
	if (deltaPos.y > 0.f) setState(State::Falling);

	if (Input::getButton("Jump")) {
		velocity.y = jumpSpeed;
		setState(State::Jumping);
		
	}

	if (deltaPos.x > 0) transform.setScale({ 1, 1 });
	else if (deltaPos.x < 0) transform.setScale({ -1, 1 });

	runAnim.update(deltaTime);
}

void Player::doJump(float deltaTime) {
	doMovement(deltaTime);
	if (velocity.y > 0.0f) setState(State::Falling);
	jumpAnim.update(deltaTime);
}

void Player::doFalling(float deltaTime) {
	doMovement(deltaTime);
	if (velocity.y < 1.0f) setState(State::Idle);
	fallAnim.update(deltaTime);
}

void Player::doDead(float deltaTime){
	resetSpeed();
	if (lives > 0) {
		switch (level->getState()) {
		case LevelState::Level1:
			level->initLevelOne();
			break;
		case LevelState::Level2:
			level->initLevelTwo();
			break;
		case LevelState::Level3:
			level->initLevelThree();
			break;
		}
		setState(State::Idle);
		healthPoints = 1;
		lives--;
	}
	else if (lives <= 0) {
		level->setState(LevelState::Dead);
	}
}

void Player::Gravity(float deltaTime) {
	velocity.y += gravity * deltaTime;
}

void Player::CheckBounds() {
	auto aabb = getAABB();
	glm::vec2 correction{ 0.f };

	if (aabb.min.x < 0.f) {
		correction.x = -aabb.min.x;
	}
	else if (aabb.max.x >= level->getLevelMap().getWidth()) {
		correction.x = level->getLevelMap().getWidth() - aabb.max.x;
	}

	if (aabb.min.y < 0.f) {
		correction.y = -aabb.min.y;
	}
	else if (aabb.max.y >= level->getLevelMap().getHeight()) {
		correction.y = level->getLevelMap().getHeight() - aabb.max.y;
	}

	translate(correction);
	
	if (glm::abs(correction.y) > 0.f) {
		velocity.y = 0.f;
		acceleration.y = 0.f;

	}
	if (glm::abs(correction.x) > 0.f) {
		velocity.x = 0.f; 
		acceleration.x = 0.f;
	}
}

float Player::getJumpSpeed() {
	return jumpSpeed;
}

void Player::coinCheck() {
	if (coins >= 100) {
		coins -= 100;
		lives++;
	}
}