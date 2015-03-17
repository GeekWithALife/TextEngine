#ifndef INC_TextEngine_Engine
#define INC_TextEngine_Engine

#define TextEngine_NAMESPACE ASQII

// Standard
#include <string>
#include <vector>

// Libraries

// GLEW
#include <GL/glew.h>
#pragma comment(lib, "glew32.lib")

// GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_OUTLINE_H
#include FT_GLYPH_H
  
typedef std::u32string UnicodeString;
typedef char32_t UnicodeChar;

static UnicodeString delim (1, '\n');
static std::string font ("./fonts/cour.ttf");

namespace TextEngine_NAMESPACE {
	static UnicodeString DefaultDelimiter = delim;
	static std::string DefaultFont = font;
}

#endif