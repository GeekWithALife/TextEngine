#ifndef INC_TextEngine_Character
#define INC_TextEngine_Character

#include "NEngine.hpp"
#include "CFont.hpp"

class Character {
	public:
		Character();
		Character(UnicodeChar code);
		~Character();
		
		bool IsUnicode();
		
		void SetChar(UnicodeChar code);
		UnicodeChar GetChar();
		
		void SetFont(std::string font);
		Font GetFont();
		
		void SetColor(char r, char g, char b, char a);
		void SetColor(char r, char g, char b);
		void GetColor(float color[4]);
		
		void SetBackgroundColor(char r, char g, char b, char a);
		void SetBackgroundColor(char r, char g, char b);
		
	private:
		UnicodeChar codePoint;
		char fgColor[4];
		char bgColor[4];
		Font fontRef;
};
#endif