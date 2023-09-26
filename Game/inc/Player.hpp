#pragma once


#include <Graphics/SpriteAnim.hpp>
#include <glm/vec2.hpp>


class Player {
public:
	//Default constructor
	Player();

	explicit Player(const glm::vec2& pos, const Graphics::SpriteAnim&);

	void update(float deltaTime);

	void draw(Graphics::Image& image);

	void setPosition(const glm::vec2& pos);
	const glm::vec2& getPosition() const;


private:
	glm::vec2 position;
	float Speed{ 60.0f };
	Graphics::SpriteAnim sprite;
};
