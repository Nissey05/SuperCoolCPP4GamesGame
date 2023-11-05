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

	void update(class Player& player, Math::Camera2D& camera);

	void resolveCollisionForLevel(class Player* player, Math::Camera2D& camera);

private:

	void setState(BackgroundState newState);
	BackgroundState getState();
	void doLevelOne();

	Graphics::Sprite WorldMap1;
	Graphics::Sprite WorldMap2;
	Graphics::Sprite WorldMap3;
	Graphics::Sprite StartScreen;
	Graphics::Sprite DeathScreen;

	std::vector<Math::AABB> aabbVec;
	BackgroundState state;
};
