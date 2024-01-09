#pragma once

#include <Graphics/Image.hpp>
#include <Math/AABB.hpp>
#include <Math/Transform2D.hpp>
#include <Math/Camera2D.hpp>


class Entity {
public: 
	virtual ~Entity() = default;

	virtual void update(float deltaTime) = 0;
	virtual void draw(Graphics::Image& image, const Math::Camera2D& camera) = 0;
	
	void translate(const glm::vec2& t);
	const Math::AABB getAABB() const;
	virtual bool collides(const Entity& entity) const;
	virtual void Gravity(float deltaTime);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;

	const std::string& getName() const;

	const std::string& getType() const;

	void setVelocity(const glm::vec2& vel);
	void setVelocityX(const float velX);
	void setVelocityY(const float velY);
	const glm::vec2& getVelocity() const;

	void setAcceleration(const glm::vec2& acc);
	void setAccelerationX(const float accX);
	void setAccelerationY(const float accY);
	const glm::vec2& getAcceleration() const;

	void setLives(const int life);
	const int getLives() const;

	void setCoins(const int coins);
	void setCoinsPlusOne();
	const int getCoins() const;

	void resetSpeed();

	bool checkCameraBounds(const Math::Camera2D& camera);
	

	void doDamage();
	int getHP();

protected:
	Entity() = default;
	Entity(const std::string& name, const std::string& entityType, const glm::vec2& pos, const Math::AABB& aabb);
	int lives = 5;
	int coins = 0;
	std::string name;
	std::string entityType;
	int healthPoints;
	Math::AABB aabb;
	Math::Transform2D transform;
	glm::vec2 velocity{ 0 };
	glm::vec2 acceleration{ 0 };

};
