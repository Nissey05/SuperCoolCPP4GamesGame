#include <Player.hpp>

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
Sprite background;
Camera2D camera;



const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
int CHAR_START_POS = SCREEN_WIDTH / 4 - 26;

Player player;

float Player_speed = 60.f;

void InitGame() {
	player.setPosition({ CHAR_START_POS, SCREEN_HEIGHT / 2 - 32 });
	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition(player.getPosition());
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

	
	player = Player({SCREEN_WIDTH/2, SCREEN_HEIGHT/2}, &background);
	camera.setSize({ SCREEN_WIDTH, SCREEN_HEIGHT });
	camera.setPosition(player.getPosition());

	auto groundback = ResourceManager::loadImage("assets/Pinky/WorldMap1.png");
	background = Sprite( groundback );


	//Load tilemap.
	auto grass_sprites = ResourceManager::loadSpriteSheet("assets/pixelart/TX Tileset Grass.png", 16, 16);
	grassTiles = TileMap(grass_sprites, 30, 30);

	for (int i = 0; i < 30; ++i) {
		for (int j = 0; j < 30; ++j) {
			grassTiles(i, j) = (i * grass_sprites->getNumColumns() + j) % grass_sprites->getNumSprites();
		}
	}


	Timer       timer;
	double      totalTime = 0.0;
	uint64_t    frameCount = 0ull;
	std::string fps = "FPS: 0";

	InitGame();

	while (window) {

		// Update loop
		Input::update();
		
		//if (Input::getButton("Sprint")) {
		//	pos.x += Input::getAxis("HorizontalRun") * Player_speed * timer.elapsedSeconds();
		//	//pos.y -= Input::getAxis("VerticalRun") * Player_speed * timer.elapsedSeconds();	
		//}
		//else {
		//	pos.x += Input::getAxis("Horizontal") * Player_speed * timer.elapsedSeconds();
		//	//pos.y -= Input::getAxis("Vertical") * Player_speed * timer.elapsedSeconds();
		//}

		player.update(timer.elapsedSeconds());
		//collisions

		/*{
			auto aabb = player.getAABB();
			glm::vec2 correction{ 0 };
			if (aabb.min.x < 0) {
				correction.x = -aabb.min.x;
			}
			if (aabb.min.y < 0) {
				correction.y = -aabb.min.y;
			}
			if (aabb.max.x >= image.getWidth()) {
				correction.x = image.getWidth() - aabb.max.x;
			}
			if (aabb.max.y >= image.getHeight()) {
				correction.y = image.getHeight() - aabb.max.y;
			}


			player.translate(correction);
		}*/
		camera.setPosition(player.getPosition() + glm::vec2(16, 16));

		glm::vec2 cameraCorrection{ 0 };
		if (camera.getLeftEdge() < 0)
		{
			cameraCorrection.x = -camera.getLeftEdge();
		}
		else if (camera.getRightEdge() > static_cast<float>(background.getWidth()))
		{
			cameraCorrection.x = std::floor(static_cast<float>(background.getWidth()) - camera.getRightEdge());
		}

		if (camera.getTopEdge() < 0)
		{
			cameraCorrection.y = -camera.getTopEdge();
		}
		else if (camera.getBottomEdge() > static_cast<float>(background.getHeight()))
		{
			cameraCorrection.y = std::floor(static_cast<float>(background.getHeight()) - camera.getBottomEdge());
		}

		camera.translate(cameraCorrection);
		

		
		//if (pos.y >= SCREEN_HEIGHT - 37)  pos.y = SCREEN_HEIGHT - 37;
		//else if (pos.y < SCREEN_HEIGHT - 37) pos.y++;


		if (Input::getButton("Reload")) {
			InitGame();
		}


		//Render loop
		image.clear(Color::Black);

		/*if (pos.x <= SCREEN_WIDTH / 2 - 26 + 1) {
			grassTiles.draw(image, 0, 0);
			image.drawSprite(idleAnim, pos.x, pos.y);
		}
		else {
			grassTiles.draw(image, -pos.x + SCREEN_WIDTH / 2 - 26 + 1, 0);
			image.drawSprite(player, (SCREEN_WIDTH / 2) - 26, pos.y);
		}*/
		



		//grassTiles.draw(image, camera);

		image.drawSprite(background, camera);

		player.draw(image, camera);

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
