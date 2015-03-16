# TextEngine

This is an old personal project that used ncurses to make games with ASCII art. It had a basic game loop with Z-indexing and even animation! Alas, it was poorly written and buggy.

I'm currently rewriting it to use OpenGL and FreeType. It's an attempt to teach myself about basic game engine architecture.

I'd also like to integrate it with Lua.

## Installation

Clone the repo: `git clone https://github.com/GeekWithALife/TextEngine.git`

### Windows

- Install (CMake)[http://www.cmake.org/]
- Install (MinGW)[http://www.mingw.org/]
- Install (FreeType)[http://freetype.org/]
- Install (FreeGLUT)[http://freeglut.sourceforge.net/]
- Install (GLEW)[http://glew.sourceforge.net/]

### Linux

```shell
sudo apt-get install cmake &&
sudo apt-get install libfreetype6 &&
sudo apt-get install libfreetype6-dev &&
sudo apt-get install libxmu-dev &&
sudo apt-get install libxi-dev &&
sudo apt-get install freeglut3-dev &&
sudo apt-get install libglew-dev
```

## Development

### Windows

Open the repository folder in the Windows command prompt by holding Shift and right-clicking in/on the folder and selecting "Open command window here".

First make sure the fully qualified path to your MinGW/bin folder is at the START of your evironment path.

`set path=<YOUR_MINGW_INSTALL>/bin;%path%`

Next we will configure the makefile.

`cmake -G "MinGW Makefiles" .`

If there are no errors, congrats! You installed everything in the right places. Now compile.

`mingw32-make`

And that's that! If for some reason you need to start over, it is highly advisable you run `./cleanup.sh` beforehand.