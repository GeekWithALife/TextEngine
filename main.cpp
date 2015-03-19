#include "include/NEngine.hpp"
#include "include/CGame.hpp"
#include "SFont.hpp"

class MyGame : public Game {
	int x = 0;
	int y = 0;
	float fX = 0;
	float fY = 0;
	void OnStart() {
		printf("Starting...\n");
		float color[4] = {1, 1, 1, 1};
		Font::SetColor(color);
		float colorBG[4] = {0, 0, 0, 1};
		Font::SetColorBG(colorBG);
		std::string family, style;
		if (!Font::LoadFace(TextEngine_NAMESPACE::DefaultFont, family, style))
			printf("Failed to load face.\n");
		else
			if (!Font::SetFace(family, style, 36))
				printf("Failed to set face.\n");
			else
				printf("We have font!\n");
	}
	void OnTerminate() {
		printf("Terminating...\n");
	}
	void OnRender(Canvas& canvas) {
		//canvas.Clear();
		std::string strVal = "abc\ndef\nghi\n";
		TextBuffer buf(strVal);
		canvas.Draw(buf, x, y);
	}
	int speed = 2;
	void OnUpdate(const float delta) {
		//printf("Updated: %f %f\n", delta, CurTime());
		if (IsKeyDown('w'))
			fY -= speed * delta;
		if (IsKeyDown('s'))
			fY += speed * delta;
		if (IsKeyDown('a'))
			fX -= speed * delta;
		if (IsKeyDown('d'))
			fX += speed * delta;
		x = fX;
		y = fY;
	}
	void OnKeyDown(char key) {
		printf("Pressed %c\n", key);
		if (IsKeyDown('w') || IsKeyDown('s'))
			fY = y;
		if (IsKeyDown('a') || IsKeyDown('d'))
			fX = x;
		switch(key){
			case 'q':
			case 27:
				Terminate();
				break;
			default:
				break;
		}
	}
	void OnKeyUp(char key) {
		printf("Released %c\n", key);
	}
	void OnMouseDown(char button, unsigned int x, unsigned int y) {}
	void OnMouseUp(char button, unsigned int x, unsigned int y) {}
};

int main(int argc, char **argv) {
	MyGame *game = new MyGame();
	if (game->Setup(argc, argv, "My Awesome Game", 1024, 768))
		game->Start();
	delete game;
	return 0;
}