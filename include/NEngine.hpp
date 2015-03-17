#ifndef INC_TextEngine_Engine
#define INC_TextEngine_Engine

#define TextEngine_NAMESPACE ASQII

// Standard
#include <stdio.h>
#include <string>
#include <vector>

// Libraries
#include "utf8/utf8.h"

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

typedef std::string UnicodeString;
typedef char UnicodeChar;
//typedef std::u32string UnicodeString;
//typedef char32_t UnicodeChar;

static UnicodeString delim (1, '\n');
static std::string font ("./fonts/cour.ttf");
static std::string shader ("./shaders/text");

namespace TextEngine_NAMESPACE {
	static UnicodeString DefaultDelimiter = delim;
	static std::string DefaultFont = font;
	static std::string DefaultShader = shader;
}

#endif