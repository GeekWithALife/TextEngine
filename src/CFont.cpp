#include "../include/CFont.hpp"

FT_Library Font::lib;
std::unordered_map<std::string, FT_Face> Font::cache;
GLuint Font::program = 0;
GLuint Font::tex = 0;
GLuint Font::vbo = 0;
	
bool Font::Initialize(std::string vertexSource, std::string fragmentSource) {
	if (FT_Init_FreeType(&Font::lib)) {
		fprintf(stderr, "Could not initialize the FreeType library\n");
		return false;
	}
	
	/*Font::program = CreateShader(vertexSource, fragmentSource);
	
	if (Font::program <= 0) {
		fprintf(stderr, "Could not initialize the shader\n");
		return false;
	}
	
	GLuint uniform_tex = glGetUniformLocation(Font::program, "tex");
	GLuint uniform_color = glGetUniformLocation(Font::program, "color");
	GLuint attribute_coord = glGetAttribLocation(Font::program, "coord");
	
	// Before we can start rendering text, there are still some things that need initialization.
	// First, we will use a single texture object to render all the glyphs:
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &Font::tex);
	glBindTexture(GL_TEXTURE_2D, Font::tex);
	glUniform1i(uniform_tex, 0);

	// To prevent certain artifacts when a character is not rendered exactly on pixel boundaries, 
	// we should clamp the texture at the edges, and enable linear interpolation:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// It is also very important to disable the default 4-byte alignment restrictions that OpenGL 
	// uses for uploading textures and other data. Normally you won't be affected by this restriction, 
	// as most textures have a width that is a multiple of 4, and/or use 4 bytes per pixel. 
	// The glyph images are in a 1-byte greyscale format though, and can have any possible width. 
	// To ensure there are no alignment restrictions, we have to use this line:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	// We also need to set up a vertex buffer object for our combined vertex and texture coordinates:
	glGenBuffers(1, &Font::vbo);
	glEnableVertexAttribArray(attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, Font::vbo);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);*/
	
	return true;
}

bool Font::Initialize() {
	return Font::Initialize("shaders/text.vert", "shaders/text.frag");
}

void Font::Release() {
	//glDeleteBuffers(1, &Font::vbo);
	//glDeleteTextures(1, &Font::tex);
	// Release FreeType
	FT_Done_FreeType(Font::lib);
}
	
Font::Font() {
	pointSize = 12;
	isValid = LoadFont(TextEngine_NAMESPACE::DefaultFont);
	face = {0};
}
Font::Font(const std::string name) {
	pointSize = 12;
	isValid = LoadFont(name);
	face = {0};
}
Font::~Font() {}

bool Font::LoadFont(const std::string name) {
	if (Font::cache.find(name) == Font::cache.end()) {
		//printf("Initializing new font! %p\n", Font::cache[name]);
		int err = 0;
		if (err = FT_New_Face(Font::lib, name.c_str(), 0, &face)) {
			fprintf(stderr, "Error 0x%04x: Could not open font\n", err);
			return false;
		}
		printf("Initialized new font: %s\n", face->family_name);
		Font::cache[name] = face;
	}
	//printf("Found font in cache!\n");
	face = Font::cache[name];
	fontName = name;
	return true;
}

bool Font::IsValid() {
	return isValid;
}

// State Management
bool Font::SetSize(const unsigned int size) {
	if (!IsValid())
		return false;
	pointSize = pointSize;
	if (FT_Set_Pixel_Sizes(face, 0, pointSize)) {
		fprintf(stderr, "Could not resize font\n");
		return false;
	}
	return true;
}
bool Font::LoadCharacter(const char ch) {
	if (!IsValid())
		return false;
	if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character '%c'\n", ch);
		return false;
	}
	return true;
}
bool Font::LoadCharacter(const char32_t ch) {
	if (!IsValid())
		return false;
	if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character '%c'\n", ch);
		return false;
	}
	return true;
}

bool Font::GetGlyph(FT_GlyphSlot& glyph) {
	if (!IsValid())
		return false;
	glyph = face->glyph;
	return true;
}

void Font::DrawGlyph(FT_GlyphSlot glyph, unsigned int &x, unsigned int &y, const float sx, const float sy) {
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      glyph->bitmap.width,
      glyph->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      glyph->bitmap.buffer
    );
 
    float x2 = x + glyph->bitmap_left * sx;
    float y2 = -y - glyph->bitmap_top * sy;
    float w = glyph->bitmap.width * sx;
    float h = glyph->bitmap.rows * sy;
 
    GLfloat box[4][4] = {
        {x2,     -y2    , 0, 0},
        {x2 + w, -y2    , 1, 0},
        {x2,     -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };
 
    //glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
    //glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
    x += (glyph->advance.x >> 6) * sx;
    y += (glyph->advance.y >> 6) * sy;
}
/*
static void render_text(const char *text, float x, float y, float sx, float sy) {
  const char *p;
 
	for(p = text; *p; p++) {
		if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
			continue;
 
  }
}*/
//FT_Face Font::GetFace(std::string fontName) { return NULL; }
//FT_Bitmap Font::GetBitmap(std::string fontName, char32_t ch) { return NULL; }