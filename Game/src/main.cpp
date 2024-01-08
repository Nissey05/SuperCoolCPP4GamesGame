#include <Player.hpp>
#include <Level.hpp>
#include <Enemy.hpp>
#include <Utils.hpp>
#include <Hulkazoid.hpp>

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
#include <Math/Camera2D.hpp>

#include <glm/vec2.hpp>
#include <fmt/core.h>
#include <iostream>

using namespace Graphics;
using namespace Math;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float PLAYER_SPEED = 60.f;
const int CHAR_START_POS = SCREEN_WIDTH / 4 - 26;

std::shared_ptr<Player> player;
std::shared_ptr<Level> level;

Camera2D camera;
Window window;
Image image;

void InitGame() {
	player->init(level);
	player->setPosition({ CHAR_START_POS, SCREEN_HEIGHT / 2 - 32 });
	player->resetSpeed();
	player->setLives(6);
	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition({SCREEN_WIDTH /2, SCREEN_HEIGHT / 2});
	level->setLevelMap(LevelState::Start);
}

int main() {
	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	// Input to reload map
	Input::mapButton("Reload", [](std::span<const GamePadStateTracker> gamePadStates, const KeyboardStateTracker& keyboardState, const MouseStateTracker& mouseState) {
		bool y = false;

		for (auto& GamePadState : gamePadStates) {
			y = y || GamePadState.y == ButtonState::Pressed;
		}

		const bool r = keyboardState.isKeyPressed(KeyCode::R);
		const bool enter = keyboardState.isKeyPressed(KeyCode::Enter);

		return y || enter || r;
	});

	image.resize(SCREEN_WIDTH, SCREEN_HEIGHT);

	window.create(L"Wowzers", SCREEN_WIDTH, SCREEN_HEIGHT);
	window.show();

	player = std::make_shared<Player>(glm::vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
	level = std::make_shared<Level>(player);

	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition(player->getPosition());

	InitGame();

	// Game logic loop
	while (window) {
		timer.tick();

		// Allow game reload
		if (Input::getButton("Reload"))
			InitGame();

		// Update input and items
		Input::update();
		level->update(timer.elapsedSeconds());
		
		// Set camera position to player position
		camera.setPosition(player->getPosition() + glm::vec2(16, 16));

		// Prevent camera from exiting bounds
		glm::vec2 cameraCorrection{ 0 };
		if (camera.getLeftEdge() < 0)
			cameraCorrection.x = -camera.getLeftEdge();
		else if (camera.getRightEdge() > static_cast<float>(level->getWidth()))
			cameraCorrection.x = std::floor(static_cast<float>(level->getWidth()) - camera.getRightEdge());

		if (camera.getTopEdge() < 0)
			cameraCorrection.y = -camera.getTopEdge();
		else if (camera.getBottomEdge() > static_cast<float>(level->getHeight()))
			cameraCorrection.y = std::floor(static_cast<float>(level->getHeight()) - camera.getBottomEdge());

		camera.translate(cameraCorrection);

		// Clear screen
		image.clear(Color::Black);

		// Draw assets
		level->draw(image, camera);
		

		// Draw fps & lives and present
		image.drawText(Font::Default, fps, 10, 10, Color::White);
		std::string livesString = fmt::format("Lives: {}", static_cast<int>(player->getLives()));
		image.drawText(Font::Default, livesString, 10, 25, Color::White);
		std::string coinsString = fmt::format("Coins: {}", static_cast<int>(player->getCoins()));
		image.drawText(Font::Default, coinsString, 10, 40, Color::White);
		window.present(image);

		// Event loop
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
					break;
				}
				default:
					break;
			}
		}
		
		// Frame Counter, also calculates frames per second (fps)
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
