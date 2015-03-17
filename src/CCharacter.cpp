#include "../include/CCharacter.hpp"

Character::Character(UnicodeChar code) {
	SetColor(0, 0, 0, 255);
	SetChar(code);
	SetFont(TextEngine_NAMESPACE::DefaultFont);
}
Character::Character() {
	SetColor(0, 0, 0, 255);
	SetChar(0);
	SetFont(TextEngine_NAMESPACE::DefaultFont);
}

Character::~Character() {}

bool Character::IsUnicode() {
	return codePoint > 127;
}

void Character::SetChar(UnicodeChar code) {
	codePoint = code;
}
UnicodeChar Character::GetChar() {
	return codePoint;
}

void Character::SetFont(std::string font) {
	if (Font::Exists(font))
		fontRef = Font::Get(font);
	else
		fontRef.LoadFont(font);
}
Font Character::GetFont() {
	return fontRef;
}

void Character::SetColor(char r, char g, char b, char a) {
	fgColor[0] = r;
	fgColor[1] = g;
	fgColor[2] = b;
	fgColor[3] = a;
}
void Character::SetColor(char r, char g, char b) {
	SetColor(r, g, b, 255);
}
void Character::GetColor(float color[4]) {
	for (unsigned int i = 0; i < 4; i++)
		color[i] = fgColor[i]/255.0;
}

void Character::SetBackgroundColor(char r, char g, char b, char a) {
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
	bgColor[3] = a;
}
void Character::SetBackgroundColor(char r, char g, char b) {
	SetBackgroundColor(r, g, b, 255);
}