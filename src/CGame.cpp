void Game::Game() {
	canvas = new Canvas();
}
void Game::~Game() {
	delete canvas;
}

Game Game::instance() {
	if (Game::inst == NULL)
		Game::inst = new Game();
	return Game::inst;
}

// Runtime Management
void Game::Initialize(std::string title, unsigned int screenWidth, unsigned int screenHeight) {
	// Setup OpenGL, GLEW, and GLFW
	// Add event listeners
}
void Game::Start() {
	// Game Loop
	while (!shouldTerminate) {
		
	}
	OnTerminate();
}
void Game::Terminate() {
	shouldTerminate = true;
}
void Game::Render() {
	OnRender();
}
void Game::Update(const float delta) {
	OnUpdate(delta);
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