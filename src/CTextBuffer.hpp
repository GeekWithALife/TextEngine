#define TEXTENGINE_DEFAULT_DELIMITER "\n"
class TextBuffer {
	public:
		// Create an empty buffer of size 0.
		TextBuffer();
		// Create a [width]x[height] buffer.
		TextBuffer(const unsigned int width, const unsigned int height);
		// Create a buffer using a string and delimiter.
		// Width is the length of the longest line, and height is the number of strings split by delimiter.
		TextBuffer(const std::u32string str, const std::u32string delimiter);
		// Create a buffer using a string with the default delimiter (newline)
		TextBuffer(const std::u32string str);
		// Create a buffer using the contents of a file with a specified delimiter.
		TextBuffer(std::string fileName, const std::u32string delimiter);
		// Create a buffer using the contents of the file [fileName] and the default delimiter (newline).
		TextBuffer(std::string fileName);
		~TextBuffer();
		
		void SetSize(const unsigned int width, const unsigned int height);
		void GetSize(unsigned int& width, unsigned int& height);
		bool IsValid();
		
		void SetCharacter(const unsigned int x, const unsigned int y, Character ch);
		void GetCharacter(const unsigned int x, const unsigned int y, Character& ch);
		
		void Clear(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1);
		void Clear();
		
		void Put(const unsigned int x0, const unsigned int y0, const TextBuffer buf);
		
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const std::u32string str, const std::u32string delimiter);
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, const std::u32string str);
		
	private:
		void GetLines(const std::u32string str, const std::u32string delimiter, std::vector<std::u32string>& lines, unsigned int& width, unsigned int& height);
		void SetRange(const unsigned int x0, const unsigned int y0, const unsigned int x1, const unsigned int y1, std::vector<std::u32string> lines);
		void InitFromString(const std::u32string str, const std::u32string delimiter);

		unsigned int rows;
		unsigned int columns;
		Character** buffer;
		
};