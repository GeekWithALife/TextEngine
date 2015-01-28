#ifndef __TEXTENG_TEXTOBJ_H
#define __TEXTENG_TEXTOBJ_H

#include <string>
#include <vector>

using std::string;
using std::vector;
  
// TextObject class definition
class TextObject {
  public:
    // Constructors/Destructors
    TextObject();
    ~TextObject();
    void wipe(); // Replaces all chars with the alpha char.
    void clear();
    bool isValid(); // Validity checker
    string toString(); // Test
    // Initializers
    bool fill( string str );
    bool fill( char *str );
    bool fill( int width, int height ); // Fill to width/height with current alpha char.
    bool fill( int width, int height, char c ); // Fill to width/height with char c.
    // Here's the useful part
    bool overlay( TextObject obj, int row, int col );
    // Setters
    void setCharAt( int row, int col, char c );
    void setAlpha( char alpha );
    void setAttrMask( int mask );
    void setX( int x );
    void setY( int y );
    void setZ( int z );
    // Getters
    char getCharAt( int row, int col );
    char getAlpha();
    int getAttrMask();
    int getX();
    int getY();
    int getZ();
    int getW();
    int getH();
    string getRow( int row );
    // Animation
    void addFrame( string str );
    void saveAnim();
    void setDefaultAnim( int anim );
    void setAnim( int anim );
    void setAnim( int anim, int loop );
    int getAnim();
    void nextFrame();
    int getFrame();
    int getFrameCount();
    int getFrameCount( int anim );
  protected:
    // Container
    vector<string> content;
    // Dimensions
    int w;
    int h;
    // Character to treat as transparent.
    char alpha;
  private:
    // Coordinates
    int x;
    int y;
    int z;
    int attrMask;
    //vector< vector<int> > animStack;
    vector< vector<int> > frameRanges;
    vector<string> frames;
    int lastSavedFrame;
    int defAnim;
    int curAnim;
    int curFrame;
    int maxLoop;
    int curLoop;
};

#endif
