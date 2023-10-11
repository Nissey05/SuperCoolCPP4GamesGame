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
	};
	//Default constructor
	Player();

	explicit Player(const glm::vec2& pos, Graphics::Sprite* Backside);

	virtual void update(float deltaTime) override;

	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) override; 

	virtual void Gravity(glm::vec2& newPos, float deltaTime, bool coll = false) override;


private:

	void setState(State newState);

	void doIdle(float deltaTime);
	void doRunning(float deltaTime);
	void doMovement(float deltaTime);
	
	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Math::AABB aabb;

	Graphics::Sprite* Backside;

	State state = State::None;
	
	bool Collision = false;
	bool Jumping = false;
};
