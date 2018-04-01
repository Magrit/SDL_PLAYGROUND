#include "stdafx.h"
#include <string>
#include "Window.h"
#include <iostream>
Window::Window(int width, int height)
  : _width(width), _height(height)
{
}


Window::~Window()
{
}

void Window::Quit()
{
  //Destroy window
  SDL_DestroyWindow(window);

  //Quit SDL subsystems
  SDL_Quit();
}

bool Window::Init()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }
  else {
    //Create window
    window = SDL_CreateWindow("SDL Test",
      SDL_WINDOWPOS_UNDEFINED,
      SDL_WINDOWPOS_UNDEFINED,
      _width,
      _height,
      SDL_WINDOW_OPENGL | 
      SDL_WINDOW_ALLOW_HIGHDPI |
      SDL_WINDOW_RESIZABLE);

    /*if (window == nullptr){
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else{
    screenSurface = SDL_GetWindowSurface(window);
    SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xDA, 0xDD, 0xFF));
    }*/
  }
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  mainContext = SDL_GL_CreateContext(window);

  SDL_GL_SetSwapInterval(1);
  return true;
}

bool Window::Frame()
{
  SDL_StartTextInput();
  while (SDL_PollEvent(&e) != 0)
  {
    std::string str;
    switch (e.type) {
    case SDL_WINDOWEVENT:
    {
      switch (e.window.event)
      {
      case SDL_WINDOWEVENT_RESIZED:
      {
        std::cout << "Window resized to: " << e.window.data1 << ":" << e.window.data2 << std::endl;
      }
      default:
        break;
      }
    }
    break;
    case SDL_TEXTINPUT:
    {
      if (_charHandler) {
        _charHandler((void*)&e.text.text[0]);
      }
    }
    break;
    case SDL_KEYDOWN:
      if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE || e.type == SDL_QUIT)
      {
        return false;
      }
      if (e.key.keysym.scancode == SDL_SCANCODE_BACKSPACE) {
        if (_backspaceHandler) {
          _backspaceHandler();
        }
      }
      break;

    case SDL_KEYUP:
      break;

    default:
      break;
    }
  }
  return true;
}

void Window::SwapBuffers()
{
  SDL_GL_SwapWindow(window);
}

void Window::registerCharHandler(void(*f)(void *data))
{
  _charHandler = f;
}

void Window::registerBackspaceHandler(void(*f)())
{
  _backspaceHandler = f;
}
