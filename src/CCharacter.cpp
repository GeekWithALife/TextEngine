void Character::Character() {
	SetCharCode(0);
}
void Character::Character(char32_t code) {
	SetCharCode(code);
}

void Character::~Character() {
	
}

bool Character::IsUnicode() {
	return code > 127;
}

void Character::SetChar(char32_t code) {
	codePoint = code;
}
void Character::GetChar(char32_t& code) {
	code = codePoint;
}

void Character::SetFont(std::string name) {
	fontName = name;
}
void Character::GetFont(std::string& name) {
	name = fontName;
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