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
		
		bool IsValid();
		bool SetSize(const unsigned int size);
		bool LoadFont(std::string fontName);
		bool LoadCharacter(const UnicodeChar ch);
		
		bool GetGlyph(FT_GlyphSlot& glyph);
		void DrawGlyph(float &x, float &y, const float sx, const float sy);
		
		static bool Initialize(std::string vertexFile, std::string fragmentFile);
		static bool Initialize();
		static void Release();
		
		static bool Exists(std::string name);
		static Font Get(std::string name);
		static void Cache(std::string name, Font font);
		static GLuint uniform_color;
		
	private:
		static FT_Library lib;
		static GLuint program;
		static GLuint tex;
		static GLuint vbo;
		
		static GLuint uniform_tex;
		static GLuint attribute_coord;
		
		static std::unordered_map<std::string, Font> fontCache;
		static std::unordered_map<std::string, FT_Face> faceCache;
		
		bool isValid;
		FT_Face face;
		std::string fontName;
		unsigned int pointSize;
};
#endif