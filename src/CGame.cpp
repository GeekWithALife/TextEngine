#include "../include/CGame.hpp"
#include "SFont.hpp"

static Game *TheGame = NULL;

Game::Game() {
	shouldTerminate = false;
	timeLastFrame = 0;
	timeSinceLastFrame = 0.00001;
	timeStepConstant = 0.1;
	timeStepMin = 0.01;
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
static void keyboard(unsigned char key, int x, int y) {
	TheGame->Keyboard(key);
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
	glutInitContextVersion(2,0);
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
	
	printf("Adding callbacks...\n");
	
	// Add event listeners
	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	
	printf("Initializing FreeType...\n");
	
	// Initialize Fonts
	if (!Font::Initialize()) {
		return false;
	}
	
	printf("Initializing Canvas...\n");
	
	// Must be called after Font::Initialize.
	mainCanvas.SetSize(screenWidth/64, screenHeight/64);
	
	printf("Setup complete!\n");
	
	return true;
}
void Game::Start() {
	OnStart();
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
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
	
	Font::StartRender();
	Font::Render("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
	Font::Render("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);
	Font::StopRender();
	/*TextBuffer buf = canvas.GetBuffer();
	unsigned int width = 0, height = 0;
	//unsigned int screenWidth = 0, screenHeight = 0;
	buf.GetSize(width, height);
	float x = 0, y = 0;
	//printf("Drawing...\n");
	Font f;
	if (f.IsValid()) {
		f.Render("The Quick Brown Fox Jumps Over The Lazy Dog", -1 + 8 * sx, 1 - 50 * sy, sx, sy);
		f.Render("The Misaligned Fox Jumps Over The Lazy Dog", -1 + 8.5 * sx, 1 - 100.5 * sy, sx, sy);
	} else {
		printf("Failed to render with default font!\n");
	}
	for (unsigned int row = 0; row < height; row++) {
		x = -1 + 8 * sx;
		y = 1 - 50 * sy;
		for (unsigned int col = 0; col < width; col++) {
			Character ch = buf.GetCharacter(col, row);
			if (ch.GetChar() == 0)
				continue;
			Font f = ch.GetFont();
			if (!f.SetSize(48))
				continue;
			if (!f.LoadCharacter(ch.GetChar()))
				continue;
			printf("Drawing char %d:%d %c\n", col, row, ch.GetChar());
			//ch.GetColor(color);
		}
		x = 0;
		y = 0;
	}*/
    glutSwapBuffers();
    glutPostRedisplay();
}
void Game::Update() {
	float delta = (CurTime() - timeLastFrame);
	OnUpdate(delta);
	timeLastFrame = CurTime();
}
void Game::Keyboard(unsigned char key) {
	static char pressed[255] = { 0 };
	pressed[key] = !pressed[key];
	if (pressed[key])
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