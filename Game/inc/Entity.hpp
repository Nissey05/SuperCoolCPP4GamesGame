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

	void setVelocity(const glm::vec2& vel);
	void setVelocityX(const float velX);
	void setVelocityY(const float velY);
	const glm::vec2& getVelocity() const;

	void setAcceleration(const glm::vec2& acc);
	void setAccelerationX(const float accX);
	void setAccelerationY(const float accY);
	const glm::vec2& getAcceleration() const;

protected:
	Entity() = default;
	Entity(const glm::vec2& pos, const Math::AABB& aabb);

	Math::AABB aabb;
	Math::Transform2D transform;
	glm::vec2 velocity{ 0 };
	glm::vec2 acceleration{ 0 };
};
