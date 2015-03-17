#include "../include/CGame.hpp"

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
void Game::OnRender(Canvas canvas) { }
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
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// Ask GLUT to for a double buffered, full color window without a depth buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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
	mainCanvas.SetSize(screenWidth, screenHeight);
	
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

// Calculates font size needed for a character using font fontName to fill a space of size cellSize.
static int get_point_size(int cellSize, Font font) {
	return 12;
}
void Game::Render() {
	Canvas c = TheGame->mainCanvas;
	TheGame->OnRender(c);
	// Draw everything
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	TextBuffer buf = c.GetBuffer();
	unsigned int width = 0, height = 0;
	//unsigned int screenWidth = 0, screenHeight = 0;
	buf.GetSize(width, height);
	float sx = 2.0 / glutGet(GLUT_WINDOW_WIDTH);
	float sy = 2.0 / glutGet(GLUT_WINDOW_HEIGHT);
	unsigned int x = 0, y = 0;
	printf("Drawing...");
	for (unsigned int row = 0; row < height; row++) {
		for (unsigned int col = 0; col < width; col++) {
			Character ch = buf.GetCharacter(col, row);
			if (ch.GetChar() == 0)
				continue;
			Font f = ch.GetFont();
			f.SetSize(12);
			f.LoadCharacter(ch.GetChar());
			float color[4] = {0};
			ch.GetColor(color);
			glUniform4fv(Font::uniform_color, 1, color);
			f.DrawGlyph(x, y, sx, sy);
		}
		x = 0;
		y = 0;
	}
	
    glutSwapBuffers();
    glutPostRedisplay();
}
void Game::Update() {
	float delta = (CurTime() - timeLastFrame);
	TheGame->OnUpdate(delta);
	timeLastFrame = CurTime();
}
void Game::Keyboard(unsigned char key) {
	static char pressed[255] = { 0 };
	pressed[key] = !pressed[key];
	if (pressed[key])
		TheGame->OnKeyDown(key);
	else
		TheGame->OnKeyUp(key);
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