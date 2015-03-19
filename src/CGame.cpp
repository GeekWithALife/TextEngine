#include "../include/CGame.hpp"
#include "SFont.hpp"
#include <ctype.h>

static Game *TheGame = NULL;

Game::Game() {
	shouldTerminate = false;
	timeLastFrame = 0;
	timeSinceLastFrame = 0.00001;
	timeStepConstant = 0.1;
	timeStepMin = 0.01;
	offsetX = 0;
	offsetY = 0;
	TheGame = this;
}
Game::~Game() {
	// Release Fonts
	Font::Release();
}
void Game::OnStart() { }
void Game::OnTerminate() { }
void Game::OnRender(Canvas& canvas) { }
void Game::OnUpdate(const float delta) { }
void Game::OnKeyDown(char key) { }
void Game::OnKeyUp(char key) { }
void Game::OnMouseDown(char button, unsigned int x, unsigned int y) { }
void Game::OnMouseUp(char button, unsigned int x, unsigned int y) { }

static void render() {
	TheGame->Render();
}
static void update() {
	TheGame->Update();
}
static void keyboarddown(unsigned char key, int x, int y) {
	TheGame->Keyboard(key, true);
}
static void keyboardup(unsigned char key, int x, int y) {
	TheGame->Keyboard(key, false);
}

float Game::CurTime() {
	return (glutGet(GLUT_ELAPSED_TIME) / 1000.0);
}

// Runtime Management
bool Game::Setup(int argc, char **argv, std::string title, unsigned int scrWidth, unsigned int scrHeight) {
	screenWidth = scrWidth;
	screenHeight = scrHeight;
	
	printf("Initializing GLUT...\n");
	
	// Initialize GLUT
	glutInit(&argc, argv);
	//glutInitContextVersion(2,0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// Ask GLUT to for a double buffered, full color window without a depth buffer
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow(title.c_str()); // set window title to executable name
	
	printf("Initializing GLEW...\n");
	
	// Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(glewError));
		return false;
	}
	
	
	printf("Initializing State...\n");
	
	// Initialize key state
	for (int i = 0; i < 256; i++) {
		char ch = i;
		printf("Setting %c\n", ch);
		keyState[ch] = false;
	}
	printf("Adding callbacks...\n");
	
	// Add event listeners
	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboarddown);
	glutKeyboardUpFunc(keyboardup);
	
	printf("Initializing FreeType...\n");
	
	// Initialize Fonts
	if (!Font::Initialize()) {
		return false;
	}
	
	return true;
}
void Game::Start() {
	OnStart();
	
	printf("Initializing Canvas...\n");
	
	// Must be called after Font::Initialize.
	//unsigned int w, h;
	//Font::GetCharDimensions('j', w, h);
	//printf("Char size: %d %d\n", w, h);
	int cols = screenWidth/Font::curPointSize;
	int rows = screenHeight/Font::curPointSize;
	mainCanvas.SetSize(cols, rows);
	
	int trueWidth = cols * Font::curPointSize;
	int trueHeight = rows * Font::curPointSize;
	
	offsetX = (screenWidth - trueWidth) / 2.0;
	offsetY = (screenHeight - trueHeight) / 2.0;
	
	printf("Setup complete!\n");
	
	timeLastFrame = CurTime();
	glutMainLoop();
	OnTerminate();
}
void Game::Terminate() {
	glutLeaveMainLoop();
}

void Game::Render() {
	Canvas canvas = mainCanvas;
	OnRender(canvas);
	
	// Draw everything
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
	
	Font::StartRender();
	TextBuffer buf = canvas.GetBuffer();
	unsigned int width = 0, height = 0;
	buf.GetSize(width, height);
	unsigned int w = 0, h = 0;
	float x = offsetX, y = offsetY + Font::curPointSize;
	/*if (Font::GetCharDimensions('j', w, h)) {
		x = w/2.0;
		y = h/2.0;
	}*/
	for (unsigned int row = 0; row < height; row++) {
		std::string line = buf.GetLine(row);
		Font::Render(line.c_str(), -1 + x * sx, 1 - y * sy, sx, sy);
		//for (unsigned int col = 0; col < width; col++) {
		//	printf("Drawing char %d:%d %c\n", col, row, ch.GetChar());
			//ch.GetColor(color);
		//}
		y += Font::curPointSize;
	}
	Font::StopRender();
	
    glutSwapBuffers();
    glutPostRedisplay();
}
void Game::Update() {
	float delta = (CurTime() - timeLastFrame);
	OnUpdate(delta);
	timeLastFrame = CurTime();
}
bool Game::IsKeyDown(unsigned char key) {
	key = tolower(key);
	return keyState[key];
}
// TODO: Learn the keyboard callback behavior on different systems. It's apparently window system dependent.
void Game::Keyboard(unsigned char key, bool state) {
	key = tolower(key);
	if (keyState[key] == state)
		return;
	keyState[key] = state;
	if (keyState[key])
		OnKeyDown(key);
	else
		OnKeyUp(key);
}

// Callbacks
/*virtual void Game::OnStart() {}
virtual void Game::OnTerminate() {}
virtual void Game::OnRender() {}
virtual void Game::OnUpdate(const float delta) {}
virtual void Game::OnKeyDown(char key) {}
virtual void Game::OnKeyUp(char key) {}
virtual void Game::OnMouseDown(char button, unsigned int x, unsigned int y) {}
virtual void Game::OnMouseUp(char button, unsigned int x, unsigned int y) {}*/