void TextBuffer::TextBuffer() {
	rows = 0;
	columns = 0;
	buffer = NULL;
}

void TextBuffer::TextBuffer(const unsigned int width, const unsigned int height) {
	SetSize(width, height);
}

void TextBuffer::TextBuffer(const std::u32string str, char32_t delimiter) {
	InitFromString(str, delimiter);
}
void TextBuffer::TextBuffer(const std::u32string str) {
	InitFromString(str, TEXTENGINE_DEFAULT_DELIMITER);
}

void TextBuffer::~TextBuffer() {
	delete buffer;
}

void TextBuffer::Put(const unsigned int x0, const unsigned int y0, const TextBuffer buf) {
	if (x0 >= width || y0 >= height)
		return;
	unsigned int width, height;
	GetSize(width, height);
	unsigned int putWidth, putHeight;
	buf->GetSize(putWidth, putHeight);
	unsigned int x1 = x0 + putWidth, y1 = y0 + putHeight;
	if (x1 > width)
		x1 = width;
	if (y1 > height)
		y1 = hieght;
	for (unsigned int y = y0; y < y1; y++) {
		for (unsigned int x = x0; x < x1; x++) {
			Character ch = buf->GetCharacter(x-x0, y-y0);
			char32_t code = 0;
			ch->GetChar(code);
			if (code > 0)
				SetCharacter(x, y, ch);
		}
	}
}

void TextBuffer::Clear(const unsigned int x0, const unsigned int y0, unsigned int x1, unsigned int y1) {
	if (x0 >= width || y0 >= height)
		return;
	unsigned int width, height;
	GetSize(width, height);
	unsigned int putWidth, putHeight;
	buf->GetSize(putWidth, putHeight);
	if (x1 > width)
		x1 = width;
	if (y1 > height)
		y1 = hieght;
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

void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const std::u32string str, const std::u32string delimiter) {
	std::vector<std::u32string> lines;
	unsigned int strWidth, strHeight;
	GetLines(str, delimiter, lines, strWidth, strHeight);
	SetRange(x0, y0, x1, y1, lines);
}

void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const std::u32string str) {
	SetRange(x0, y0, x1, y1, str, TEXTENGINE_DEFAULT_DELIMITER);
}

bool TextBuffer::IsValid() {
	return ((buffer != NULL) && (width > 0) && (height > 0));
}

void TextBuffer::SetSize(const unsigned int width, const unsigned int height) {
	rows = height;
	columns = width;
	delete buffer;
	buffer = NULL;
	if (rows == 0 || columns == 0) {
		rows = 0;
		columns = 0;
		return;
	}
	buffer = new Character[width][height];
	Clear();
}

void TextBuffer::GetSize(int& width, int& height) {
	width = columns;
	height = rows;
}

void TextBuffer::SetCharacter(const unsigned int x, const unsigned int y, Character ch) {
	if (x < 1 || y < 1)
		return;
	buffer[x][y] = ch;
}
void TextBuffer::GetCharacter(const unsigned int x, const unsigned int y, Character& ch) {
	if (x < 1 || y < 1)
		return;
	ch = buffer[x][y];
}

// Private methods
void TextBuffer::GetLines(const std::u32string str, const std::u32string delimiter, std::vector<std::u32string>& lines, unsigned int& width, unsigned int& height) {
	unsigned int longestLine = 0;
	size_t position = 0;
	size_t positionPrev = 0;
	while((position = str.find((char)delimiter, positionPrev)) >= 0) {
		std:u32string newLine = str.substr(positionPrev, position);
		if (newLine.length() > longestLine)
			longestLine = newLine.length();
		lines.push_back(newLine);
		positionPrev = position;
	}
	return lines;
}
void TextBuffer::InitFromString(const std::u32string str, const std::u32string delimiter) {
	std::vector<std::u32string> lines;
	unsigned int width, height;
	GetLines(str, delimiter, lines, width, height);
	SetSize(width, height);
	SetRange(0, 0, width, height, str, TEXTENGINE_DEFAULT_DELIMITER);
}
void TextBuffer::SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, std::vector<std::u32string> lines) {
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
