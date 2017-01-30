# Popanel

## Project history

This project was started August 2015. 

My goal back then was to learn C++ on a larger scale project than the exercises in the C++ book that I was reading at the time.

I stopped working on it on May 2016, and picked it back up to create the custom keyboard controls around October 2016.

## Personal notes
### Library choice
SDL 1.2 doesn't seem like the best choice for a C++ game, but that choice was made for several reasons :
* I wanted to use a library that I already knew, to make debugging easier
* I wanted the game to be cross-platform (Windows, Linux, Raspberry Pi), that also ruled out the use of Visual Studio
* I wanted to create a wrapper for SDL's graphical and sound functions, to make the drawing easier without manipulating pointers all the time
* Why not SDL 2.0 ? At the time I started the project, the Raspberry Pi's desktop was not hardware accelerated by OpenGL, and SDL2 would offer poor (who said worse ?) performance

### About the coding style
The code written at the beginning of this project is rather different than the code at the end of it.

For a good idea of the differences, check the "GameState" and "KeyBoardConfigState" classes.
The latter uses objects more efficiently, while the former is more written like a standard C program with a lot of game functions inside the GameState class.

There are also classes which I would do differently today, eg. "MenuControl". 
* I would separate the rendering from the class, as that would give more freedom to the way to draw it. (pictures, text, a fancy 3D effect)
* The values containers would probably be their own classes instead of vectors, so the type of an entry wouldn't matter for the code controlling changes.

Also, there are a lost of static const inside namespaces. This convention was chosen over compiler macros to facilitate code writing and reading.

### Bottom line
This is version 1.2 of the game. I am sharing this repository to illustrate the project page on my website. Head out to http://www.armandchristophe.com to see it. 

I learnt A LOT while working on this project. There are things which I would not code the same way today, but I'm definitely happy I was able to make this in a playable state.

I plan to update the game at some point, maybe give it a makeover, maybe switch to SDL2 now that hardware acceleration is available on the Raspberry Pi...

The source code is licensed under GPL3.

## How to use the source code
The code requires Code::Blocks to run.

Then you need to install the SDL 1.2 bundled with that source.
* If you're on Windows, you need to install 7-zip first. Then, go and execute the script in ./_install/getdllfromextlibs-windows.cmd.
* If you're on Linux, go and execute the script in ./_install/sdlinstall-linux.sh from a command line.

Then open the cbp file associated with your OS.
