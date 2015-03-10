class Character {
	public:
		Character();
		Character(char32_t code);
		~Character();
		
		bool IsUnicode();
		
		void SetChar(char32_t code);
		void GetChar(char32_t& code);
		
		void SetFont(std::string name);
		void GetFont(std::string& name);
		
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