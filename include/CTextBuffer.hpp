#ifndef INC_TextEngine_TextBuffer
#define INC_TextEngine_TextBuffer

#include "NEngine.hpp"
#include "CCharacter.hpp"

class TextBuffer {
	public:
		// Create an empty buffer of size 0.
		TextBuffer();
		// Create a [width]x[height] buffer.
		TextBuffer(const unsigned int width, const unsigned int height);
		// Create a buffer using a string and delimiter.
		// Width is the length of the longest line, and height is the number of strings split by delimiter.
		TextBuffer(const UnicodeString str, const UnicodeString delimiter);
		// Create a buffer using a string with the default delimiter (newline)
		TextBuffer(const UnicodeString str);
		~TextBuffer();
		
		void SetSize(const unsigned int width, const unsigned int height);
		void GetSize(unsigned int& width, unsigned int& height);
		bool IsValid();
		
		void SetCharacter(const unsigned int x, const unsigned int y, Character ch);
		Character GetCharacter(const unsigned int x, const unsigned int y);
		std::string GetLine(const unsigned int row);
		
		void Clear(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1);
		void Clear();
		
		void Put(const unsigned int x0, const unsigned int y0, TextBuffer buf);
		
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const UnicodeString str, const UnicodeString delimiter);
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const UnicodeString str);
		
	private:
		void GetLines(const UnicodeString str, const UnicodeString delimiter, std::vector<UnicodeString>& lines, unsigned int& width, unsigned int& height);
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, std::vector<UnicodeString> lines);
		void InitFromString(const UnicodeString str, const UnicodeString delimiter);

		unsigned int rows;
		unsigned int columns;
		std::vector<std::vector<Character>> buffer;
		
};
#endif