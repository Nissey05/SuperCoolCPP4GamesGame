#include <Entity.hpp>

#include <iostream>

Entity::Entity(const glm::vec2& pos, const Math::AABB& aabb)
	: transform{ pos }
	, aabb{ aabb }
{

}
void Entity::setPosition(const glm::vec2& pos) {
	transform.setPosition(pos);
}

const glm::vec2& Entity::getPosition() const {
	return transform.getPosition();
}

void Entity::translate(const glm::vec2& t) {
	transform.translate(t);
}

const Math::AABB Entity::getAABB() const {
	return transform * aabb;
}

bool Entity::collides(const Entity& entity) const {
	return getAABB().intersect(entity.getAABB());
}

void Entity::Gravity(float deltaTime) {
	transform.setPosition(transform.getPosition() + glm::vec2{0, 1.f});
}

void Entity::setVelocity(const glm::vec2& vel)
{
	velocity = vel;
}

void Entity::setVelocityX(const float velX)
{
	velocity.x = velX;
}

void Entity::setVelocityY(const float velY)
{
	velocity.y = velY;
}

const glm::vec2& Entity::getVelocity() const
{
	return velocity;
}

void Entity::setAcceleration(const glm::vec2& acc)
{
	acceleration = acc;
}

void Entity::setAccelerationX(const float accX)
{
	acceleration.x = accX;
}

void Entity::setAccelerationY(const float accY)
{
	acceleration.y = accY;
}

const glm::vec2& Entity::getAcceleration() const
{
	return acceleration;
}


