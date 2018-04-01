#pragma once
#include "SDL.h"

class Window
{
public:
  Window(int width, int height);
  ~Window();
  void Quit();
  bool Init();
  bool Frame();
  void SwapBuffers();
  void registerCharHandler(void (*f)(void* data));
  void registerBackspaceHandler(void(*f)());
  int getHeight() {
    return _height;
  }
  int getWidth() {
    return _width;
  }

private:
  SDL_Window* window = nullptr;
  SDL_GLContext mainContext;

  SDL_Surface* screenSurface = nullptr;
  bool quit = false;
  int _width;
  int _height;
  void (*_charHandler)(void*);
  void (*_backspaceHandler)();

  SDL_Event e;
};

