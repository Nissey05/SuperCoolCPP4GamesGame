#include <Enemy.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Level.hpp>
#include <Utils.hpp>
#include <map>

#include <iostream>

using namespace Graphics;

static std::map < Enemy::EnemyState, std::string> g_stateMap = {
	{Enemy::EnemyState::None, "None" },
	{Enemy::EnemyState::Idle, "Idle" },
	{Enemy::EnemyState::Running, "Running" },
	{Enemy::EnemyState::Dead, "Dead" },
	{Enemy::EnemyState::Falling, "Falling" },
	{Enemy::EnemyState::Return, "Return" },

};


//Constructor for Enemy taking name (string), pos (vec2), level (Level), aabb (AABB), idlePath (string), runPath (string) and fallPath (string).
Enemy::Enemy(const std::string& name, const glm::vec2& pos, Level* level, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath) :
	Entity(name, "enemy", pos, {{0, 0, 0}, {32, 32, 0}}),
	level(level)
{
	auto idleSprites = ResourceManager::loadSpriteSheet(idlePath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet(runPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	auto fallSprites = ResourceManager::loadSpriteSheet(fallPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	fallAnim = SpriteAnim{ fallSprites, 6 };
	startPos = pos;
	setState(EnemyState::Idle);
	transform.setScale({ -1, 1 });
	transform.setAnchor({ 16, 32 });
}

//Constructor for Enemy taking name (string), pos (vec2), level (Level), aabb (AABB), idlePath (string), runPath (string), fallPath (string) and type (string).
Enemy::Enemy(const std::string& name, const glm::vec2& pos, Level* level, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath, const std::string& type) :
	Entity(name, "enemy", pos, { {0, 0, 0}, {32, 32, 0} }),
	level(level),
	enemyType(type)
{
	auto idleSprites = ResourceManager::loadSpriteSheet(idlePath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet(runPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	auto fallSprites = ResourceManager::loadSpriteSheet(fallPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	fallAnim = SpriteAnim{ fallSprites, 6 };
	startPos = pos;
	setState(EnemyState::Idle);
	transform.setScale({ -1, 1 });
	transform.setAnchor({ 16, 32 });
}

//Calls Functions every frame based on state
void Enemy::update(float deltaTime) {
	switch (state) {
	case EnemyState::Idle:
		doIdle(deltaTime);
		break;
	case EnemyState::Running:
		doRunning(deltaTime);
		break;
	case EnemyState::Falling:
		doFalling(deltaTime);
		break;
	case EnemyState::Return:
		doReturn(deltaTime);
		break;
	}
}

//Draws sprite animation based on state
void Enemy::draw(Graphics::Image& image, const Math::Camera2D& camera){
	switch (state)
	{
	case EnemyState::Idle:
		image.drawSprite(idleAnim, camera * transform);
		break;
	case EnemyState::Running:
		image.drawSprite(runAnim, camera * transform);
		break;
	case EnemyState::Falling:
		image.drawSprite(fallAnim, camera * transform);
		break;
	case EnemyState::Return:
		image.drawSprite(runAnim, camera * transform);
		break;
	}
//If Debug mode is on, draws Enemy AABB and state
#if _DEBUG
	image.drawAABB(camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
	auto pos = camera * transform;
	image.drawText(Font::Default, g_stateMap[state], pos[2][0], pos[2][1] - 15, Color::White);
#endif
}

void Enemy::setState(EnemyState newState) {
	if (newState != state) {
		switch (newState) {
		case EnemyState::Idle:
			break;
		case EnemyState::Running:
			break;
		case EnemyState::Falling:
			break;
		}

		state = newState;
	}
}

//Handles movement, CheckBounds and collisions for Enemies through acceleration, velocity, transform2D and an AABB
void Enemy::doMovement(float deltaTime){
	auto initialPos = getPosition();
	auto newPos = initialPos;
	
	if (name == "hulkazoid_enemy") {
		Gravity(deltaTime);
	}

	velocity += acceleration * deltaTime;

	newPos += velocity * deltaTime;

	setPosition(newPos);

	CheckBounds();

	level->resolveCollisionForLevel(this);

	deltaPos = getPosition() - initialPos;
}

//Function called every frame while Enemy is in Idle state
//Checks if enemy is moving to swap to running state and makes sure hulkazoid gets moving
void Enemy::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(deltaPos.x) > 0.f) setState(EnemyState::Running);

	if (name == "hulkazoid_enemy") velocity.x = -50;
	
	idleAnim.update(deltaTime);
}

//Function called every frame while Enemy is in Running state
//Checks if the length of vector deltaPos equals 0, change state depending on enemy name.
//Checks if the y of deltaPos is greater than 0 to change state if enemy is not Vorz.
void Enemy::doRunning(float deltaTime){
	doMovement(deltaTime);

	if (glm::length(deltaPos) == 0.0f) (name == "vorz_enemy") ? setState(EnemyState::Return) : setState(EnemyState::Idle);
	
	if (deltaPos.y > 0.f && name != "vorz_enemy") setState(EnemyState::Falling);

	runAnim.update(deltaTime);
}

//Function called every frame while Enemy is in Falling state
//Checks if the y of deltaPos equals 0 and then if the absolute of the x of velocity is greater than 0 (if velocity is not 0)
// set state to running, otherwise set it to idle.
void Enemy::doFalling(float deltaTime){
	doMovement(deltaTime);

	if (deltaPos.y == 0.f) {
		if (glm::abs(velocity.x) > 0.f) setState(EnemyState::Running);
		else setState(EnemyState::Idle);
	}

	idleAnim.update(deltaTime);
}
//Function called every frame while Enemy is in Return state
//Calls returnToStartPos
void Enemy::doReturn(float deltaTime) {
	doMovement(deltaTime);

	returnToStartPos();

	runAnim.update(deltaTime);
}

//Applies gravity to the y of velocity depending on the time between current and last frame in ms
void Enemy::Gravity(float deltaTime){
	velocity.y += gravity * deltaTime;
}

//Checks if the Enemy is inside of the Level's bounds, if not, correct position and reset velocity and/or acceleration to 0
void Enemy::CheckBounds() {
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

	if (glm::abs(correction.y) > 0.f) {
		velocity.y = 0.f;
	}
	if (glm::abs(correction.x) > 0.f) {
		velocity.x = 0.f;
		acceleration.x = 0.f;
	}

	translate(correction);
}

//First checks if the enemy is in the correct state then checks enemy type
//Then checks if the original entity is at the correct location, then sets startPos and sets acceleration.
void Enemy::Attack(std::shared_ptr<Entity> entity) {
	if (state == EnemyState::Idle) {
		if (enemyType == "horizontal") {
			if (entity->getPosition().y >= getPosition().y - 10 && entity->getPosition().y <= getPosition().y + 42) {
				if (entity->getPosition().x > getPosition().x) {
					setStartPos(getPosition());
					setAccelerationX(100);
					setState(EnemyState::Running);
				}
				else if (entity->getPosition().x < getPosition().x) {
					setStartPos(getPosition());
					setAccelerationX(-100);
					setState(EnemyState::Running);
				}
			}
		}
		else if (enemyType == "vertical") {
			if (entity->getPosition().x >= getPosition().x - 32 && entity->getPosition().x <= getPosition().x + 32) {
					setStartPos(getPosition());
					setAccelerationY(500);
					setState(EnemyState::Running);
			}
		}
	}
}

//Calculates the difference between start position and current position, if this is not equal to zero, set velocity depending on 
//if dPos.n is positive or negative, if dPos is less than 5, set velocity to 0 and set state to Idle
void Enemy::returnToStartPos() {
	auto initialPos = getStartPos();
	auto newPos = getPosition();
	auto dPos = initialPos - newPos;
	if (dPos.x != 0) setVelocityX(dPos.x > 0 ? 100 : -100);
	if (dPos.y != 0) setVelocityY(dPos.y > 0 ? 100 : -100);
	if (glm::abs(dPos.x) < 5 && glm::abs(dPos.y) < 5) {
			setVelocity({ 0, 0 });
			setState(EnemyState::Idle);
	}
}

void Enemy::setStartPos(glm::vec2 pos) {
	startPos = pos;
}

glm::vec2 Enemy::getStartPos()
{
	return glm::vec2(startPos);
}

