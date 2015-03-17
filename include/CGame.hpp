#ifndef INC_TextEngine_Game
#define INC_TextEngine_Game

#include "NEngine.hpp"
#include "CCanvas.hpp"
#include "CFont.hpp"

class Game {
	public:
		Game();
		virtual ~Game();
		
		// Callbacks
		virtual void OnStart();
		virtual void OnTerminate();
		virtual void OnRender(Canvas canvas);
		virtual void OnUpdate(const float delta);
		virtual void OnKeyDown(char key);
		virtual void OnKeyUp(char key);
		virtual void OnMouseDown(char button, unsigned int x, unsigned int y);
		virtual void OnMouseUp(char button, unsigned int x, unsigned int y);
		
		// Properties
		//void SetTitle(std::u32string title);
		//void SetResolution(unsigned int width, unsigned int height);
		
		// Runtime Management
		bool Setup(int argc, char **argv, std::string title, unsigned int scrWidth, unsigned int scrHeight);
		void Start();
		void Terminate();
		void Destroy();
		
		void Render();
		void Update();
		void Keyboard(unsigned char key);
		float CurTime();
		
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
#endif