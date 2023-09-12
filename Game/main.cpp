#include <Graphics/Window.hpp>

#include <iostream>

Graphics::Window window{};

int main() {

	window.create(L"Wowzers", 800, 600);

	window.show();

	while (window) {

		//Game Logic

		Graphics::Event e;
		while (window.popEvent(e)) {
			switch (e.type) {
			case Graphics::Event::Close:
				window.destroy();
				break;
			case Graphics::Event::KeyPressed:
			{
				switch (e.key.code)\
				{
				case Graphics::KeyCode::Escape:
					window.destroy();
					break;
				}

			}	break;
			
			}
		}
	}
	



	return 0;
}
