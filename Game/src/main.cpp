#include <Player.hpp>
#include <Background.hpp>
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

Window window;
Image image;
TileMap grassTiles;
Camera2D camera;
Background background;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int CHAR_START_POS = SCREEN_WIDTH / 4 - 26;

Player player;

float Player_speed = 60.f;

void InitGame() {
	player.setPosition({ CHAR_START_POS, SCREEN_HEIGHT / 2 - 32 });
	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition({SCREEN_WIDTH /2, SCREEN_HEIGHT / 2});
	background.setLevelMap(BackgroundState::Start);
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

	background = Background(&player);
	player = Player({SCREEN_WIDTH/2, SCREEN_HEIGHT/2}, &background);
	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition(player.getPosition());

	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window) {

		timer.tick();

		// Update loop
		Input::update();
		
		player.update(timer.elapsedSeconds());
		background.update(timer.elapsedSeconds());
		//hulky.update(timer.elapsedSeconds());
		
		// Camera position
		camera.setPosition(player.getPosition() + glm::vec2(16, 16));

		glm::vec2 cameraCorrection{ 0 };
		if (camera.getLeftEdge() < 0)
			cameraCorrection.x = -camera.getLeftEdge();
		else if (camera.getRightEdge() > static_cast<float>(background.getWidth()))
			cameraCorrection.x = std::floor(static_cast<float>(background.getWidth()) - camera.getRightEdge());

		if (camera.getTopEdge() < 0)
			cameraCorrection.y = -camera.getTopEdge();
		else if (camera.getBottomEdge() > static_cast<float>(background.getHeight()))
			cameraCorrection.y = std::floor(static_cast<float>(background.getHeight()) - camera.getBottomEdge());

		camera.translate(cameraCorrection);

		if (Input::getButton("Reload")) 
			InitGame();

		/*glm::vec2 correction = Utils::GetCollisionCorrection(player.getAABB(), enemy.getAABB());
		player.translate(correction);

		if (glm::abs(correction.y) > 0.f) {
			player.setVelocityY(0.f);
			player.setAccelerationY(0.f);

		}
		if (glm::abs(correction.x) > 0.f) {
			player.setVelocityX(0.f);
			player.setAccelerationX(0.f);
			enemy.setVelocityX(0.f);
		}*/

		//Render loop
		image.clear(Color::Black);

		background.draw(image, camera);
		if(background.getState() == BackgroundState::Level1 
			|| background.getState() == BackgroundState::Level2 
			|| background.getState() == BackgroundState::Level3)
		player.draw(image, camera);

		


		//if (player.collides(hulky)) hulky.setVelocityX(100);
		

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
