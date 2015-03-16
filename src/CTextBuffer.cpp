#include "../include/CTextBuffer.hpp"

TextBuffer::TextBuffer() {
	rows = 0;
	columns = 0;
}

TextBuffer::TextBuffer(const unsigned int width, const unsigned int height) {
	SetSize(width, height);
}

TextBuffer::TextBuffer(const UnicodeString str, UnicodeString delimiter) {
	InitFromString(str, delimiter);
}
TextBuffer::TextBuffer(const UnicodeString str) {
	InitFromString(str, TextEngine_NAMESPACE::DELIMITER);
}

TextBuffer::~TextBuffer() {
}

void TextBuffer::Put(const unsigned int x0, const unsigned int y0, TextBuffer buf) {
	unsigned int width, height;
	GetSize(width, height);
	if (x0 >= width || y0 >= height)
		return;
	unsigned int putWidth, putHeight;
	buf.GetSize(putWidth, putHeight);
	unsigned int x1 = x0 + putWidth, y1 = y0 + putHeight;
	if (x1 > width)
		x1 = width;
	if (y1 > height)
		y1 = height;
	for (unsigned int y = y0; y < y1; y++) {
		for (unsigned int x = x0; x < x1; x++) {
			Character ch = buf.GetCharacter(x-x0, y-y0);
			char32_t code = ch.GetChar();
			if (code > 0)
				SetCharacter(x, y, ch);
		}
	}
}

void TextBuffer::Clear(const unsigned int x0, const unsigned int y0, unsigned int x1, unsigned int y1) {
	unsigned int width, height;
	GetSize(width, height);
	if (x0 >= width || y0 >= height)
		return;
	if (x1 > width)
		x1 = width;
	if (y1 > height)
		y1 = height;
	for (unsigned int y = y0; y < y1; y++) {
		for (unsigned int x = x0; x < x1; x++) {
			Character ch(0);
			SetCharacter(x, y, ch);
		}
	}
}
void TextBuffer::Clear() {
	unsigned int width, height;
	GetSize(width, height);
	Clear(0, 0, width, height);
}

void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const UnicodeString str, const UnicodeString delimiter) {
	std::vector<UnicodeString> lines;
	unsigned int strWidth, strHeight;
	GetLines(str, delimiter, lines, strWidth, strHeight);
	SetRange(x0, y0, x1, y1, lines);
}

void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const UnicodeString str) {
	SetRange(x0, y0, x1, y1, str, TextEngine_NAMESPACE::DELIMITER);
}

bool TextBuffer::IsValid() {
	unsigned int width, height;
	GetSize(width, height);
	return ((width > 0) && (height > 0));
}

void TextBuffer::SetSize(const unsigned int width, const unsigned int height) {
	rows = height;
	columns = width;
	if (rows == 0 || columns == 0) {
		rows = 0;
		columns = 0;
		return;
	}
	buffer.resize(width);
	for (unsigned int x = 0; x < width; ++x)
		buffer[x].resize(height);
	Clear();
}

void TextBuffer::GetSize(unsigned int& width, unsigned int& height) {
	width = columns;
	height = rows;
}

void TextBuffer::SetCharacter(const unsigned int x, const unsigned int y, Character ch) {
	if (x < 1 || y < 1)
		return;
	buffer[x][y] = ch;
}
Character TextBuffer::GetCharacter(const unsigned int x, const unsigned int y) {
	Character nul (0);
	if (x < 1 || y < 1 || x >= columns || y >= rows)
		return nul;
	return buffer[x][y];
}

// Private methods
void TextBuffer::GetLines(const UnicodeString str, const UnicodeString delimiter, std::vector<UnicodeString>& lines, unsigned int& width, unsigned int& height) {
	unsigned int longestLine = 0;
	size_t position = 0;
	size_t positionPrev = 0;
	while((position = str.find(delimiter, positionPrev)) >= 0) {
		UnicodeString newLine = str.substr(positionPrev, position);
		if (newLine.length() > longestLine)
			longestLine = newLine.length();
		lines.push_back(newLine);
		positionPrev = position;
	}
}
void TextBuffer::InitFromString(const UnicodeString str, const UnicodeString delimiter) {
	std::vector<UnicodeString> lines;
	unsigned int width, height;
	GetLines(str, delimiter, lines, width, height);
	SetSize(width, height);
	SetRange(0, 0, width, height, str, TextEngine_NAMESPACE::DELIMITER);
}
void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, std::vector<UnicodeString> lines) {
	unsigned int width, height;
	GetSize(width, height);
	if (x1 >= width || y1 >= height)
		return;	
	for (unsigned int y = y0; y < y1; y++) {
		for (unsigned int x = x0; x < x1; x++) {
			char32_t value = 0;
			if (x-x0 < lines[y-y0].size())
				value = lines[y-y0][x-x0];
			Character ch(value);
			SetCharacter(x, y, ch);
		}
	}
}
