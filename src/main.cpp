#include "TextEngine.h"
#include "TextObject.h"
#include <iostream> // cout
#include <curses.h>  // ncurses
#include <cstdlib> // atoi
#include <fstream> // ifstream
#include <string> // string
#include <vector> // vector
using namespace std;
/* 
 * Sliders
 * Idea: Build words to make items to solve puzzles (i.e. b-o-m-b or k-e-y).
 */

bool isEmptySpace( char c ) {
  return ( c == ' ' );
}
bool isEndZone( char c ) {
  return ( c == '@' );
}
bool isCharWall( char c ) {
  return ( c >= 'a' ) && ( c <= 'z' );
}
bool isCharSwitch( char c ) {
  return ( c >= 'A' ) && ( c <= 'Z' );
}
bool isInBounds( int x, int y ) {
  return ( x > 0 ) && ( x < 80 ) && ( y > 0 ) && ( y < 24 );
}
bool isPhysicalChar( char c, char guy ) {
  return !isEmptySpace( c ) && !isEndZone( c ) && !isCharSwitch( c ) && ( c != guy );
}

vector<string> readLevelConfig( const char *filename ) {
  vector<string> levels = vector<string>();
  ifstream stream( filename );
  if( !stream.is_open() ) {
    cout << "Failed to read " << filename << "! Terminating." << endl;
    return levels;
  }
  string line = "";
  char c;
  while( ( c = stream.get() ) != EOF ) {
    if( c != '\n' )
      line += c;
    else {
      levels.push_back( line );
      line = "";
    }
  }
  return levels;
}

// Read levels.
bool readLevelFile( const char *filename, TextObject& theLevel ) {
  ifstream stream( filename );
  if( !stream.is_open() ) {
    cout << "Failed to read " << filename << "! Terminating." << endl;
    return false;
  }
  
  string buffer = "";
  char c;
  while( ( c = stream.get() ) != EOF )
    buffer += c;
  theLevel.fill( buffer );
  return theLevel.isValid();
}

bool readAnimFile( const char *filename, TextObject& obj ) {
  ifstream stream( filename );
  if( !stream.is_open() ) {
    cout << "Failed to read " << filename << "! Terminating." << endl;
    return false;
  }
  string buffer = "";
  int h;
  stream >> buffer;
  w = atoi( buffer.c_str() );
  stream >> buffer;
  h = atoi( buffer.c_str() );
  int line = 0;
  char c;
  while( ( c = stream.get() ) != EOF )  {
    if( c == '\n' )
      line++;
    if( line == h ) {
      line = 0;
      obj.addFrame( buffer );
      buffer = "";
    }
    buffer += c;
    
  }
  obj.fill( buffer );
  return obj.isValid();
}

int main( int argc, char **argv ) {
  if( argc < 3 )
    cout << "Not enough args!" << endl;
    
  // Load level list.
  vector<string> levels = readLevelConfig( argv[1] );
  if( levels.empty() ) {
    cout << "No levels!" << endl;
    return 0;
  } else {
    for( unsigned i = 0; i < levels.size(); i++ )
      cout << levels[i] << endl;
  }
  
  // The level.
  TextObject level = TextObject();
  level.setX( 0 );
  level.setY( 0 );
  
  if( !readLevelFile( levels[ atoi( argv[2] ) ].c_str(), level ) ) {
    cout << "Failed to get valid level!" << endl;
    return 0;
  }
  // Your character.
  TextObject guy = TextObject();
  guy.setX( 5 );
  guy.setY( 4 );
  
  string frameDef = ".===.\n|O_O|\n'==='";
  if( !guy.fill( frameDef ) ) {
    cout << "Failed to fill valid player" << endl;
    return 0;
  }
  
  string frame;
  frame = frameDef;
  guy.addFrame( frame );
  guy.saveAnim();
  
  frame = frameDef;
  guy.addFrame( frame );
  frame = ".===.\n|-_-|\n'==='";
  guy.addFrame( frame );
  frame = frameDef;
  guy.addFrame( frame );
  guy.saveAnim();
  
  frame = frameDef;
  guy.addFrame( frame );
  frame = ".===.\n|>_<|\n'==='";
  guy.addFrame( frame );
  frame = frameDef;
  guy.addFrame( frame );
  guy.saveAnim();
  
  guy.setAttrMask( A_BOLD );
  guy.setAnim( 0 );
  
  // Debug messages.
  TextObject debug = TextObject();
  debug.setX( 2 );
  debug.setY( 1 );
  
  // Initialize engine
  TextEngine engine = TextEngine( 80, 24 );
  
  // Add actors.
  guy.setZ( 1 );
  debug.setZ( 2 );
  level.setZ( 3 );
  
  engine.addActor( &guy );
  engine.addActor( &debug );
  engine.addActor( &level );
  
  // Initialize ncurses mode.
  engine.setCurses( true );
  
  // Main game loop.
  char ch = '\0';
  int time = 0;
  while( engine.hasCurses() && ( ch != 'q' ) && ( ch != KEY_EXIT ) ) {
    time++;
    
    char buf[255];
    sprintf(buf,"%d : %d/%d",time, guy.getFrame(), guy.getFrameCount());
    string text = buf;
    debug.fill( text );
    
    // Update guy's anim.
    guy.nextFrame();
    
    // Keypress stuff. This will get replaced later.
    ch = getch();
    if( ch == ERR ) {
      //continue; else {
      if( guy.getAnim() != 0 )
        guy.setAnim( 0 );
    } else {
      // Test misc stuff
      if( ch == '1' ) {
        guy.setDefaultAnim( 0 );
        guy.setAnim( 0 );
        guy.setAnim( 1, 1 );
      } else if( ch == '2' ) {
        guy.setAnim( 0 );
        guy.setAnim( 2, 2 );
      } else if( ch == '3' ) {
        guy.setAnim( 2 );
      }
        
      int prevX = guy.getX(), prevY = guy.getY();
      
      // Character movement.
      int tryX = prevX, tryY = prevY;
      if( ch == KEY_UP - 256 || ch == 'w' )
        tryY = guy.getY() - 1;
      else if( ch == KEY_DOWN - 256 || ch == 's' )
        tryY = guy.getY() + 1;
      else if( ch == KEY_LEFT - 256 || ch == 'a' )
        tryX = guy.getX() - 1;
      else if( ch == KEY_RIGHT - 256 || ch == 'd' )
        tryX = guy.getX() + 1;
        // && isInBounds( tryX + guy.getX(), tryY + guy.getY() )
      if( tryX != prevX ) {
        if( guy.getAnim() != 1 )
          guy.setAnim( 1 );
      } else if( tryY != prevY ) {
        if( guy.getAnim() != 2 )
          guy.setAnim( 2 );
      }
      if( isInBounds( tryX, tryY ) ) {
        char myChar = guy.getCharAt( 0, 0 );
        char inSpot = engine.getScreenChar( tryY, tryX );
        
        //if( !isPhysicalChar( inSpot, myChar ) ) {
          guy.setX( tryX );
          guy.setY( tryY );
        //}
        
        // Update guy's character.
        guy.setAttrMask( A_BOLD );
        if( isCharSwitch( inSpot ) ) {
          guy.setCharAt( 0, 0, inSpot + 32 );
          guy.setAttrMask( A_STANDOUT );
        }
        
        if( isEndZone( inSpot ) ) {
          // end game!
        }
        
      }
    }
    
    // Update the engine
    engine.update();
    
  }
  
  engine.setCurses( false );
  cout << endl << "Goodbye!" << endl;
  
  return 0;
}
/*int test() {
  char test1[12] = "abc\ndef\nghi";
  char test2[12] = "xyx\nyxy\nxyx";
  //char test3[12] = "xyx\nyxy\nxyx";
  TextObject *x = new TextObject();
  TextObject *y = new TextObject();
  TextObject *z = new TextObject();
  //if( !x->fill(7, 4, 'x') )
  //if( !x->fill(10,10) )
  if( !x->fill( test1 ) )
  //if( !x->fill("abc\ndef\nghi") )
  //if( !x->fill("abc\ndef\nghi") )
  //if( !x->fill("abc\ndef\nghi") )
  {
    return 1;
  }
  if( !y->fill( test2 ) )
    return 1;
  if( !z->fill( 3, 3, 'z' ) )
    return 1;
  
  // Fill test
  cout << x->toString().c_str() << endl;
  
  // Basic overlay
  y->setAlpha('y');
  if( !x->overlay( y, 0, 0 ) )
    return 1;
  cout << x->toString().c_str() << endl;
  
  y->setAlpha('x');
  if( !x->overlay( y, 0, 0 ) )
    return 1;
  cout << x->toString().c_str() << endl;
  
  // Overlay with offsets
  for( int i = -1 * x->getW(); i < x->getW() + 1; i++ ) {
    cout << "Overlaying " << i << ',' << i << endl;
    x->fill( test1 );
    if( !x->overlay( z, i, i ) )
      cout << "Overlay did not occur" << endl;
    cout << x->toString().c_str() << endl;
  }

  cout << x->getW() << 'x' << x->getH() << endl;
  delete x;
  delete y;
  delete z;
  return 0;
}*/
