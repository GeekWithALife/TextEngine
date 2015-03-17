# TextEngine

This is an old personal project that once used ncurses to make games with ASCII art. It had a basic game loop with Z-indexing and even animation! Alas, it was poorly written and buggy.

I'm currently rewriting it to use OpenGL and FreeType. It's an attempt to teach myself about basic game engine architecture.

At some point in the future I want to re-add ncurses support and make it possible to use that instead of OpenGL. The way I've got it set up it should be easy to do either.

I'd also like to integrate it with Lua.

## Installation

Clone the repo: `git clone https://github.com/GeekWithALife/TextEngine.git`

### Windows

You will need to install the following to be able to configure/compile this project.

- [CMake](http://www.cmake.org/)
- [MinGW](http://www.mingw.org/)

#### Special note about MinGW

You will need to add the MinGW bin folder path to the *start* of your System Environment Path variable before any command-line compiling can take place. [Tutorial Link](http://www.computerhope.com/issues/ch000549.htm)

Your resulting Path variable should look like `<path_to_mingw_install>\bin;<ORIGINAL_PATH_VARIABLES>`

*Note:* You cannot build anything as described in this tutorial from the Git Bash terminal or the MinGW MSYS terminal. It will break. This is because when CMake is told to build using MinGW, it runs an internal script to find the path of the toolchain, which includes a path to sh.exe. Git Bash and MSYS are using their own sh.exe. CMake will recognize this conflict and print an error message.

#### Installing the Libraries

First make a folder somewhere on your system that your compiler will be able to access to store downloaded libraries. On my system, it's `D:/Env/Libraries/`.

Download the following libraries, and extract each of them to the directory you just created. The extracted library folders should contain folders like `src`, `lib`, and maybe a `CMakeLists.txt` file. 

- [FreeGLUT](http://freeglut.sourceforge.net/)
- [FreeType](http://freetype.org/)
- [GLEW](http://glew.sourceforge.net/)

FreeType and GLEW will need to be built using MinGW in order for linking to work properly.

To do this, copy the `build_glew.bat` and `build_freetype.bat` files to their respective installation directories (ex: copy `build_glew.bat` to `<your_library_folder>/glew-<version#>`).

Now run each of those batch files. You can do this by simply double clicking on them.

You will also need to move the relevant binaries to the directory where the final executable ends up. In this case, move them to the folder that this repo was cloned to.

FreeGLUT's binary will be `<freeglut_install_folder>/bin/freeglut.dll`

GLEW's binary will be `<glew_install_folder>/lib/glew32.dll`

FreeType's binary is a static library, and will not need to be copied.

#### Configuring CMake

Finally, you will then need to edit Windows.cmake such that the folder names match the ones on your computer.

Open the file and follow the instructions in the comments.

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

This project comes with a default main.cpp file. You may edit this, or make a new file and add it to the build list in the BuildList.cmake file.

If for some reason you need to start over at any point in the process, it is highly advisable you run `cleanup.sh` beforehand. This file should be runnable on Windows if your `path` environment variable is set correctly.

### Windows

Open the repository folder in the Windows command prompt by holding Shift and right-clicking in/on the folder and selecting "Open command window here".

Run CMake: `cmake -G "MinGW Makefiles" .`

If there are no errors, congrats! You installed everything in the right places.

Make the project: `mingw32-make`

And that's that!

Alternatively, you may simply run the `rebuild.bat` file if you're okay with it rebuilding from the ground up.

### Linux

Open the repository folder in terminal.

Run CMake: `cmake .`

Make the project: `make`

And you're done. Isn't Linux nice?
