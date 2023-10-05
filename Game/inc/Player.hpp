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
		Jumping
	};
	//Default constructor
	Player();

	explicit Player(const glm::vec2& pos);

	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const glm::mat3& transform) override; 

	void Gravity(bool coll = false);


private:

	void setState(State newState);

	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doMovement(float deltaTime);
	
	glm::vec2 position{ 0 };
	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };
	float runspeed{ 2.0f * speed };
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Math::AABB aabb;

	
	State state = State::None;
	
	bool Collision = false;
	bool Jumping = false;
};
