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

};

Enemy::Enemy(const std::string& name, const glm::vec2& pos, Level* level, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath) :
	Entity(name, pos, { {0, 0, 0}, {32, 32, 0} }),
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
}

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
	}
}

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
	}

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

void Enemy::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(deltaPos.x) > 0.f) setState(EnemyState::Running);

	if (name == "hulkazoid_enemy") velocity.x = -50;

	if (name == "vorz_enemy") {
		returnToStartPos();
	}
	
	idleAnim.update(deltaTime);
}

void Enemy::doRunning(float deltaTime){
	doMovement(deltaTime);
	
	if (glm::abs(deltaPos.x) == 0.f) setState(EnemyState::Idle);
	if (deltaPos.y > 0.f) setState(EnemyState::Falling);
	
	runAnim.update(deltaTime);
}

void Enemy::doFalling(float deltaTime){
	doMovement(deltaTime);

	if (deltaPos.y == 0.f) {
		if (glm::abs(velocity.x) > 0.f) setState(EnemyState::Running);
		else setState(EnemyState::Idle);
	}

	idleAnim.update(deltaTime);
}

void Enemy::Gravity(float deltaTime){
	velocity.y += gravity * deltaTime;
}

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

	translate(correction);

	if (glm::abs(correction.y) > 0.f) {
		velocity.y = 0.f;
	}
	if (glm::abs(correction.x) > 0.f) {
		velocity.x = 0.f;
		acceleration.x = 0.f;
	}
}


void Enemy::Attack(std::shared_ptr<Entity> entity) {
	if (state == EnemyState::Idle) {
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
}

void Enemy::returnToStartPos() {
	auto initialPos = getStartPos();
	auto newPos = getPosition();
	auto dPos = initialPos - newPos;
	if (dPos.x != 0) setVelocityX(dPos.x / 5);
}

void Enemy::setStartPos(glm::vec2 pos) {
	startPos = pos;
}

glm::vec2 Enemy::getStartPos()
{
	return glm::vec2(startPos);
}

