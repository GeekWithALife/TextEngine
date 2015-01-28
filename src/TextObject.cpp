/**
 * TODO: Add unicode support!
 * TODO: Change char** to string*
 */
#include "TextObject.h"
#include <ncurses.h>
//#include <stdlib.h>

#include <iostream>
#include <string>
using std::cout;
using std::endl;
using std::string;
//using namespace std;

// Constructor.
TextObject::TextObject() {
  alpha = ' ';
  w = h = x = y = z = 0;
  content = vector<string>();
  frames = vector<string>();
  frameRanges = vector< vector<int> >();
  //animStack = vector< vector<int> >();
  attrMask = A_NORMAL;
  lastSavedFrame = 0;
  defAnim = 0;
  curAnim = 0;
  maxLoop = 0;
  curLoop = 0;
  curFrame = 0;
}

// Destructor
TextObject::~TextObject() {
  this->clear();
}

// Clear the object of non-alpha chars. Does NOT clear animations.
void TextObject::wipe() {
  this->fill( this->w, this->h );
}

// Clears contents and resets width/height. Does NOT reset alpha character.
void TextObject::clear() {
  this->content.clear();
  this->frames.clear();
  this->frameRanges.clear();
  //this->animStack.clear();
  this->w = 0;
  this->h = 0;
}

// Returns object validity.
bool TextObject::isValid() {
  return ( this->w != 0 ) && ( this->h != 0 ) && ( this->content.size() > 0 );
}

// Returns a printable string of the text object
string TextObject::toString() {
  string str = "";
  if( !this->isValid() ) {
    return str;
  }
  for( int row = 0; row < this->h; row++ ) {
   for( int col = 0; col < this->w; col++ )
     str += this->content[row][col];
   str += '\n';
  }
  return str;
}

// Fill using string.
bool TextObject::fill( string str ) {
  int width = 0;
  int height = 0;
  
  // Empty strings are bad, mkay?
  if( str.length() == 0 )
    return false;
  
  // Step 1: Find width and height
  int curWidth = 0;
  int curHeight = 1;
  for( unsigned i = 0; i < str.length(); ++i ) {
    char c = str.at( i ); 
    if( c == '\n' ) {
      if( curWidth > 0 )
        curHeight++;
      curWidth = 0;
    } else {
      curWidth++;
    }
    if( curWidth > width )
      width = curWidth;
  }
  height = curHeight;
  
  // Sanity check.
  if( width < 1 || height < 1 )
    return false;
  
  // Step 2: Fill an empty TextObject
  if( !this->fill( width, height ) )
    return false;
  
  // Step 3: Insert chars from str.
  int row = 0, col = 0;
  for( unsigned i = 0; i < str.length(); ++i ) {
    char c = str.at( i );
    if( c == '\n' ) {
      row++;
      col = 0;
      continue;
    }
    this->content[row][col++] = str[i];
  }
  return true;
}

// Fill using character buffer.
bool TextObject::fill( char *str ) {
  return this->fill( string( str ) );
}

// Fills with current alpha char.
bool TextObject::fill( int width, int height ) {
  return this->fill( width, height, this->alpha );
}

// Fill a text object to the required dimensions with char c.
bool TextObject::fill( int width, int height, char c ) {
  
  // Clear this object if it's different in dimensions.
  if( this->w != width || this->h != height )
    this->clear();
  
  // Add new contents.
  for( int row = 0; row < height; row++ ) {
    this->content.push_back( string( width, c ) );
  }
  
  // Set width and height
  this->w = width;
  this->h = height;
  
  return true;
}

// Overlay a TextObject on this one starting at the specified row/col offset.
bool TextObject::overlay( TextObject obj, int row, int col ) {
  
  // No overlapping with, or on invalid text objects!
  if( !this->isValid() || !obj.isValid() )
    return false;
    
  // Make sure it's even possible for an overlap to occur.
  if( row >= this->h || -row >= obj.h || col >= this->w || -col >= obj.w )
    return false;
  
  // Perform the overlap!
  
  int myRow, myCol, obRow, obCol;
  
  // Don't start iterating on a negative row!
  if( row < 0 )
    myRow = 0;
  else
    myRow = row;
  
  // Loop from row offset to my height
  for( ; myRow < this->h; myRow++ ) { 
  
    // Apply offsets on obj for this row.
    if( row < 0 )
      obRow = -row + myRow;
    else
      obRow = myRow - row;
  
    // Don't write more rows than there are in obj!
    if( obRow >= obj.h )
      break;
  
    // Don't start iterating on a negative column!
    if( col < 0 )
      myCol = 0;
    else
      myCol = col;
  
    // Loop from col offset to my width
    for( ; myCol < this->w; myCol++ ) { 
  
      // Apply offsets on obj for this col.
      if( col < 0 )
        obCol = -col + myCol;
      else
        obCol = myCol - col;
  
      // Don't write more cols per-row than there are in obj!
      if( obCol >= obj.w )
        continue;
  
      // Only overlay non-alpha chars
      if( obj.content[obRow][obCol] != obj.alpha )
        this->content[myRow][myCol] = obj.content[obRow][obCol];
    }
  }
  
  return true;
}

// Getters!

// Return char in specified row/column.
char TextObject::getCharAt( int row, int col ) {
  if( row < 0 || row > this->h || col < 0 || col > this->w )
    return '\0';
  return this->content[row][col];
}

// Return the string representation of the specified row.
string TextObject::getRow( int row ) {
  std::string str = "";
  if( row < 0 || row > this->h )
    return str;
  for( int col = 0; col < this->w; col++ ) {
    str += this->content[row][col];
  }
  return str;
}

// These should speak for themselves.
int TextObject::getX() {
  return this->x;
}
int TextObject::getY() {
  return this->y;
}
int TextObject::getZ() {
  return this->z;
}
int TextObject::getW() {
  return this->w;
}
int TextObject::getH() {
  return this->h;
}
char TextObject::getAlpha() {
  return this->alpha;
}
int TextObject::getAttrMask() {
  return this->attrMask;
}

// Now for the setters!

void TextObject::setCharAt( int row, int col, char c ) {
  if( row < 0 || row > this->h || col < 0 || col > this->w || c > 255 )
    return;
  this->content[row][col] = c;
}
void TextObject::setX( int x ) {
  this->x = x;
}
void TextObject::setY( int y ) {
  this->y = y;
}
void TextObject::setZ( int z ) {
  this->z = z;
}
void TextObject::setAlpha( char alpha ) {
  this->alpha = alpha;
}
void TextObject::setAttrMask( int mask ) {
  this->attrMask = mask;
}

// Animation stuff
void TextObject::addFrame( string str ) {
  frames.push_back( str );
}

void TextObject::saveAnim() {
  vector<int> v = vector<int>();
  v.push_back( this->lastSavedFrame );
  this->lastSavedFrame = frames.size();
  v.push_back( this->lastSavedFrame );
  this->frameRanges.push_back( v );
}

void TextObject::setDefaultAnim( int anim ) {
  if( anim < 0 || anim > (int)this->frameRanges.size() )
    return;
  this->defAnim = anim;
}

void TextObject::setAnim( int anim ) {
  this->setAnim( anim, 0 );
}
int TextObject::getAnim() {
  return this->curAnim;
}

void TextObject::setAnim( int anim, int loop ) {
  if( anim < 0 || anim > (int)this->frameRanges.size() || loop < 0 )
    return;
  this->curLoop = 1;
  this->maxLoop = loop;
  this->curAnim = anim;
  this->curFrame = 0;
}

void TextObject::nextFrame() {
  // If we have reached the end of one animation cycle.
  if( this->curFrame >= this->getFrameCount() ) {
    this->curFrame = 0; // Set current frame to zero.
    if( this->maxLoop > 0 ) {
      // If the current loop count has exceeded the max loop count
      if( this->curLoop >= this->maxLoop )
        this->setAnim( this->defAnim );
      this->curLoop++;
    }
  }
  int frame = this->frameRanges[this->curAnim][0] + this->curFrame;
  this->fill( this->frames[ frame ] );
  this->curFrame++;
}

int TextObject::getFrame() {
  return this->curFrame;
}

int TextObject::getFrameCount() {
  return this->getFrameCount( this->curAnim );
}

int TextObject::getFrameCount( int anim ) {
  if( anim > (int)this->frameRanges.size() )
    return 0;
  return this->frameRanges[anim][1] - this->frameRanges[anim][0];
}
/*
void TextObject::() {
  
}
void TextObject::() {
  
}
void TextObject::() {
  
}
*/
