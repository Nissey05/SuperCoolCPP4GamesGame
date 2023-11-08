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
	Background() = default;
	~Background() = default;

	explicit Background(class Player* player);

	int getWidth();
	int getHeight();
	Graphics::Sprite& getLevelMap();

	void setLevelMap(BackgroundState map);
	void draw(Graphics::Image& image, const Math::Camera2D& camera);

	void update(float deltaTime);

	BackgroundState getState();

	void resolveCollisionForLevel(class Entity* entity);
	//void resolveEnemyCollisionForLevel(class Enemy* enemy);

private:


	void setState(BackgroundState newState);
	
	void drawAssets(Graphics::Image& image, const Math::Camera2D& camera);

	Graphics::Sprite worldMap1;
	Graphics::Sprite worldMap2;
	Graphics::Sprite worldMap3;
	Graphics::Sprite startScreen;
	Graphics::Sprite deathScreen;

	class Player* player;

	std::vector<Math::AABB> aabbVec;
	std::vector < std::shared_ptr<class Enemy> > enemyVec;

	BackgroundState state;
};
