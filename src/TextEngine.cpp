#include "TextEngine.h"
#include <curses.h>
#include <iostream>
#include <algorithm>
using std::cout;
using std::endl;
using std::vector;

TextEngine::TextEngine( int width, int height ) {
  tScreen = TextObject();
  tScreen.fill( width, height );
  tScreen.setX( 0 );
  tScreen.setY( 0 );
  tScreen.setZ( 0 );
  vActors = vector<TextObject*>();
  cursesEnabled = false;
}

TextEngine::~TextEngine() {
  if( cursesEnabled )
    setCurses( false );
  vActors.clear(); // Clear our actor list.
  tScreen.clear(); // Clear our screen contents.
}

void TextEngine::setCurses( bool on ) {
  // ncurses config
  if( on && !cursesEnabled ) {
    initscr(); // Start curses mode.
    curs_set( 0 ); // Make cursor invisible.
    raw(); // Line buffering disabled.
    keypad( stdscr, TRUE ); // We get F1, F2 etc.
    noecho(); // Don't echo() while we do getch.
    halfdelay(1); // Half-delays allow for updates.
  } else if( cursesEnabled ) {
    clrtoeol(); // Clear to End-of-line.
    nocbreak(); // Go back to normal input mode.
    endwin(); // Exit curses mode.
  }
  cursesEnabled = on;
}

bool TextEngine::hasCurses() {
  return cursesEnabled;
}

// Add an actor.
void TextEngine::addActor( TextObject *obj ) {
  // If obj->getZ() is lower than vActors[0]->getZ(), push front.
  // If obj->getZ() is higher than vActors[n]->getZ(), push back.
  // Otherwise, find acceptable insertion point.
  if( vActors.empty() )
    vActors.push_back( obj );
  vector<TextObject*>::iterator it = vActors.begin();
  while( it != vActors.end() ) {
    if( obj->getZ() > (*it)->getZ() ) {
      vActors.insert( it, obj );
      break;
    }
    it++;
  }
}

int compareZs( const void * a, const void * b ) {
  return ( ((TextObject*)a)->getZ() - ((TextObject*)b)->getZ() );
}
// Sorts actors by Z-value
void TextEngine::sortActors() {
  stable_sort( vActors.begin(), vActors.end(), compareZs );
  stable_sort( vActors.begin(), vActors.end(), compareZs );
}

// Draw an actor to the terminal. This allows us to use attributes.
void TextEngine::drawText( TextObject *obj ) {
  attron( obj->getAttrMask() );
  for( int row = 0; row < obj->getH(); row++ )
    for( int col = 0; col < obj->getW(); col++ )
      if( obj->getCharAt( row, col ) != obj->getAlpha() )
        mvaddch( obj->getY() + row, obj->getX() + col, obj->getCharAt( row, col ) );
  attroff( obj->getAttrMask() );
}

// Draw actors to terminal and virtual screen.
void TextEngine::drawActors() {
  for( unsigned i = 0; i < vActors.size(); i++ ) {
    // Make sure an actor is valid before adding them to the screen.
    if( vActors[i]->isValid() ) {
      // Draw to real screen.
      drawText( vActors[i] );
      // Draw to virtual screen.
      //tScreen.overlay( *vActors[i], vActors[i]->getY(), vActors[i]->getX() );
      //mvprintw( i + 10, 60, "%d - %c", i, getScreenChar( vActors[i]->getY(), vActors[i]->getX() ) );
    }
  }
}

void TextEngine::update() {
  if( !cursesEnabled ) {
    cout << "No curses!" << endl;
    return;
  }
  // If our screen text object is invalid, all is lost...
  if( !tScreen.isValid() ) {
    printw( "Screen text invalid! Bad frame!\n" );
    return;
  }
  if( vActors.empty() ) {
    printw( "No actors! :(\n" );
    return;
  }
  // Clear the screen.
  erase();
  // Revert screen to be just alpha chars.
  tScreen.wipe();
  // Sort our TextObjects by Z-value.
  sortActors();
  // Draw our TextObjects.
  drawActors();
  // Print everything to the screen for reals.
  refresh();
}

int TextEngine::getScreenW() {
  if( !tScreen.isValid() )
    return 0;
  return tScreen.getW();
}

int TextEngine::getScreenH() {
  if( !tScreen.isValid() )
    return 0;
  return tScreen.getH();
}

char TextEngine::getScreenChar( int y, int x ) {
  // Loop through actors (front layer to back layer).
  for( unsigned i = 0; i < vActors.size(); i++ ) {
    if( ( y <= vActors[i]->getY() ) && ( y < vActors[i]->getY() + vActors[i]->getH() ) && 
        ( x <= vActors[i]->getX() ) && ( x < vActors[i]->getX() + vActors[i]->getW() ) ) {
      char c = vActors[i]->getCharAt( y - vActors[i]->getY(), x - vActors[i]->getX() );
      if( c != vActors[i]->getAlpha() )
        return c;
    }
  }
  return ' ';
}

/*for( unsigned i = 0; i < vActors.size(); i++ ) {
        TextObject *obj = vActors[i];
        mvaddch( row, col, getScreenChar( row, col ) );
        // If there is an actor here...
        if( ( col >= obj->getX() ) && 
            ( col <= obj->getX() + obj->getW() ) &&
            ( row >= obj->getY() ) && 
            ( row <= obj->getY() + obj->getH() ) ) {
          attron( obj->getAttrMask() );
          mvprintw( row, col, "%c", getScreenChar( row, col ) );
          attroff( obj->getAttrMask() );
        }
      }*/
