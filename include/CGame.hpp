#ifndef INC_TextEngine_Game
#define INC_TextEngine_Game

#include "NEngine.hpp"
#include "CCanvas.hpp"
#include <unordered_map>

class Game {
	public:
		Game();
		virtual ~Game();
		
		// Internal callers
		void Render();
		void Update();
		void Keyboard(unsigned char key, bool state);
		
		// Callbacks
		virtual void OnStart();
		virtual void OnTerminate();
		virtual void OnRender(Canvas& canvas);
		virtual void OnUpdate(const float delta);
		virtual void OnKeyDown(char key);
		virtual void OnKeyUp(char key);
		virtual void OnMouseDown(char button, unsigned int x, unsigned int y);
		virtual void OnMouseUp(char button, unsigned int x, unsigned int y);
		
		// Logic Utils
		bool IsKeyDown(unsigned char key);
		float CurTime();
		
		// Properties
		//void SetTitle(std::u32string title);
		//void SetResolution(unsigned int width, unsigned int height);
		
		// Runtime Management
		bool Setup(int argc, char **argv, std::string title, unsigned int scrWidth, unsigned int scrHeight);
		void Start();
		void Terminate();
		void Destroy();
		
	private:
		Canvas mainCanvas;
		bool shouldTerminate;
		unsigned int screenWidth;
		unsigned int screenHeight;
		float timeLastFrame;
		float timeSinceLastFrame;
		float timeStepConstant;
		float timeStepMin;
		
		float offsetX;
		float offsetY;
		std::unordered_map<char, bool> keyState;
};
#endif