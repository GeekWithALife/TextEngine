#include "include/NEngine.hpp"
#include "include/CGame.hpp"
#include "SFont.hpp"

class MyGame : public Game {
	void OnStart() {
		printf("Starting...\n");
		float color[4] = {0, 0, 0, 1};
		Font::SetColor(color);
		std::string family, style;
		if (!Font::LoadFace(TextEngine_NAMESPACE::DefaultFont, family, style))
			printf("Failed to load face.\n");
		else
			if (!Font::SetFace(family, style, 48))
				printf("Failed to set face.\n");
			else
				printf("We have font!\n");
	}
	void OnTerminate() {
		printf("Terminating...\n");
	}
	void OnRender(Canvas& canvas) {
		//canvas.Clear();
		std::string strVal = "abc\ndef\nghi\njkl\n";
		TextBuffer buf(strVal);
		canvas.Draw(buf, 0, 0);
		
	}
	void OnUpdate(const float delta) {
		//printf("Updated: %f %f\n", delta, CurTime());
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
	if (game->Setup(argc, argv, "My Awesome Game", 640, 512))
		game->Start();
	delete game;
	return 0;
}