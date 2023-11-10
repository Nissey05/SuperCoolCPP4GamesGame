#include "Level.hpp"

#include <Graphics/ResourceManager.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/Input.hpp>
#include <Math/Camera2D.hpp>
#include <Math/AABB.hpp>
#include "Player.hpp"
#include "Enemy.hpp"
#include "Hulkazoid.hpp" 
#include "Vorz.hpp"
#include "Utils.hpp"
#include <iostream>
#include <algorithm>

using namespace Graphics;

Level::Level(std::shared_ptr<Player> player)
{
	// Setup level sprites
	auto screenStart = ResourceManager::loadImage("assets/Pinky/StartScreen.png");
	startScreen = Sprite(screenStart);
	auto screenDeath = ResourceManager::loadImage("assets/Pinky/DeathScreen.png");
	deathScreen = Sprite(screenDeath);
	auto screenWin = ResourceManager::loadImage("assets/Pinky/DeathScreen.png");
	winScreen = Sprite(screenWin);
	auto map1 = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	worldMap1 = Sprite(map1);
	auto map2 = ResourceManager::loadImage("assets/Pinky/WorldMap2.png");
	worldMap2 = Sprite(map2);

	// Setup entity list
	entityList.push_back(player);
}


void Level::draw(Graphics::Image& image, const Math::Camera2D& camera) {
	switch (state)
	{
	case LevelState::Level1:
		image.drawSprite(worldMap1, camera);
		drawAssets(image, camera);
		break;
	case LevelState::Level2:
		image.drawSprite(worldMap2, camera);
		drawAssets(image, camera);
		break;
	case LevelState::Start:
		image.drawSprite(startScreen, camera);
		break;
	case LevelState::Dead:
		image.drawSprite(deathScreen, camera);
		break;
	case LevelState::Win:
		image.drawSprite(winScreen, camera);
		break;
	}

#if _DEBUG
	for (const auto& aabb : aabbVec)
		image.drawAABB(camera * aabb, Color::Yellow, {}, FillMode::WireFrame);
	for (const auto& pits : deathBoxAABB)
		image.drawAABB(camera * pits, Color::Red, {}, FillMode::WireFrame);
	image.drawAABB(camera * endAABB, Color::Red, {}, FillMode::WireFrame);
	
#endif
}

void Level::setState(LevelState newState) {
	if (newState != state) {
		switch (newState) {
		case LevelState::Level1:
			initLevelOne();
			break;
		case LevelState::Level2:
			initLevelTwo();
			break;
		case LevelState::Start:
			resetAssets();
			break;
		case LevelState::Dead:
			resetAssets();
			break;
		}

		state = newState;
	}
}

LevelState Level::getState() {
	return state;
}

void Level::update(float deltaTime) {
	switch (state) {
	case LevelState::Start:
		if(Input::getButtonDown("e")) nextLevel();
		break;
	case LevelState::Level1:
		for (size_t i = 0; i < entityList.size(); ) {
			auto& entity = entityList[i];

			// Checks if entity is not a nullptr
			if (entity) {
				entity->update(deltaTime);

				if (entity->getName() != "player" && entity->getHP() <= 0) {
					entityList.erase(entityList.begin() + i);
					continue;
				}
			}
			++i;
			if (Input::getButtonDown("e")) nextLevel();
		}
		break;
	case LevelState::Level2:
		for (size_t i = 0; i < entityList.size(); ) {
			auto& entity = entityList[i];

			// Checks if entity is not a nullptr
			if (entity) {
				entity->update(deltaTime);

				if (entity->getName() != "player" && entity->getHP() <= 0) {
					entityList.erase(entityList.begin() + i);
					continue;
				}
				if (entity->getName() == "vorz_enemy") {
					const auto& enemy = std::dynamic_pointer_cast<Enemy>(entity);
					enemy->Attack(entityList[0]);
				}
			}

			++i;
		}
		break;
	case LevelState::Dead:
		if (Input::getButtonDown("e")) {
			entityList[0]->setLives(6);
			setState(LevelState::Level1);
		}
			
		break;
	case LevelState::Win:
		if (Input::getButtonDown("e")) setState(LevelState::Start);
		break;

	}

	
}

int Level::getWidth() {
	Sprite map = getLevelMap();
	return map.getWidth();
}

int Level::getHeight() {
	Sprite map = getLevelMap();
	return map.getHeight();
}

void Level::initLevelOne(){
	//if (state != LevelState::Level1) setState(LevelState::Level1);
			// Setup collision
	aabbVec = {
		Math::AABB({0, 737, 0}, {382, 800, 0}),
		Math::AABB({515, 737, 0}, { 970, 800, 0 }),
		Math::AABB({1066, 737, 0}, { 1600, 800, 0 }),
		Math::AABB({375, 538, 0}, { 505, 578, 0 }),
		Math::AABB({745, 614, 0}, { 872, 659, 0 }),
	};
	// Setup deathbox collisions
	deathBoxAABB = {
		Math::AABB({382, 799, 0}, {515, 800, 0}),
		Math::AABB({970, 799, 0}, {1056, 800, 0})
	};

	// Reset all entities but player and add enemies
	entityList.erase(entityList.begin() + 1, entityList.end());
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 620, 675 }, this));
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 800, 550 }, this));
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 1180, 670 }, this));

	// Reset player position
	entityList[0]->setPosition({ 125, 720 });

	// End of level box
	endAABB = { { 1447, 487, 0 }, { 1600, 736, 0 } };
}

void Level::initLevelTwo(){
	// Setup collision
	aabbVec = {
		Math::AABB({0, 2899, 0}, {800, 3000, 0}),
		Math::AABB({564, 2786, 0}, {800, 2899, 0}),
		Math::AABB({527, 2601, 0}, {571, 2644, 0}),
		Math::AABB({230, 2516, 0}, {527, 2601, 0}),
		Math::AABB({0, 2317, 0}, {273, 2433, 0}),
		Math::AABB({379, 2052, 0}, {789, 2166, 0}),
		Math::AABB({321, 2123, 0}, {379, 2166, 0}),
		Math::AABB({0, 1841, 0}, {206, 1962, 0}),
		Math::AABB({206, 1947, 0}, {253, 1962, 0}),
		Math::AABB({295, 1647, 0}, {577, 1738, 0}),
		Math::AABB({589, 1493, 0}, {800, 1584, 0}),
		Math::AABB({0, 1306, 0}, {432, 1395, 0}),
		Math::AABB({615,1197, 0}, {800, 1278, 0}),
		Math::AABB({236, 1135, 0}, {277, 1147, 0}),
		Math::AABB({0, 1050, 0}, {202, 1147, 0}),
		Math::AABB({554, 993, 0}, {580, 1003, 0}),
		Math::AABB({319, 936, 0}, {554, 1003, 0}),
		Math::AABB({2, 759, 0}, {313, 811, 0}),
		Math::AABB({157, 599, 0}, {800, 633, 0})
	};
	// Setup deathbox collisions
	deathBoxAABB = {
		Math::AABB({731, 2781, 0}, {778, 2786, 0}),
		Math::AABB({1, 2312, 0}, {64, 2317, 0}),
		Math::AABB({231, 2511, 0}, {278, 2516, 0}),
		Math::AABB({374, 2099, 0}, {379, 2122, 0}),
		Math::AABB({661, 2047, 0}, {788, 2052, 0}),
		Math::AABB({3, 754, 0}, {34, 759, 0})
	};

	// Reset all entities but player and add enemies
	entityList.erase(entityList.begin() + 1, entityList.end());
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 340, 2897 }, this));
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 450, 2897 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 23, 1610 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 37, 1224 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 724, 770 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 677, 1014 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 674, 2717 }, this));


	// Reset player position
	entityList[0]->setPosition({ 225, 2897 });

	// End of level box
	endAABB = { { 683, 533, 0 }, { 800, 599, 0 } };
}

void Level::resetAssets(){
	// Reset aabb list
	aabbVec = {};
	// Reset all entities but player
	entityList.erase(entityList.begin() + 1, entityList.end());
	//Reset player
	entityList[0]->setPosition({ 0, 0 });
	//Reset end of level aabb
	endAABB = {};
}

Sprite& Level::getLevelMap() {
	auto level = getState();
	switch (level) {
	case LevelState::Start:
		return startScreen;
		break;
	case LevelState::Level1:
		return worldMap1;
		break;
	case LevelState::Level2:
		return worldMap2;
		break;
	}
}

void Level::nextLevel(){
	auto currentLevel = getState();
	switch (currentLevel) {
	case LevelState::Start:
		setState(LevelState::Level1);
		break;
	case LevelState::Level1:
		setState(LevelState::Level2);
		break;
	case LevelState::Level2:
		setState(LevelState::Win);
		break;
	}
}

void Level::setLevelMap(LevelState map) {
	setState(map);
}

void Level::drawAssets(Graphics::Image& image, const Math::Camera2D& camera) {
	for (const auto& entity : entityList) {
		if (entity->getPosition().x < camera.getRightEdge() && entity->getPosition().x > camera.getLeftEdge()) {
			entity->draw(image, camera);
		}
	}
} 

void Level::resolveCollisionForLevel(Entity* entity) {
	// Resolve collisions for level static objects
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

	// Resolve collision for entities
	for (const auto& entity : entityList) {
		// Ignore player as we're colliding against them
		if (entity->getName() == "player") continue;

		// If we're an enemy, push them back
		const auto& player = std::dynamic_pointer_cast<Player>(entityList[0]);
		glm::vec2 correction = Utils::GetCollisionCorrection(entity->getAABB(), player->getAABB());

		

		if (correction.y > 0.f) {
			if (entity->getName() == "hulkazoid_enemy") {
			entity->doDamage();
			player->setVelocityY(player->getJumpSpeed() / 2);
			}
			else {
				player->doDamage();
			}
		}
		if (glm::abs(correction.x) > 0.f || correction.y < 0.f ) {
			player->doDamage();
		}
	}

	for (const auto& death : deathBoxAABB) {
		if (entity->getAABB().intersect(death)) entity->doDamage();
	}

	if (entityList[0]->getAABB().intersect(endAABB)) nextLevel();
}
