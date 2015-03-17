#include "../include/CCharacter.hpp"

Character::Character(char32_t code) {
	SetChar(code);
	SetFont(TextEngine_NAMESPACE::DefaultFont);
}
Character::Character() {
	SetChar(0);
	SetFont(TextEngine_NAMESPACE::DefaultFont);
}

Character::~Character() {}

bool Character::IsUnicode() {
	return codePoint > 127;
}

void Character::SetChar(char32_t code) {
	codePoint = code;
}
char32_t Character::GetChar() {
	return codePoint;
}

void Character::SetFont(std::string font) {
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

void Character::SetBackgroundColor(char r, char g, char b, char a) {
	bgColor[0] = r;
	bgColor[1] = g;
	bgColor[2] = b;
	bgColor[3] = a;
}
void Character::SetBackgroundColor(char r, char g, char b) {
	SetBackgroundColor(r, g, b, 255);
}