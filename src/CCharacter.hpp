class Character {
	public:
		Character();
		Character(char32_t code);
		~Character();
		
		bool IsUnicode();
		
		void SetChar(char32_t code);
		char32_t GetChar();
		
		void SetFont(std::string name);
		std::string GetFont();
		
		void SetColor(char r, char g, char b, char a);
		void SetColor(char r, char g, char b);
		
		void SetBackgroundColor(char r, char g, char b, char a);
		void SetBackgroundColor(char r, char g, char b);
		
	private:
		char32_t codePoint;
		char fgColor[4];
		char bgColor[4];
		std::string fontName;
};