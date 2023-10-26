#pragma once

#include "Entity.hpp"


#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <glm/vec2.hpp>


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
	Player();

	explicit Player(const glm::vec2& pos, Graphics::Sprite* Backside);

	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override; 

	virtual void Gravity(float deltaTime) override;

	void CheckBounds(glm::vec2& pos);


private:

	void setState(State newState);

	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doMovement(float deltaTime);
	void doMove(float deltaTime);
	void doJump(float deltaTime);

	void doFalling(float deltaTime);
	
	glm::vec2 velocity{ 0 };
	glm::vec2 acceleration{ 0 };
	glm::vec2 deltaPos{ 0 };
	float speed{ 120.0f };
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Math::AABB aabb;

	Graphics::Sprite* Backside;

	State state = State::None;
	
	bool Collision = false;


	static inline const float maxJumpHeight = 250.0f;
	static inline const float jumpTime = 0.5f;
	static inline const float gravity = 2.0f * maxJumpHeight / (jumpTime * jumpTime);
	static inline const float jumpSpeed = -std::sqrt(2.0f * maxJumpHeight * gravity);
};
