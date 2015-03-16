# TextEngine

This is an old personal project that used ncurses to make games with ASCII art. It had a basic game loop with Z-indexing and even animation! Alas, it was poorly written and buggy.

I'm currently rewriting it to use OpenGL and FreeType. It's an attempt to teach myself about basic game engine architecture.

I'd also like to integrate it with Lua.

## Installation

Clone the repo: `git clone https://github.com/GeekWithALife/TextEngine.git`

### Windows

You will need to install the following to be able to configure/compile this project.

- [CMake](http://www.cmake.org/)
- [MinGW](http://www.mingw.org/)

You will need to install the following libraries as well.

- [FreeType](http://freetype.org/)
- [FreeGLUT](http://freeglut.sourceforge.net/)
- [GLEW](http://glew.sourceforge.net/)

Some of the libraries may need to be compiled.

Once you have the library binaries available, it is best to install them in your MinGW environment. Otherwise, you'll need to edit CMakeLists to tell it where to find them. I did the latter, but if you know enough to do otherwise I would advise it.

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

First make sure the fully qualified path to your MinGW/bin folder is at the START of your evironment path. You can ensure this by running the following.

`set path=[YOUR_MINGW_INSTALL]/bin;%path%`

Next we will configure the makefile.

`cmake -G "MinGW Makefiles" .`

If there are no errors, congrats! You installed everything in the right places. Now compile.

`mingw32-make`

And that's that! If for some reason you need to start over, it is highly advisable you run `./cleanup.sh` beforehand.