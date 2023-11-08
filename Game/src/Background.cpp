#include "Background.hpp"

#include <Graphics/ResourceManager.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Math/Camera2D.hpp>
#include <Math/AABB.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Utils.hpp"
#include <iostream>

using namespace Graphics;

Background::Background(int overload)
{
	auto groundback = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	WorldMap1 = Sprite(groundback);

	setState(BackgroundState::Level1);
}


void Background::draw(Graphics::Image& image, const Math::Camera2D& camera) {
	switch (state)
	{
	case BackgroundState::Level1:
		image.drawSprite(WorldMap1, camera);
		break;
	case BackgroundState::Level2:
		image.drawSprite(WorldMap2, camera);
		break;
	case BackgroundState::Level3:
		image.drawSprite(WorldMap3, camera);
		break;
	case BackgroundState::Start:
		image.drawSprite(StartScreen, camera);
		break;
	case BackgroundState::Dead:
		image.drawSprite(DeathScreen, camera);
		break;
	}

#if _DEBUG
	for (const auto& aabb : aabbVec)
		image.drawAABB(camera * aabb, Color::Yellow, {}, FillMode::WireFrame);
#endif
}

void Background::setState(BackgroundState newState) {
	if (newState != state) {
		switch (newState) {
		case BackgroundState::Level1:
			aabbVec = {
				Math::AABB({0, 737, 0}, {382, 800, 0}),
				Math::AABB({515, 737, 0}, { 970, 800, 0 }),
				Math::AABB({1066, 737, 0}, { 1600, 800, 0 }),
				Math::AABB({375, 538, 0}, { 505, 578, 0 }),
				Math::AABB({745, 614, 0}, { 872, 659, 0 }),
			};
			break;
		case BackgroundState::Level2:
			break;
		case BackgroundState::Level3:
			break;
		case BackgroundState::Start:
			break;
		case BackgroundState::Dead:
			break;
		}

		state = newState;
	}
}

BackgroundState Background::getState() {
	return state;
}

void Background::update(Player& player) {
	switch (state) {
	case BackgroundState::Level1:
		doLevelOne();
		break;
	}
}

int Background::getWidth() {
	Sprite map = getLevelMap();
	return map.getWidth();
}

int Background::getHeight() {
	Sprite map = getLevelMap();
	return map.getHeight();
}

Sprite& Background::getLevelMap() {
	auto level = getState();
	switch (level) {
	case BackgroundState::Level1:
		return WorldMap1;
		break;
	case BackgroundState::Level2:
		return WorldMap2;
		break;
	case BackgroundState::Level3:
		return WorldMap3;
		break;
	}
}

void Background::setLevelMap(BackgroundState map) {
	setState(map);
}

void Background::doLevelOne() {
	
} 

void Background::resolveCollisionForLevel(Entity* entity) {
	for (const auto& aabb : aabbVec) {
		glm::vec2 correction = Utils::GetCollisionCorrection(entity->getAABB(), aabb);
		entity->translate(correction);

		if (glm::abs(correction.y) > 0.f) {
			entity->setVelocityY(0.f);
			entity->setAccelerationY(0.f);

		}
		if (glm::abs(correction.x) > 0.f) {
			entity->setVelocityX(0.f);
			entity->setAccelerationX(0.f);
		}
	}
}


