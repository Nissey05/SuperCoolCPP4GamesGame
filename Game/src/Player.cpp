#include <Player.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Font.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/Window.hpp>

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

Player::Player() = default;

Player::Player(const glm::vec2& pos, Sprite* Backside)
	: Entity{pos, { {0, 0, 0}, {32, 32, 0} } }, 
	Backside(Backside)
	

{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Pinky/Pinky.png", 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Run.png", 137, 44, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	setState(State::Falling);

	transform.setAnchor({ 16, 32 });
}

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
	float accelMultiCopy = accelerationMultiplier;

	if (horizontalMove > 0 && acceleration.x < 0) accelMultiCopy *= 60.f, velocity.x *= 0.3f;
	if (horizontalMove < 0 && acceleration.x > 0) accelMultiCopy *= 60.f, velocity.x *= 0.3f;

	if (glm::abs(horizontalMove) > 0.f) {
		const bool isSprinting = Input::getButton("Sprint");
		maxHorizontal = isSprinting ? 600.f : 200.f;
		accelMultiCopy *= isSprinting ? 2.0f : 1.0f;
		acceleration.x += (horizontalMove * speed * accelMultiCopy * deltaTime);
	}
	else {
		float scaledAccelDamping = pow(referenceAccelDamping, deltaTime * referenceFPS);
		float scaledVelocDamping = pow(referenceVelocDamping, deltaTime * referenceFPS);
		velocity.x *= scaledVelocDamping;
		acceleration *= scaledAccelDamping;
		
		if (glm::abs(velocity.x) < 20.f) velocity.x = 0, acceleration.x = 0;
	}

	

	Gravity(deltaTime);


	acceleration.x = glm::clamp(acceleration.x, -120.f, 120.f);
	acceleration.y = glm::clamp(acceleration.y, -80.f, 80.f);
	

	velocity += acceleration * deltaTime;

	velocity.x = glm::clamp(velocity.x, -maxHorizontal, maxHorizontal);
	velocity.y = glm::clamp(velocity.y, jumpSpeed, -jumpSpeed);
	
	newPos += velocity * deltaTime;

	
	if(velocity.y >= 0){
		CheckBounds(newPos);
	}
	

	
	

	deltaPos = newPos - initialPos;

	setPosition(newPos);
}

void Player::doMove(float deltaTime) {
	
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
	auto aabb = getAABB();
	glm::vec2 correction{ 0 };
	if (aabb.min.x < 0) {
		correction.x = -aabb.min.x;
	}
	if (aabb.min.y < 0) {
		correction.y = -aabb.min.y;
	}
	if (aabb.max.x >= Backside->getWidth()-5) {
		correction.x = Backside->getWidth()-5 - aabb.max.x;
	}
	if (aabb.max.y >= Backside->getHeight()-5) {
		correction.y = Backside->getHeight()-5 - aabb.max.y;
	}

	pos += correction;

	if (glm::abs(correction.y) > 0) {
		velocity.y = 0;
		acceleration.y = 0;
	}
	if (glm::abs(correction.x) > 0) {
		velocity.x = 0; 
		acceleration.x = 0;
	}

	
}
