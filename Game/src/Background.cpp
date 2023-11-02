#include "Background.hpp"

#include<Graphics/ResourceManager.hpp>
#include<Graphics/Image.hpp>
#include<Graphics/Sprite.hpp>
#include<Math/Camera2D.hpp>
#include<Math/AABB.hpp>

using namespace Graphics;

Background::Background() = default;

Background::Background(int overload) {
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

#if _DEBUG
	image.drawAABB(aabb1, Color::Yellow, {}, FillMode::WireFrame);
	image.drawAABB(aabb2, Color::Yellow, {}, FillMode::WireFrame);
	image.drawAABB(aabb3, Color::Yellow, {}, FillMode::WireFrame);
	image.drawAABB(aabb4, Color::Yellow, {}, FillMode::WireFrame);
	image.drawAABB(aabb5, Color::Yellow, {}, FillMode::WireFrame);
#endif
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



void Background::update() {
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