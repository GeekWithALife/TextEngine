#ifndef INC_TextEngine_Font
#define INC_TextEngine_Font

#include "NEngine.hpp"
#include <unordered_map>

class Font {
	public:
		// Setup/Teardown
		static bool Initialize(std::string vertexFile, std::string fragmentFile);
		static bool Initialize();
		static void Release();
		
		// Font Caching
		static bool LoadFont(std::string folderName);
		static bool LoadFace(std::string fileName, std::string& familyName, std::string& styleName);
		static bool SetFace(std::string familyName, std::string styleName, unsigned int pointSize);
		static bool SetColor(float color[4]);
		
		// Rendering
		static void StartRender();
		static void Render(const char *text, float x, float y, float sx, float sy);
		static void StopRender();
		
		// State
		static FT_Library lib;
		static FT_Face curFace;
		static float curColor[4];
		static unsigned int curPointSize;
		
		// Cache - fontCache[familyName][styleName] = face;
		static std::unordered_map<std::string, std::unordered_map<std::string, FT_Face>> fontCache;
		
		// OpenGL
		static GLuint program;
		static GLuint tex;
		static GLuint vbo;
		static GLint uniform_tex;
		static GLint attribute_coord;
		static GLint uniform_color;
		
	private:
};

#endif