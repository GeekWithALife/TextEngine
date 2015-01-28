#ifndef __TEXTENG_TEXTENG_H
#define __TEXTENG_TEXTENG_H
#include "TextObject.h"
#include <vector>
using std::vector;

class TextEngine {
  public:
    TextEngine( int width, int height );
    ~TextEngine();
    void setCurses( bool on );
    bool hasCurses();
    void update();
    void addActor( TextObject *obj );
    //TextObject *getActor( int i );
    int getScreenW();
    int getScreenH();
    char getScreenChar( int y, int x );
  private:
    bool cursesEnabled;
    TextObject tScreen;
    vector<TextObject*> vActors;
    void drawText( TextObject *obj );
    void drawActors();
    void sortActors();
};
#endif
