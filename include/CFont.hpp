#ifndef INC_TextEngine_Font
#define INC_TextEngine_Font

#include "NEngine.hpp"
#include <unordered_map>

class Font {
	public:
		Font();
		Font(const std::string fontName);
		~Font();
		
		//FT_Face GetFace(std::string fontName);
		//FT_Bitmap GetBitmap(std::string fontName, UnicodeChar ch);
		
		// Configuration
		bool IsValid();
		bool SetSize(const unsigned int size);
		bool LoadFont(std::string fontName);
		bool LoadCharacter(const UnicodeChar ch);
		FT_GlyphSlot& GetGlyph();
		
		// Rendering
		static void BeginRender();
		void Render(float &x, float &y, const float sx, const float sy);
		void Render(const char *text, float x, float y, float sx, float sy);
		static void EndRender();
		
		// Setup/Teardown
		static bool Initialize(std::string vertexFile, std::string fragmentFile);
		static bool Initialize();
		static void Release();
		
		// Font Caching
		static bool Exists(std::string name);
		static Font Get(std::string name);
		static void Cache(std::string name, Font font);
		
		static GLuint program;
		static GLuint tex;
		static GLuint vbo;
		
		static GLuint uniform_tex;
		static GLuint attribute_coord;
		static GLuint uniform_color;
		
	private:
		static FT_Library lib;
		
		static std::unordered_map<std::string, Font> fontCache;
		static std::unordered_map<std::string, FT_Face> faceCache;
		
		bool isValid;
		FT_Face face;
		std::string fontName;
		unsigned int pointSize;
};
#endif