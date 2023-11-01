#pragma once

#include <Graphics/Sprite.hpp>
#include <Math/AABB.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/Image.hpp>

enum class BackgroundState
{
	Level1,
	Level2,
	Level3,
	Start,
	Dead

};

class Background {
public:
	//Default Constructor
	Background();

	explicit Background();

	float getWidth(Sprite map);
	float getHeight(Sprite map);
	Graphics::Sprite getLevelMap(BackgroundState level);
	BackgroundState getState();
	void setLevelMap(BackgroundState map);
	void draw(Graphics::Image& image, const Math::Camera2D& camera);

	void update(float deltaTime);

private:

	void setState(BackgroundState newState);
	

	Graphics::Sprite WorldMap1;
	Graphics::Sprite WorldMap2;
	Graphics::Sprite WorldMap3;
	Graphics::Sprite StartScreen;
	Graphics::Sprite DeathScreen;

	BackgroundState state = BackgroundState::Start;

};