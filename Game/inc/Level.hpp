#pragma once

#include <Graphics/Sprite.hpp>
#include <Math/AABB.hpp>
#include <Math/Camera2D.hpp>
#include <Graphics/Image.hpp>

enum class LevelState
{
	Level1,
	Level2,
	Start,
	Dead,
	Win
};

 // Class could be renamed to Level as it handles entities / other objects next to background too.
class Level {
public:
	//Default Constructor
	Level() = default;
	~Level() = default;

	explicit Level(std::shared_ptr<class Player> player);

	void draw(Graphics::Image& image, const Math::Camera2D& camera);
	void update(float deltaTime);
	void resolveCollisionForLevel(class Entity* entity);
	void resolveEntityCollision(class Entity* entity);
	
	int getWidth();
	int getHeight();

	void initLevelOne();
	void initLevelTwo();
	void resetAssets();

	LevelState getState();
	void setState(LevelState newState);

	void nextLevel();
	void setLevelMap(LevelState map);
	Graphics::Sprite& getLevelMap();

private:
	void drawAssets(Graphics::Image& image, const Math::Camera2D& camera);

	bool taken = false;

	Graphics::Sprite worldMap1;
	Graphics::Sprite worldMap2;
	Graphics::Sprite startScreen;
	Graphics::Sprite deathScreen;
	Graphics::Sprite winScreen;

	
	Math::AABB endAABB;
	std::vector<Math::AABB> aabbVec;
	std::vector<Math::AABB> deathBoxAABB;
	//std::vector<class Coin> coinList;
	std::vector<std::shared_ptr<class Entity>> entityList;

	LevelState state;
};
