#include "../include/CFont.hpp"

FT_Library Font::lib;

Font::Font(std::string fontName, unsigned int pointSize) { }
Font::~Font() {}

//FT_Face Font::GetFace(std::string fontName) { return NULL; }
//FT_Bitmap Font::GetBitmap(std::string fontName, char32_t ch) { return NULL; }