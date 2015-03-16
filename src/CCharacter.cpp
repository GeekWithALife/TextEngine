#include "../include/CCharacter.hpp"

Character::Character() {
	SetChar(0);
}
Character::Character(char32_t code) {
	SetChar(code);
}

Character::~Character() {
	
}

bool Character::IsUnicode() {
	return codePoint > 127;
}

void Character::SetChar(char32_t code) {
	codePoint = code;
}
char32_t Character::GetChar() {
	return codePoint;
}

void Character::SetFont(std::string name) {
	fontName = name;
}
std::string Character::GetFont() {
	return fontName;
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