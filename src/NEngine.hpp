#ifndef INC_TextEngine_Engine
#define INC_TextEngine_Engine

#define TextEngine_NAMESPACE ASQII

// Standard
#include <string>
#include <vector>

// Libraries

// GLEW
#include <GL/glew.h>

// GLUT
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

// Freetype
#include <ft2build.h>
#include FT_FREETYPE_H
  
typedef std::u32string UnicodeString;
typedef char32_t UnicodeChar;

static UnicodeString delim (1, '\n');
namespace TextEngine_NAMESPACE {
	static UnicodeString DELIMITER = delim;
}

#endif