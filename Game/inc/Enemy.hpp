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
		Return,
	};
	
	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override;


	virtual void Gravity(float deltaTime) override;

	void Attack(std::shared_ptr<Entity> entity);

	void returnToStartPos();

	void setStartPos(glm::vec2 pos);
	glm::vec2 getStartPos();

protected:
	Enemy() = default;
	explicit Enemy(const std::string& name, const glm::vec2& pos, class Level* level, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath);
	explicit Enemy(const std::string& name, const glm::vec2& pos, class Level* level, const Math::AABB& aabb, std::string idlePath, std::string runPath, std::string fallPath, const std::string& type);

private:
	void setState(EnemyState newState);

	void doMovement(float deltaTime);
	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doFalling(float deltaTime);
	void doReturn(float deltaTime);

	void CheckBounds();

	glm::vec2 deltaPos{ 0 };
	glm::vec2 startPos{};
	EnemyState state;
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Graphics::SpriteAnim fallAnim;

	std::string enemyType;

	bool gocheck = false;

	class Level* level;

	static inline const float maxJumpHeight = 250.0f;
	static inline const float jumpTime = 0.5f;
	static inline const float gravity = 2.0f * maxJumpHeight / (jumpTime * jumpTime);
	static inline const float jumpSpeed = -std::sqrt(2.0f * maxJumpHeight * gravity);
};