#include "../include/CFont.hpp"
#include <iostream>
#include <fstream>

FT_Library Font::lib;
std::unordered_map<std::string, Font> Font::fontCache;
std::unordered_map<std::string, FT_Face> Font::faceCache;
GLuint Font::program = 0;
GLuint Font::tex = 0;
GLuint Font::vbo = 0;
GLuint Font::uniform_tex = 0;
GLuint Font::uniform_color = 0;
GLuint Font::attribute_coord = 0;

#define printOpenGLError() printOglError(__FILE__, __LINE__)
int printOglError(char *file, int line)
{

    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s\n",
			     file, line, gluErrorString(glErr));
        retCode = 1;
    }
    return retCode;
}
static GLuint CreateShader(std::string vertexSource, std::string fragmentSource) {
	GLuint program = 0;
	printf("Creating vertex shader...\n");
	//Create an empty vertex shader handle
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	 
	printf("Setting source...\n");
	//Send the vertex shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	const GLchar *source = (const GLchar *)vertexSource.c_str();
	glShaderSource(vertexShader, 1, &source, 0);
	 
	printf("Compiling...\n");
	//Compile the vertex shader
	glCompileShader(vertexShader);
	 
	printf("Checking...\n");
	GLint isCompiled = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the shader anymore.
		glDeleteShader(vertexShader);
	 
		//Use the infoLog as you see fit.
		printf("Failed to compile vertex shader!\n");
		for (unsigned int i = 0; i < infoLog.size(); ++i)
			printf("%c", infoLog[i]);
		printf("\n");
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	printf("Creating fragment shader...\n");
	//Create an empty fragment shader handle
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	 
	printf("Setting source...\n");
	//Send the fragment shader source code to GL
	//Note that std::string's .c_str is NULL character terminated.
	source = (const GLchar *)fragmentSource.c_str();
	glShaderSource(fragmentShader, 1, &source, 0);
	 
	printf("Compiling...\n");
	//Compile the fragment shader
	glCompileShader(fragmentShader);
	 
	printf("Checking...\n");
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
	if(isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the shader anymore.
		glDeleteShader(fragmentShader);
		//Either of them. Don't leak shaders.
		glDeleteShader(vertexShader);
	 
		//Use the infoLog as you see fit.
		printf("Failed to compile fragment shader!\n");
		for (unsigned int i = 0; i < infoLog.size(); ++i)
			printf("%c", infoLog[i]);
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	printf("Creating shader program...\n");
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	program = glCreateProgram();
	 
	printf("Attaching shaders...\n");
	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	 
	printf("Linking...\n");
	//Link our program
	glLinkProgram(program);
	 
	printf("Checking...\n");
	//Note the different functions here: glGetProgram* instead of glGetShader*.
	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if(isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
	 
		//The maxLength includes the NULL character
		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
	 
		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	 
		//Use the infoLog as you see fit.
		printf("Failed to link!\n");
	 
		//In this simple program, we'll just leave
		return 0;
	}
	 
	printf("Detaching shaders...\n");
	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	
	printf("Returning...\n");
	return program;
}
static void ReadShaderFile(std::string fileName, std::string& source) {
	std::ifstream is (fileName, std::ifstream::binary);
	if (is) {
		// get length of file:
		is.seekg (0, is.end);
		int length = is.tellg();
		is.seekg (0, is.beg);

		char * buffer = new char [length];

		std::cout << "Reading " << length << " characters... " << std::endl;
		// read data as a block:
		is.read (buffer,length);

		if (is)
		  std::cout << "all characters read successfully." << std::endl;
		else
		  std::cout << "error: only " << is.gcount() << " could be read" << std::endl;
		is.close();
		
		source.assign(buffer);
		
		delete[] buffer;
	}
}
bool Font::Initialize(std::string vertexFile, std::string fragmentFile) {
	if (FT_Init_FreeType(&Font::lib)) {
		fprintf(stderr, "Could not initialize the FreeType library\n");
		return false;
	}
	
	std::string vertexSource;
	std::string fragmentSource;
	ReadShaderFile(vertexFile, vertexSource);
	ReadShaderFile(fragmentFile, fragmentSource);
	
	Font::program = CreateShader(vertexSource, fragmentSource);
	
	if (Font::program == 0) {
		fprintf(stderr, "Could not initialize the shader\n");
		return false;
	}
	
	glUseProgram(Font::program);
	
	Font::uniform_tex = glGetUniformLocation(Font::program, "tex");
	if (Font::uniform_tex < 0) {
		fprintf(stderr, "Could not bind texture uniform\n");
		return false;
	}
	Font::uniform_color = glGetUniformLocation(Font::program, "color");
	if (Font::uniform_color < 0) {
		fprintf(stderr, "Could not bind color uniform\n");
		return false;
	}
	Font::attribute_coord = glGetAttribLocation(Font::program, "coord");
	if (Font::attribute_coord < 0) {
		fprintf(stderr, "Could not bind coord attribute\n");
		return false;
	}
	
	// Before we can start rendering text, there are still some things that need initialization.
	// First, we will use a single texture object to render all the glyphs:
	glActiveTexture(GL_TEXTURE0);
	printOpenGLError();
	glGenTextures(1, &Font::tex);
	printOpenGLError();
	glBindTexture(GL_TEXTURE_2D, Font::tex);
	printOpenGLError();
	glUniform1i(Font::uniform_tex, 0);
	printOpenGLError();

	// To prevent certain artifacts when a character is not rendered exactly on pixel boundaries, 
	// we should clamp the texture at the edges, and enable linear interpolation:
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	printOpenGLError();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	printOpenGLError();

	// It is also very important to disable the default 4-byte alignment restrictions that OpenGL 
	// uses for uploading textures and other data. Normally you won't be affected by this restriction, 
	// as most textures have a width that is a multiple of 4, and/or use 4 bytes per pixel. 
	// The glyph images are in a 1-byte greyscale format though, and can have any possible width. 
	// To ensure there are no alignment restrictions, we have to use this line:
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	printOpenGLError();
	
	// We also need to set up a vertex buffer object for our combined vertex and texture coordinates:
	glGenBuffers(1, &Font::vbo);
	printOpenGLError();
	glEnableVertexAttribArray(Font::attribute_coord);
	printOpenGLError();
	glBindBuffer(GL_ARRAY_BUFFER, Font::vbo);
	printOpenGLError();
	glVertexAttribPointer(Font::attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	printOpenGLError();
	
	//Font::EndRender();
	
	return true;
}

bool Font::Initialize() {
	return Font::Initialize("./shaders/text.vert", "./shaders/text.frag");
}

void Font::Release() {
	glDeleteProgram(Font::program);
	glDeleteBuffers(1, &Font::vbo);
	glDeleteTextures(1, &Font::tex);
	// Release FreeType
	FT_Done_FreeType(Font::lib);
}

void Font::BeginRender() {
	/*glUseProgram(Font::program);
	printOpenGLError();
	glBindTexture(GL_TEXTURE_2D, Font::tex);
	printOpenGLError();
	glBindBuffer(GL_ARRAY_BUFFER, Font::vbo);
	printOpenGLError();
	glEnableVertexAttribArray(Font::attribute_coord);
	printOpenGLError();
	glUniform1i(Font::uniform_tex, Font::tex);
	printOpenGLError();*/
}
void Font::Render(const char *text, float x, float y, float sx, float sy) {
  const char *p;
  for(p = text; *p; p++) {
    if(FT_Load_Char(face, *p, FT_LOAD_RENDER))
        continue;
	FT_GlyphSlot g = GetGlyph();
 
    glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RED,
      g->bitmap.width,
      g->bitmap.rows,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      g->bitmap.buffer
    );
	printOpenGLError();
 
    float x2 = x + g->bitmap_left * sx;
    float y2 = -y - g->bitmap_top * sy;
    float w = g->bitmap.width * sx;
    float h = g->bitmap.rows * sy;
 
    GLfloat box[4][4] = {
        {x2,     -y2    , 0, 0},
        {x2 + w, -y2    , 1, 0},
        {x2,     -y2 - h, 0, 1},
        {x2 + w, -y2 - h, 1, 1},
    };
 
    glBufferData(GL_ARRAY_BUFFER, sizeof box, box, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	printOpenGLError();
 
    x += (g->advance.x >> 6) * sx;
    y += (g->advance.y >> 6) * sy;
  }
}

void Font::Render(float &x, float &y, const float sx, const float sy) {
	FT_GlyphSlot glyph = GetGlyph();
	
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
	printOpenGLError();
 
    glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
	printOpenGLError();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	printOpenGLError();
	
	//printf("x2:%f y2:%f w:%f h:%f x:%f y:%f\n", x2, y2, w, h, x, y);
    x += (glyph->advance.x >> 6) * sx;
    y += (glyph->advance.y >> 6) * sy;
}
void Font::EndRender() {
	/*glDisableVertexAttribArray(Font::attribute_coord);
	printOpenGLError();
	glBindTexture(GL_TEXTURE_2D, 0);
	printOpenGLError();
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	printOpenGLError();*/
}

bool Font::Exists(std::string name) {
	return (Font::fontCache.find(name) != Font::fontCache.end());
}
Font Font::Get(std::string name) {
	return Font::fontCache[name];
}
void Font::Cache(std::string name, Font font) {
	Font::fontCache[name] = font;
}
	
Font::Font() {
	pointSize = 12;
	//face = {0};
	isValid = LoadFont(TextEngine_NAMESPACE::DefaultFont);
}
Font::Font(const std::string name) {
	pointSize = 12;
	//face = {0};
	isValid = LoadFont(name);
}
Font::~Font() {}

bool Font::LoadFont(const std::string name) {
	if (Font::faceCache.find(name) == Font::faceCache.end()) {
		//printf("Attempting to init new font: %s\n", name.c_str());
		int err = 0;
		if ((err = FT_New_Face(Font::lib, name.c_str(), 0, &face))) {
			fprintf(stderr, "Error 0x%04x: Could not open font! Is FreeType initialized?\n", err);
			return false;
		}
		printf("Initialized new font: %s\n", face->family_name);
		Font::faceCache[name] = face;
	}
	//printf("Found font in faceCache!\n");
	face = Font::faceCache[name];
	fontName = name;
	FT_Set_Pixel_Sizes(face, 0, 48);
	return true;
}

bool Font::IsValid() {
	return isValid;
}

// State Management
bool Font::SetSize(const unsigned int size) {
	if (!IsValid())
		return false;
	pointSize = size;
	if (FT_Set_Pixel_Sizes(face, 0, pointSize)) {
		fprintf(stderr, "Could not resize font\n");
		return false;
	}
	return true;
}
bool Font::LoadCharacter(const UnicodeChar ch) {
	if (!IsValid())
		return false;
	if (FT_Load_Char(face, ch, FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character '%c'\n", ch);
		return false;
	}
	return true;
}

FT_GlyphSlot& Font::GetGlyph() {
	return face->glyph;
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
//FT_Bitmap Font::GetBitmap(std::string fontName, UnicodeChar ch) { return NULL; }