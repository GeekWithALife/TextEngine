class Game {
	public:
		Game();
		~Game();
		
		// Singleton
		static Game instance();
		
		// Properties
		//void SetTitle(std::u32string title);
		//void SetResolution(unsigned int width, unsigned int height);
		
		// Runtime Management
		void Initialize(std::string title, unsigned int screenWidth, unsigned int screenHeight);
		void Start();
		void Terminate();
		void Render();
		void Update(const float delta);
		
		// Callbacks
		virtual void OnStart();
		virtual void OnTerminate();
		virtual void OnRender();
		virtual void OnUpdate(const float delta);
		virtual void OnKeyDown(char key);
		virtual void OnKeyUp(char key);
		virtual void OnMouseDown(char button, unsigned int x, unsigned int y);
		virtual void OnMouseUp(char button, unsigned int x, unsigned int y);
		
	private:
		Canvas canvas;
		bool shouldTerminate;
};