#pragma once

#include "Entity.hpp"


#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>



class Player : public Entity{
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

	explicit Player(const glm::vec2& pos, class Background* backside);
	virtual void update(float deltaTime, Math::Camera2D& camera) override;
	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override; 
	virtual void Gravity(float deltaTime) override;
	virtual void CheckBounds();

	void setVelocity(const glm::vec2& vel);
	void setVelocityX(const float velX);
	void setVelocityY(const float velY);
	const glm::vec2& getVelocity() const;

	void setAcceleration(const glm::vec2& acc);
	void setAccelerationX(const float accX);
	void setAccelerationY(const float accY);
	const glm::vec2& getAcceleration() const;

private:

	void setState(State newState);

	void doIdle(float deltaTime, Math::Camera2D& camera);
	void doRunning(float deltaTime, Math::Camera2D& camera);
	void doMovement(float deltaTime, Math::Camera2D& camera);
	void doMove(float deltaTime, Math::Camera2D& camera);
	void doJump(float deltaTime, Math::Camera2D& camera);
	void doFalling(float deltaTime, Math::Camera2D& camera);
	
	glm::vec2 velocity{ 0 };
	glm::vec2 acceleration{ 0 };
	glm::vec2 deltaPos{ 0 };
	float speed{ 120.0f };
	float maxHorizontal{ 200.f };
	float accelerationMultiplier = 1.0f;
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Math::AABB aabb;

	class Background* backside;

	State state = State::None;
	

	float referenceAccelDamping = 0.6f;
	float referenceVelocDamping = 0.8f;
	float referenceFPS = 30.f;


	static inline const float maxJumpHeight = 250.0f;
	static inline const float jumpTime = 0.5f;
	static inline const float gravity = 2.0f * maxJumpHeight / (jumpTime * jumpTime);
	static inline const float jumpSpeed = -std::sqrt(2.0f * maxJumpHeight * gravity);
};
