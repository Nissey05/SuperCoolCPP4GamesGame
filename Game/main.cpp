#include <Graphics/Window.hpp>
#include <Graphics/Image.hpp>
#include <Graphics/Sprite.hpp>
#include <Graphics/SpriteAnim.hpp>
#include <Graphics/Timer.hpp>
#include <Graphics/Font.hpp>
#include <Graphics/ResourceManager.hpp>
#include <Graphics/Input.hpp>
#include <Graphics/TileMap.hpp>
#include <Math/Transform2D.hpp>

#include <glm/vec2.hpp>

#include <fmt/core.h>

#include <iostream>

using namespace Graphics;

Window window;
Image image;
Sprite sprite;
SpriteAnim idleAnim;
TileMap grassTiles;


const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int CHAR_START_POS = SCREEN_WIDTH / 2 - 26;

Math::Transform2D Player_Transform;

float Player_speed = 60.f;

void InitGame() {
	Player_Transform.setPosition({ CHAR_START_POS, SCREEN_HEIGHT/2 - 32});
}


int main() {


	//input to reload map

	Input::mapButton("Reload", [](std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState) {
		bool b = false;

	for (auto& GamePadState : gamePadStates) {
		b = b || GamePadState.b == ButtonState::Pressed;
	}


	const bool r = keyboardState.isKeyPressed(KeyCode::R);
	const bool enter = keyboardState.isKeyPressed(KeyCode::Enter);

	return b || enter || r;
		});


	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"Wowzers", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();

	Player_Transform.setAnchor({ 16, 32 });


	auto idle_sprites = ResourceManager::loadSpriteSheet("assets/Spirit Boxer/Idle.png", 137, 44, 0 ,0, BlendMode::AlphaBlend);
	idleAnim = SpriteAnim(idle_sprites, 6);


	//Load tilemap.
	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/pixelart/TX Tileset Grass.png", 137, 44);
	grassTiles = TileMap(grass_sprites, 30, 30);

	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			grassTiles(i, j) = (i + j) % grass_sprites->getNumSprites();
		}
	}
	

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window) {

		// Update loop
//		auto keyState = Keyboard::getState();
		Input::update();

		auto pos = Player_Transform.getPosition();
		if (Input::getKey(KeyCode::LeftShift)) {
			pos.x += Input::getAxis("HorizontalRun") * Player_speed * timer.elapsedSeconds();
			pos.y -= Input::getAxis("VerticalRun") * Player_speed * timer.elapsedSeconds();
		}
		else {
			pos.x += Input::getAxis("Horizontal") * Player_speed * timer.elapsedSeconds();
			pos.y -= Input::getAxis("Vertical") * Player_speed * timer.elapsedSeconds();
		}
		Player_Transform.setPosition(pos);


		if (Input::getButton("Reload")) {
			InitGame();
		}

		if (Input::getButton("Jump")) {
			std::cout << "Jump!" << std::endl;
		}


		auto rot = Player_Transform.getRotation();
		if (Input::getKey(KeyCode::Q))
		{
			rot += 10.0 * timer.elapsedSeconds();
		}
		if (Input::getKey(KeyCode::E))
		{
			rot -= 10.0 * timer.elapsedSeconds();
		}
		Player_Transform.setRotation(rot);
		


		idleAnim.update(timer.elapsedSeconds());

		//Render loop
		image.clear(Color::Black);

		if (pos.x <= CHAR_START_POS + 1) {
			grassTiles.draw(image, 0, 0);
			image.drawSprite(idleAnim, pos.x, SCREEN_HEIGHT / 2 - 32);
		}
		else {
			grassTiles.draw(image, -pos.x + CHAR_START_POS + 1, 0);
			image.drawSprite(idleAnim, (SCREEN_WIDTH / 2) - 26, SCREEN_HEIGHT / 2 - 32);
		}

		

		

		image.drawText(Font::Default, fps, 10, 10, Color::White);

		window.present(image);

		Event e;
		while (window.popEvent(e)) {
			switch (e.type) {
			case Event::Close:
				window.destroy();
				break;
			case Event::KeyPressed:
			{
				switch (e.key.code)
				{
				case KeyCode::Escape:
					window.destroy();
					break;
				case KeyCode::F:
					window.toggleVSync();
					break;
				}
			

			}	break;

			}
		}


		timer.tick();
		++frameCount;

		totalTime += timer.elapsedSeconds();
		if (totalTime > 1.0)
		{
			fps = fmt::format("FPS: {:.3f}", static_cast<double>(frameCount) / totalTime);

			std::cout << fps << std::endl;

			frameCount = 0;
			totalTime = 0.0;
		}
	}


	std::cout << "Thank you for playing!" << std::endl;

	return 0;
}
