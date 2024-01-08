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
#include "Coin.hpp"
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
	auto screenWin = ResourceManager::loadImage("assets/Pinky/WinScreen.png");
	winScreen = Sprite(screenWin);
	auto map1 = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	worldMap1 = Sprite(map1);
	auto map2 = ResourceManager::loadImage("assets/Pinky/WorldMap2.png");
	worldMap2 = Sprite(map2);
	auto map3 = ResourceManager::loadImage("assets/Pinky/WorldMap3.png");
	worldMap3 = Sprite(map3);

	// Setup entity list
	entityList.reserve(50);
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
	case LevelState::Level3:
		image.drawSprite(worldMap3, camera);
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
		case LevelState::Level3:
			initLevelThree();
			break;
		case LevelState::Start:
			resetAssets();
			break;
		case LevelState::Dead:
			resetAssets();
			break;
		case LevelState::Win:
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
	if (Input::getButtonDown("f")) nextLevel();
	switch (state) {
	case LevelState::Start:
		if(Input::getButtonDown("start")) nextLevel();
		break;
	case LevelState::Level1:
		for (size_t i = 0; i < entityList.size(); i++) {
			auto& entity = entityList[i];

			// Checks if entity is not a nullptr
			if (entity) {
			//	if (i != 0) continue;
				entity->update(deltaTime);

				if (entity->getName() != "player" && entity->getHP() <= 0) {
					entityList.erase(entityList.begin() + i);
					i--;
					continue ;
				}
			}
		}
		break;
	case LevelState::Level2:
		for (size_t i = 0; i < entityList.size(); i++) {
			auto& entity = entityList[i];

			// Checks if entity is not a nullptr
			if (entity) {
			//	if (i != 0) continue;
				entity->update(deltaTime);

				if (entity->getName() != "player" && entity->getHP() <= 0) {
					entityList.erase(entityList.begin() + i);
					i--;
					continue;
				}
				if (entity->getName() == "vorz_enemy") {
					const auto& enemy = std::dynamic_pointer_cast<Enemy>(entity);
					enemy->Attack(entityList[0]);
				}
			}
		}
		break;
	case LevelState::Level3:
		for (size_t i = 0; i < entityList.size(); i++) {
			auto& entity = entityList[i];

			// Checks if entity is not a nullptr
			if (entity) {
				//	if (i != 0) continue;
				entity->update(deltaTime);

				if (entity->getName() != "player" && entity->getHP() <= 0) {
					entityList.erase(entityList.begin() + i);
					i--;
					continue;
				}
				if (entity->getName() == "vorz_enemy") {
					const auto& enemy = std::dynamic_pointer_cast<Enemy>(entity);
					enemy->Attack(entityList[0]);
				}
			}
		}
		break;
	case LevelState::Dead:
		if (Input::getButtonDown("use")) {
			entityList[0]->resetSpeed();
			entityList[0]->setLives(6);
			setState(LevelState::Level1);
		}
			
		break;
	case LevelState::Win:
		if (Input::getButtonDown("use")) setState(LevelState::Start);
		break;

	}

	
}

int Level::getWidth() {
	return getLevelMap().getWidth();
}

int Level::getHeight() {
	return getLevelMap().getHeight();
}

void Level::initLevelOne(){
	
	// Setup collision
	aabbVec = {
		Math::AABB({0, 667, 0}, { 89, 725, 0 }),
		Math::AABB({0, 737, 0}, { 382, 800, 0 }),
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
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 226, 600 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 267, 371 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 439, 631 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 537, 277 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 573, 470 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 763, 385 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 936, 506 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 1204, 470 }, this));

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
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 16, 1642 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 21, 1256}, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 708, 802 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 661, 1046 }, this));
	entityList.push_back(std::make_shared<Vorz>(glm::vec2{ 658, 2749 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 364, 2639 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 762, 2612 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 278, 2464 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 660, 2400 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 23,  2141 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 319,  2061 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 715, 1906 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 271, 1786 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 32, 1602}, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 354, 1531 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 380, 1531 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 405, 1531 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 687, 1364 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 11, 1286 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 211, 1121 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 713, 957 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 331, 840 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 238, 702 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 15, 617 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 395, 525 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 411, 502 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 426, 473 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 448, 442 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 458, 409 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 470, 380 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 476, 347 }, this));
	
	


	// Reset player position
	entityList[0]->setPosition({ 225, 2897 });

	// End of level box
	endAABB = { { 683, 533, 0 }, { 800, 599, 0 } };
}

void Level::initLevelThree()
{
	// Setup collision
	aabbVec = {
		Math::AABB({ 0, 753, 0 }, { 321, 800, 0 }),
		Math::AABB({ 503, 647, 0 }, { 740, 691, 0 }),
		Math::AABB({ 921, 760, 0 }, { 993, 800, 0 }),
		Math::AABB({ 903, 527, 0 }, { 1175, 566, 0 }),
		Math::AABB({ 1125, 722, 0 }, { 1221, 756, 0 }),
		Math::AABB({ 1301, 632, 0 }, { 1783, 668, 0 }),
		Math::AABB({ 1927, 749, 0 }, { 3914, 800, 0 }),
		Math::AABB({ 2433, 648, 0 }, { 2535, 749, 0 }),
		Math::AABB({ 2672, 510, 0 }, { 2789, 624, 0 }),
		Math::AABB({ 2905, 400, 0 }, { 3059, 447, 0 }),
		Math::AABB({ 3191, 310, 0 }, { 3294, 749, 0 }),
		Math::AABB({ 4145, 612, 0 }, { 4252, 649, 0 }),
		Math::AABB({ 4444, 555, 0 }, { 4552, 577, 0 }),
		Math::AABB({ 4837, 624, 0 }, { 4967, 652, 0 }),
		Math::AABB({ 5167, 480, 0 }, { 5235, 515, 0 }),
		Math::AABB({ 5497, 384, 0 }, { 5575, 420, 0 }),
		Math::AABB({ 5759, 514, 0 }, { 5912, 544, 0 }),
		Math::AABB({ 6181, 699, 0 }, { 8000, 800, 0 })
	};
	// Setup deathbox collisions
	deathBoxAABB = {
		Math::AABB({ 1197, 717, 0 }, { 1220, 722, 0 }),
		Math::AABB({ 1489, 627, 0 }, { 1572, 633, 0 }),
		Math::AABB({ 2789, 588, 0 }, { 2795, 623, 0 }),
		Math::AABB({ 3294, 725, 0 }, { 3299, 748, 0 }),
		Math::AABB({ 0, 799, 0 }, { 8000, 800, 0 })
	};

	// Reset all entities but player and add enemies
	entityList.erase(entityList.begin() + 1, entityList.end());
	entityList.push_back(std::make_shared<Hulkazoid>(glm::vec2{ 620, 675 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 410, 619 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 930, 723 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 965, 723 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 998, 723 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 1528, 529 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 3174, 742 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 3489, 469 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 3601, 569 }, this));
	entityList.push_back(std::make_shared<Coin>(glm::vec2{ 4733, 464 }, this));

	// Reset player position
	entityList[0]->setPosition({ 118, 750 });

	// End of level box
	endAABB = { { 7797, 608, 0 }, { 8000, 699, 0 } };
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
	case LevelState::Level3:
		return worldMap3;
		break;
	}
}


void Level::nextLevel(){
	entityList[0]->resetSpeed();
	auto currentLevel = getState();
	switch (currentLevel) {
	case LevelState::Start:
		setState(LevelState::Level1);
		break;
	case LevelState::Level1:
		setState(LevelState::Level2);
		break;
	case LevelState::Level2:
		setState(LevelState::Level3);
		break;
	case LevelState::Level3:
		setState(LevelState::Win);
		break;
	}
}

void Level::setLevelMap(LevelState map) {
	setState(map);
}

void Level::drawAssets(Graphics::Image& image, const Math::Camera2D& camera) {
	for (const auto& entity : entityList) {
		if (entity->checkCameraBounds(camera)) {
			entity->draw(image, camera);
		}
		else {
			entity->setVelocityX(0.f);
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

	for (const auto& death : deathBoxAABB) {
		if (entity->getAABB().intersect(death)) entity->doDamage();
	}

	if (entityList[0]->getAABB().intersect(endAABB)) nextLevel();
}

void Level::resolveEntityCollision(Entity* entity){
	// Resolve collision for entities
	for (const auto& entity : entityList) {
		// Ignore player as we're colliding against them
		if (entity->getName() == "player") continue;

		// If we're an enemy, push them back
		const auto& player = std::dynamic_pointer_cast<Player>(entityList[0]);
		glm::vec2 correction = Utils::GetCollisionCorrection(entity->getAABB(), player->getAABB());


		if (entity->getType() == "enemy") {
			if (correction.y > 0.f) {
				if (entity->getName() == "hulkazoid_enemy") {
					entity->doDamage();
					player->setVelocityY(player->getJumpSpeed() / 2);
					player->setCoins(player->getCoins() + 5);
				}
				else {
					player->doDamage();
				}
			}
			if (glm::abs(correction.x) > 0.f || correction.y < 0.f) {
				player->doDamage();
			}
		}
		else if (entity->getType() == "collectible") {
			if (glm::abs(correction.y) > 0.f || glm::abs(correction.x) > 0.f) {
				entity->doDamage();
				player->setCoinsPlusOne();
			}
		}
	}
}


