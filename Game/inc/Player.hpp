#pragma once


#include <Graphics/SpriteAnim.hpp>
#include <Math/AABB.hpp>
#include <glm/vec2.hpp>


class Player {
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

	void update(float deltaTime);

	void draw(Graphics::Image& image);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	void translate(const glm::vec2& t);

	const Math::AABB getAABB() const;

	void doIdle(float deltaTime);

	void doRunning(float deltaTime);

	void doMovement(float deltaTime);


private:
	void setState(State newState);

	State state = State::None;
	glm::vec2 position{ 0 };
	glm::vec2 velocity{ 0 };
	float speed{ 60.0f };
	float runspeed{ 2.0f * speed };
	Graphics::SpriteAnim idleAnim;
	Graphics::SpriteAnim runAnim;
	Math::AABB aabb;
};
