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
		//FT_Bitmap GetBitmap(std::string fontName, char32_t ch);
		
		bool IsValid();
		bool SetSize(const unsigned int size);
		bool LoadFont(std::string fontName);
		bool LoadCharacter(const char ch);
		bool LoadCharacter(const char32_t ch);
		
		bool GetGlyph(FT_GlyphSlot& glyph);
		void DrawGlyph(FT_GlyphSlot glyph, unsigned int &x, unsigned int &y, const float sx, const float sy);
		
		static bool Initialize(std::string vertexFile, std::string fragmentFile);
		static bool Initialize();
		static void Release();
		
	private:
		static FT_Library lib;
		static GLuint program;
		static GLuint tex;
		static GLuint vbo;
		static std::unordered_map<std::string, FT_Face> cache;
		
		bool isValid;
		FT_Face face;
		std::string fontName;
		unsigned int pointSize;
};
#endif