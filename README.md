# SDL_SPECIAL_BARNACLE


A non-serious sandbox for some 2D OpenGL SDL implementation.

Trying my best to contain a somewhat ease of read. C++ based and with some finesse like hot-reloading of shaders.

For this project I will probably not dance around with a cross-platform build system, but instead I will try to keep it as is.

Will probably keep this to x64 windows only.


# Dependencies: 
opengl32.lib;SDL2main.lib;SDL2.lib;freetype.lib;

Also in need of SDL2.dll as it links to all kinds of stuff


# TODO:

## General
- Add more maths to Vector2.h
- Add Rectangle or alike to easier setup character quads etc. (based upon Vector2?)
- Add some geometry-representation to easier compose vertex arrays to draw
- Better build configuration
- Add some .ini-file for better path handling?

## Fonts
- Add some CharacterStringDrawer or something that is easier to compose drawcmds of text quads.
- Add linebreaking (multiline)
