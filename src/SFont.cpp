#include "../include/SFont.hpp"
#include <iostream>
#include <fstream>

// Error checking
#define printOpenGLError() printOglError(__FILE__, __LINE__)
static int printOglError(char *file, int line)
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
// Quick and dirty read file to string.
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
// Make a shader program.
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

// State
FT_Library Font::lib;
FT_Face Font::curFace;
float Font::curColor[4] = {1, 1, 1, 1};
float Font::curColorBG[4] = {0, 0, 0, 1};
unsigned int Font::curPointSize = 12;

// Cache - fontCache[familyName][styleName] = face;
std::unordered_map<std::string, std::unordered_map<std::string, FT_Face>> Font::fontCache;

// OpenGL
GLuint Font::program = 0;
GLuint Font::tex = 0;
GLuint Font::vbo = 0;
GLint Font::uniform_tex = 0;
GLint Font::attribute_coord = -1;
GLint Font::uniform_color = -1;

// Setup/Teardown
bool Font::Initialize(std::string vertexFile, std::string fragmentFile) {
	// Init FreeType
	if (FT_Init_FreeType(&Font::lib)) {
		fprintf(stderr, "Could not initialize the FreeType library\n");
		return false;
	}
	printf("Loading %s %s\n", vertexFile.c_str(), fragmentFile.c_str());
	// Get Shader source code
	std::string vertexSource;
	std::string fragmentSource;
	ReadShaderFile(vertexFile, vertexSource);
	printf("Loading Vert:\n%s\n", vertexSource.c_str());
	ReadShaderFile(fragmentFile, fragmentSource);
	printf("Loading Frag:\n%s\n", fragmentSource.c_str());
	
	// Compile the shader
	Font::program = CreateShader(vertexSource, fragmentSource);
	if (Font::program == 0) {
		fprintf(stderr, "Could not initialize the shader\n");
		return false;
	}
	
	// Make sure it's got the right variables
	Font::uniform_tex = glGetUniformLocation(Font::program, "tex");
	printOpenGLError();
	if (Font::uniform_tex < 0) {
		fprintf(stderr, "Could not bind texture uniform\n");
		return false;
	}
	Font::uniform_color = glGetUniformLocation(Font::program, "color");
	printOpenGLError();
	if (Font::uniform_color < 0) {
		fprintf(stderr, "Could not bind color uniform\n");
		return false;
	}
	Font::attribute_coord = glGetAttribLocation(Font::program, "coord");
	printOpenGLError();
	if (Font::attribute_coord < 0) {
		fprintf(stderr, "Could not bind coord attribute\n");
		return false;
	}
	
	// Set up the shader program
	glUseProgram(Font::program);
	printOpenGLError();

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &Font::tex);
	glBindTexture(GL_TEXTURE_2D, Font::tex);
	glUniform1i(Font::uniform_tex, 0);
	printOpenGLError();
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenBuffers(1, &Font::vbo);
	glEnableVertexAttribArray(Font::attribute_coord);
	glBindBuffer(GL_ARRAY_BUFFER, Font::vbo);
	glVertexAttribPointer(Font::attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	printOpenGLError();
	
	return true;
}
bool Font::Initialize() {
	std::string defVert (TextEngine_NAMESPACE::DefaultShader);
	defVert.append(".vert");
	std::string defFrag (TextEngine_NAMESPACE::DefaultShader);
	defFrag.append(".frag");
	return Font::Initialize(defVert, defFrag);
}
void Font::Release() {
	glDeleteProgram(Font::program);
	FT_Done_FreeType(Font::lib);
}

// Font Caching
bool Font::LoadFont(std::string folderName) {return false;}
bool Font::LoadFace(std::string fileName, std::string& familyName, std::string& styleName) {
	FT_Face face;
	if(FT_New_Face(Font::lib, "./fonts/cour.ttf", 0, &face)) {
		fprintf(stderr, "Could not open font\n");
		return false;
	}
	FT_Set_Pixel_Sizes(face, 0, 12);
	if(FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
		fprintf(stderr, "Could not load character 'X'\n");
		return false;
	}
	familyName = face->family_name;
	styleName = face->style_name;
	Font::fontCache[familyName][styleName] = face;
	return true;
}
bool Font::SetFace(std::string familyName, std::string styleName, unsigned int pointSize) {
	FT_Face face = Font::fontCache[familyName][styleName];
	if (Font::curPointSize != pointSize) {
		FT_Set_Pixel_Sizes(face, 0, pointSize);
		if(FT_Load_Char(face, 'X', FT_LOAD_RENDER)) {
			fprintf(stderr, "Could not load character 'X'\n");
			return false;
		}
		Font::curPointSize = pointSize;
	}
	Font::curFace = face;
	return true;
}
bool Font::SetColor(float color[4]) {
	for (unsigned int i = 0; i < 4; i++)
		Font::curColor[i] = color[i];
	return true;
}
bool Font::SetColorBG(float color[4]) {
	for (unsigned int i = 0; i < 4; i++)
		Font::curColorBG[i] = color[i];
	return true;
}

// Rendering
void Font::StartRender() {
	glClearColor(Font::curColorBG[0], Font::curColorBG[1], Font::curColorBG[2], Font::curColorBG[3]);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(Font::program);
	printOpenGLError();
	glEnable(GL_BLEND);
	printOpenGLError();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printOpenGLError();
	glUniform4fv(Font::uniform_color, 1, Font::curColor);
}
void Font::Render(const char *text, float x, float y, float sx, float sy) {
	FT_GlyphSlot g = Font::curFace->glyph;
	const char *p;
	for(p = text; *p; p++) {
		if(FT_Load_Char(Font::curFace, *p, FT_LOAD_RENDER))
			continue;
	
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
		printOpenGLError();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		printOpenGLError();
	
		x += (g->advance.x >> 6) * sx;
		y += (g->advance.y >> 6) * sy;
	}
}
void Font::StopRender() {}
