#ifndef INC_TextEngine_Font
#define INC_TextEngine_Font

#include "NEngine.hpp"

class Font {
	public:
		Font(std::string fontName, unsigned int pointSize);
		~Font();
		
		//FT_Face GetFace(std::string fontName);
		//FT_Bitmap GetBitmap(std::string fontName, char32_t ch);
		
	private:
		static FT_Library lib;
};
#endif