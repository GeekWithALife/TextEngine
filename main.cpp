#include "include/NEngine.hpp"
#include "include/CGame.hpp"

class MyGame : public Game {
	void OnStart() {
		printf("Starting...\n");
	}
	void OnTerminate() {
		printf("Terminating...\n");
	}
	void OnRender(Canvas canvas) {
		printf("Rendering 1...\n");
		std::string strVal = "abc\ndef\nghi";
		TextBuffer buf(strVal);
		printf("Rendering 2...\n");
		canvas.Draw(buf, 0, 0);
		printf("Rendering 5...\n");
	}
	void OnUpdate(const float delta) {
		printf("Updated: %f %f\n", delta, CurTime());
	}
	void OnKeyDown(char key) {
		printf("Pressed %c\n", key);
		if (key == 'q')
			Terminate();
	}
	void OnKeyUp(char key) {
		printf("Released %c\n", key);
	}
	void OnMouseDown(char button, unsigned int x, unsigned int y) {}
	void OnMouseUp(char button, unsigned int x, unsigned int y) {}
};

int main(int argc, char **argv) {
	MyGame *game = new MyGame();
	if (game->Setup(argc, argv, "My Awesome Game", 640, 480))
		game->Start();
	delete game;
	return 0;
}