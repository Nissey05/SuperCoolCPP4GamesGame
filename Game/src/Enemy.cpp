#include <Enemy.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Background.hpp>
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


Enemy::Enemy(const glm::vec2& pos, Background* backside, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath)
	: Entity{ pos, { {0, 0, 0}, {32, 32, 0} }},
	backside(backside)
{
	auto idleSprites = ResourceManager::loadSpriteSheet(idlePath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	auto runSprites = ResourceManager::loadSpriteSheet(runPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	runAnim = SpriteAnim{ runSprites, 6 };
	auto fallSprites = ResourceManager::loadSpriteSheet(fallPath, 32, 32, 0, 0, BlendMode::AlphaBlend);
	fallAnim = SpriteAnim{ fallSprites, 6 };

	setState(EnemyState::Idle);

	transform.setScale({-1, 1});
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

	Gravity(deltaTime);

	newPos += velocity * deltaTime;

	setPosition(newPos);

	CheckBounds();

	backside->resolveCollisionForLevel(this);

	deltaPos = getPosition() - initialPos;
}

void Enemy::doIdle(float deltaTime) {
	doMovement(deltaTime);

	if (glm::abs(deltaPos.x) > 0.f) setState(EnemyState::Running);
	
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
	else if (aabb.max.x >= backside->getLevelMap().getWidth()) {
		correction.x = backside->getLevelMap().getWidth() - aabb.max.x;
	}

	if (aabb.min.y < 0.f) {
		correction.y = -aabb.min.y;
	}
	else if (aabb.max.y >= backside->getLevelMap().getHeight()) {
		correction.y = backside->getLevelMap().getHeight() - aabb.max.y;
	}

	translate(correction);

	if (glm::abs(correction.y) > 0.f) {
		velocity.y = 0.f;
	}
	if (glm::abs(correction.x) > 0.f) {
		velocity.x = 0.f;
	}
}

