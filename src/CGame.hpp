class Game {
	public:
		Game();
		~Game();
		
		// Properties
		//void SetTitle(std::u32string title);
		//void SetResolution(unsigned int width, unsigned int height);
		
		// Runtime Management
		void Initialize(std::string title, unsigned int scrWidth, unsigned int scrHeight);
		void Start();
		void Terminate();
		void Render();
		void Update(const float delta);
		float CurTime();
		
		// Callbacks
		virtual void OnStart();
		virtual void OnTerminate();
		virtual void OnRender(Canvas canvas);
		virtual void OnUpdate(const float delta);
		virtual void OnKeyDown(char key);
		virtual void OnKeyUp(char key);
		virtual void OnMouseDown(char button, unsigned int x, unsigned int y);
		virtual void OnMouseUp(char button, unsigned int x, unsigned int y);
		
	private:
		Canvas mainCanvas;
		bool shouldTerminate;
		unsigned int screenWidth;
		unsigned int screenHeight;
		float timeLastFrame;
		float timeSinceLastFrame;
		float timeStepConstant;
		float timeStepMin;
};