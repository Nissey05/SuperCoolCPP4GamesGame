#pragma once

#include "Entity.hpp"

#include <Graphics/SpriteAnim.hpp>

class Enemy : public Entity {
public:
	enum class EnemyState
	{
		None,
		Idle,
		Dead,
		Running,
		Falling,
	};
	Enemy() = default;
	
	explicit Enemy(const glm::vec2& pos, class Background* backside);

	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override;

	virtual void Gravity(float deltaTime) override;


private:

	void setState(EnemyState newState);

	glm::vec2 velocity{ 0 };
	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doFalling(float deltaTime);

	void CheckBounds();

	EnemyState state;
	Graphics::SpriteAnim idleAnimEnemy;
	Graphics::SpriteAnim runAnimEnemy;

	class Background* backside;

	static inline const float maxJumpHeight = 250.0f;
	static inline const float jumpTime = 0.5f;
	static inline const float gravity = 2.0f * maxJumpHeight / (jumpTime * jumpTime);
	static inline const float jumpSpeed = -std::sqrt(2.0f * maxJumpHeight * gravity);
};