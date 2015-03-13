void Game::Game() {
	mainCanvas = new Canvas();
	shouldTerminate = false;
	timeLastFrame = 0;
	timeSinceLastFrame = 0.00001;
	timeStepConstant = 0.1;
	timeStepMin = 0.01;
}
void Game::~Game() {
	delete mainCanvas;
}

float Game::CurTime() {
	return 0.1;
}

// Runtime Management
void Game::Initialize(std::string title, unsigned int scrWidth, unsigned int scrHeight) {
	screenWidth = scrWidth;
	screenHeight = scrHeight;
	// Setup OpenGL, GLEW, and GLFW
	// Add event listeners
}
void Game::Start() {
	// Game Loop
	while (!shouldTerminate) {
		timeSinceLastFrame = CurTime() - timeLastFrame;
		// Don't update too fast.
		if (timeSinceLastFrame < timeStepMin)
			continue;
		// Play catchup with uniform timestep
		if (timeSinceLastFrame > timeStepConstant) {
			int timeSteps = timeSinceLastFrame / timeStepConstant;
			while (timeSteps > 0) {
				timeSteps--;
				Update(1/timeStepConstant);
			}
			float lastDelta = timeSinceLastFrame - timeStepConstant * timeSteps;
			if (lastDelta > 0.00001)
				Update(1/lastDelta);
		// Update with variable timestep
		} else {
			Update(1/timeSinceLastFrame);
		}
		// Render as fast as we can.
		Render();
		timeLastFrame = CurTime();
	}
	OnTerminate();
}
void Game::Terminate() {
	shouldTerminate = true;
}

// Calculates font size needed for a character using font fontName to fill a space of size cellSize.
static int get_point_size(int cellSize, std::string fontName) {
	return 12;
}
void Game::Render() {
	OnRender(mainCanvas);
	TextBuffer buf = mainCanvas.GetBuffer();
	unsigned int width = 0, height = 0;
	unsigned int screenWidth = 0, sc
	buf.GetSize(width, height);
	int cellSize = screenWidth / width;
	for (unsigned int y = 0; y < height; y++) {
		for (unsigned int x = 0; x < width; x++) {
			Character ch = buf.GetCharacter(x, y);
			int pointSize = get_point_size(cellSize, ch.GetFont());
		}
	}
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