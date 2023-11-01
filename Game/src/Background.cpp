#include "Background.hpp"

#include<Graphics/ResourceManager.hpp>
#include<Graphics/Image.hpp>
#include<Graphics/Sprite.hpp>
#include<Math/Camera2D.hpp>

using namespace Graphics;

Background::Background() = default;

Background::Background() {
	auto groundback = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	WorldMap1 = Sprite(groundback);
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
}

void Background::setState(BackgroundState newState) {
	if (newState != state) {
		switch (newState) {
		case BackgroundState::Level1:
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

void Background::update(float deltaTime) {

}

float Background::getWidth(Sprite map) {
	return map.getWidth();
}

float Background::getHeight(Sprite map) {
	return map.getWidth();
}

Sprite Background::getLevelMap(BackgroundState level) {
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