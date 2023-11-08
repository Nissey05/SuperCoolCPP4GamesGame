#include "Background.hpp"

#include <Graphics/ResourceManager.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Input.hpp>
#include <Math/Camera2D.hpp>
#include <Math/AABB.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Hulkazoid.hpp" 
#include "Utils.hpp"
#include <iostream>

using namespace Graphics;

Background::Background(Player* player)
	: player(player)
{
	auto screenStart = ResourceManager::loadImage("assets/Pinky/StartScreen.png");
	startScreen = Sprite(screenStart);
	auto groundBack = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	worldMap1 = Sprite(groundBack);
}


void Background::draw(Graphics::Image& image, const Math::Camera2D& camera) {
	switch (state)
	{
	case BackgroundState::Level1:
		image.drawSprite(worldMap1, camera);
		drawAssets(image, camera);
		break;
	case BackgroundState::Level2:
		image.drawSprite(worldMap2, camera);
		drawAssets(image, camera);
		break;
	case BackgroundState::Level3:
		image.drawSprite(worldMap3, camera);
		drawAssets(image, camera);
		break;
	case BackgroundState::Start:
		image.drawSprite(startScreen, camera);
		break;
	case BackgroundState::Dead:
		image.drawSprite(deathScreen, camera);
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
			enemyVec = {
				std::make_shared<Hulkazoid>(glm::vec2{620, 675}, this),
				std::make_shared<Hulkazoid>(glm::vec2{800, 550}, this),
				std::make_shared<Hulkazoid>(glm::vec2{1180, 670}, this)
			};
			player->setPosition({ 125, 720 });
			break;
		case BackgroundState::Level2:
			break;
		case BackgroundState::Level3:
			break;
		case BackgroundState::Start:
			aabbVec = {};
			enemyVec = {};
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

void Background::update(float deltaTime) {
	switch (state) {
	case BackgroundState::Start:
		if(Input::getButtonDown("e")) setState(BackgroundState::Level1);
		break;
	case BackgroundState::Level1:
		for (auto& enemy : enemyVec) {
			enemy->update(deltaTime);
		}
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
	case BackgroundState::Start:
		return startScreen;
		break;
	case BackgroundState::Level1:
		return worldMap1;
		break;
	case BackgroundState::Level2:
		return worldMap2;
		break;
	case BackgroundState::Level3:
		return worldMap3;
		break;
	}
}

void Background::setLevelMap(BackgroundState map) {
	setState(map);
}

void Background::drawAssets(Graphics::Image& image, const Math::Camera2D& camera) {
	for (const auto& enemy : enemyVec) {
		if (enemy->getPosition().x < camera.getRightEdge() && enemy->getPosition().x > camera.getLeftEdge()) {
			enemy->draw(image, camera);
		}
	}
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


