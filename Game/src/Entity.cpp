#include <Entity.hpp>

#include <iostream>
#include <Math/Camera2D.hpp>

Entity::Entity(const std::string& name, const std::string& type, const glm::vec2& pos, const Math::AABB& aabb) :
	name(name),
	entityType(type),
	transform{ pos },
	aabb{ aabb },
	healthPoints(1)
{ }

void Entity::setPosition(const glm::vec2& pos) {
	transform.setPosition(pos);
}

const glm::vec2& Entity::getPosition() const {
	return transform.getPosition();
}

const std::string& Entity::getName() const
{
	return name;
}

const std::string& Entity::getType() const
{
	return entityType;
}

void Entity::translate(const glm::vec2& t) {
	transform.translate(t);
}

const Math::AABB Entity::getAABB() const {
	return transform * aabb;
}

//Checks if 2 entities collide
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

void Entity::doDamage(){
	healthPoints--;
}

int Entity::getHP(){
	return healthPoints;
}

void Entity::setLives(int life) {
	lives = life;
}

const int Entity::getLives() const
{
	return lives;
}

void Entity::setCoins(const int coin)
{
	coins = coin;
}

void Entity::setCoinsPlusOne()
{
	coins++;
}

const int Entity::getCoins() const
{
	return coins;
}

//Reset Speed
void Entity::resetSpeed() {
	setAcceleration({ 0, 0 });
	setVelocity({ 0, 0 });
}

//Checks if Entity is inside the scope of the Camera.
bool Entity::checkCameraBounds(const Math::Camera2D& camera)
{
	if (getPosition().x  < camera.getRightEdge() + 16
		&& getPosition().x  > camera.getLeftEdge() - 16
		&& getPosition().y > camera.getTopEdge()
		&& getPosition().y < camera.getBottomEdge() + 32) {
		return true;
	}
	else {
		return false;
	}
	
}



