#include "../include/CGame.hpp"

static Game *TheGame = NULL;

static GLuint CreateShader(std::string vertexSource, std::string fragmentSource) {
	//Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 
	//Send the vertex shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	const GLchar *source = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	 
	//Compile the vertex shader
	glCompileShader(vertexShader);
	 
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the shader anymore.
		glDeleteShader(vertexShader);
	 
		//Use the infoLog as you see fit.
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	//Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	 
	//Send the fragment shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	 
	//Compile the fragment shader
	glCompileShader(fragmentShader);
	 
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		//Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);
	 
		//Use the infoLog as you see fit.
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	GLuint program = glCreateProgram();
	 
	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	 
	//Link our program
	glLinkProgram(program);
	 
	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	 
		//Use the infoLog as you see fit.
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	
	return program;
}
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

	// Initialize Fonts
	if (!Font::Initialize()) {
		return false;
	}

	// Initialize GLUT
	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	// Ask GLUT to for a double buffered, full color window without a depth buffer
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screenWidth, screenHeight);
	glutCreateWindow(title.c_str()); // set window title to executable name
	
	// Initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(glewError));
		return false;
	}
	
	// Add event listeners
	glutIdleFunc(update);
	glutDisplayFunc(render);
	glutKeyboardFunc(keyboard);
	
	mainCanvas.SetSize(screenWidth, screenHeight);
	
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
	/*TextBuffer buf = c.GetBuffer();
	unsigned int width = 0, height = 0;
	//unsigned int screenWidth = 0, screenHeight = 0;
	buf.GetSize(width, height);
	int cellSize = TheGame->screenWidth / width;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			Character ch = buf.GetCharacter(x, y);
			int pointSize = get_point_size(cellSize, ch.GetFont());
		}
	}*/
	// Draw everything
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
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