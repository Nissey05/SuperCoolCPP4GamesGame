#pragma once

#include "Entity.hpp"


#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>



class Player : public Entity {
public:
	enum class State
	{
		None,
		Idle,
		Running,
		Attack,
		Dead,
		Jumping,
		Falling,
	};

	//Default constructor
	Player() = default;

	explicit Player(const glm::vec2& pos);
	virtual void update(float deltaTime) override;
	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override; 
	virtual void Gravity(float deltaTime) override;
	virtual void CheckBounds();

	void init(std::shared_ptr<class Level> level);
	float getJumpSpeed();
	

private:
	void setState(State newState);

	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doMovement(float deltaTime);
	void doJump(float deltaTime);
	void doFalling(float deltaTime);
	void doDead(float deltaTime);

	void coinCheck();

	glm::vec2 deltaPos{ 0 };
	float speed{ 150.0f };
	float maxHorizontal{ 200.f };
	float accelerationMultiplier = 1.0f;
	
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Graphics::SpriteAnim fallAnim;
	Graphics::SpriteAnim jumpAnim;
	Math::AABB aabb;

	std::shared_ptr<class Level> level;
	State state = State::None;

	float referenceAccelDamping = 0.6f;
	float referenceVelocDamping = 0.8f;
	float referenceFPS = 30.f;


	static inline const float maxJumpHeight = 250.0f;
	static inline const float jumpTime = 0.5f;
	static inline const float gravity = 2.0f * maxJumpHeight / (jumpTime * jumpTime);
	static inline const float jumpSpeed = -std::sqrt(2.0f * maxJumpHeight * gravity);
};
