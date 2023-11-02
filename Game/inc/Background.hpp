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

	explicit Background(int overload);

	int getWidth();
	int getHeight();
	Graphics::Sprite& getLevelMap();
	
	void setLevelMap(BackgroundState map);
	void draw(Graphics::Image& image, const Math::Camera2D& camera);

	void update();

private:

	void setState(BackgroundState newState);
	BackgroundState getState();
	void doLevelOne();

	Graphics::Sprite WorldMap1;
	Graphics::Sprite WorldMap2;
	Graphics::Sprite WorldMap3;
	Graphics::Sprite StartScreen;
	Graphics::Sprite DeathScreen;
	Math::AABB aabb1{ {0, 737, 0}, {382, 800, 0} };
	Math::AABB aabb2{ {515, 737, 0}, {970, 800, 0} };
	Math::AABB aabb3{ {1066, 737, 0}, {1600, 800, 0} };
	Math::AABB aabb4{ {375, 538, 0}, {505, 578, 0} };
	Math::AABB aabb5{ {745, 614, 0}, {872, 659, 0} };

	BackgroundState state = BackgroundState::Level1;

};