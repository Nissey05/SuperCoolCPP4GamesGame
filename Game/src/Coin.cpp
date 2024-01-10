#include <Coin.hpp>

#include <Graphics/SpriteAnim.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Level.hpp>

using namespace Graphics;

Coin::Coin(const glm::vec2& pos, Level* level)
	:	Entity::Entity("coin", "collectible", pos, {{5, 5, 0}, {17, 18, 0}}),
	level(level)
{
	auto idleSprites = ResourceManager::loadSpriteSheet("assets/Pinky/CoinAnim.png", 22, 24, 0, 0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim{ idleSprites, 6 };
	transform.setAnchor({ 11, 19 });
}

//Updates the coins animation
void Coin::update(float deltaTime)
{
	idleAnim.update(deltaTime);
}

//Draws the coin on the screen
void Coin::draw(Graphics::Image& image, const Math::Camera2D& camera)
{
	image.drawSprite(idleAnim, camera * transform);

//if DEBUG mode is enabled, draw AABB
#if _DEBUG
	image.drawAABB(camera * getAABB(), Color::Yellow, {}, FillMode::WireFrame);
#endif
}
