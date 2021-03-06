#include <stdafx.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>
#include <string>
#include <locale>
#include <codecvt>
#include <vector>
#include <algorithm>
#include <chrono>
#include <map>


#include <Window.h>
#include <Shader.h>
#include <Vector2.h>
#include <Rectangle.h>
#include <Image.h>
#include <glad/glad.h>

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 640;


FT_Library  library;
FT_Face     face;
void _check_gl_error(const char *file, int line) {
  GLenum err(glGetError());

  while (err != GL_NO_ERROR) {
    std::string error;

    switch (err) {
    case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
    case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
    case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
    case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
    }

    std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
    err = glGetError();
  }
}

#define check_gl_error() _check_gl_error(__FILE__,__LINE__)

struct Character {
  std::vector<std::uint8_t> _buffer;
  Vector2u _size;
  Vector2i _bearing;
  std::uint32_t _advance;
};


using namespace std::experimental::filesystem::v1; //This namespace is a monster and I should readjust the code to do something else...

std::uint32_t utf8Char;
std::string narrow;
std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> cvt;
std::vector<Character> _characters;
std::map<std::uint32_t, Rectanglef> _charTexCoordMap;
std::vector<Vector2f> verticesVec;
GLuint VAO;
GLuint VBO;
FT_Bitmap bitmap;
Image characterAtlas(2048, 2048);
float currentOffset = 0.0f;

void characterHandler(void *data) {
  char* end = std::find((char*)data, (char*)data + 31, '\0');
  narrow = std::string((char*)data, end);
  std::wstring_convert<std::codecvt_utf8<uint32_t>, uint32_t> cvt;
  utf8Char = cvt.from_bytes(narrow)[0];
  auto foundCharacter = _charTexCoordMap.find(utf8Char);
  Rectanglef texCoordRect;
  if (!FT_Load_Char(face, utf8Char, FT_LOAD_RENDER))
  {
    bitmap = face->glyph->bitmap;
    Character ch;
    if (bitmap.buffer) {
      ch._buffer.resize(bitmap.rows*bitmap.width);
      ch._buffer.insert(ch._buffer.begin(), &bitmap.buffer[0], &bitmap.buffer[bitmap.rows*bitmap.width - 1]);
    }
    ch._advance = face->glyph->advance.x;
    ch._size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
    ch._bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
    _characters.emplace_back(ch);

    if (foundCharacter == _charTexCoordMap.end()) {
      if (ch._size.x + characterAtlas._posX > characterAtlas._sizeX)
      {
        texCoordRect._min = { 0.0f,
          ((float)(characterAtlas._posY + 256) + (float)ch._size.y) / (float)characterAtlas._sizeY };
      }
      else {
        texCoordRect._min = { (float)characterAtlas._posX / (float)characterAtlas._sizeX, 
                          ((float)characterAtlas._posY + (float)ch._size.y) / (float)characterAtlas._sizeY };
      }

      texCoordRect._max = { 
        texCoordRect._min.x + ((float)ch._size.x / (float)characterAtlas._sizeX), 
        texCoordRect._min.y - ((float)ch._size.y / (float)characterAtlas._sizeY) 
      };

      _charTexCoordMap[utf8Char] = texCoordRect;

      characterAtlas.addSubImage(ch._buffer.data(), ch._size.x, ch._size.y);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, characterAtlas._sizeX, characterAtlas._sizeY, 0, GL_RED, GL_UNSIGNED_BYTE, characterAtlas._buffer.data());
    }
    else {
      texCoordRect = foundCharacter->second;
    }

    Vector2f charSize((float)_characters.back()._size.x / SCREEN_WIDTH, (float)_characters.back()._size.y / SCREEN_HEIGHT);
    Vector2f lowerLeft(-1.0f + currentOffset+static_cast<float>(ch._bearing.x)/SCREEN_WIDTH, -(((int)ch._size.y-ch._bearing.y)/640.0f));
    Rectanglef vertRect(lowerLeft, lowerLeft+charSize);

    verticesVec.emplace_back(vertRect.getLowerLeft());
    verticesVec.emplace_back(texCoordRect.getLowerLeft());

    verticesVec.emplace_back(vertRect.getUpperLeft());
    verticesVec.emplace_back(texCoordRect.getUpperLeft());

    verticesVec.emplace_back(vertRect.getLowerRight());
    verticesVec.emplace_back(texCoordRect.getLowerRight());

    verticesVec.emplace_back(vertRect.getUpperLeft());
    verticesVec.emplace_back(texCoordRect.getUpperLeft());

    verticesVec.emplace_back(vertRect.getUpperRight());
    verticesVec.emplace_back(texCoordRect.getUpperRight());

    verticesVec.emplace_back(vertRect.getLowerRight());
    verticesVec.emplace_back(texCoordRect.getLowerRight());

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verticesVec.size() * sizeof(Vector2f), verticesVec.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    currentOffset += static_cast<float>((ch._advance >> 6))/SCREEN_WIDTH;
  }

}

void backspaceHandler() {
  if (!_characters.empty()) {
    currentOffset -= static_cast<float>((_characters.back()._advance >> 6)) / SCREEN_WIDTH;
    _characters.pop_back();
    verticesVec.erase(verticesVec.end() - 12, verticesVec.end());
  }
}

int main(int argc, char* argv[]) {
  Window SDLWindow(SCREEN_WIDTH, SCREEN_HEIGHT);

  auto error = FT_Init_FreeType(&library);
  if (error)
  {
    std::cerr << "ERROR IN FREETYPE!" << std::endl;
  }
  error = FT_New_Face(library,
    "C:/Windows/Fonts/cour.ttf",
    0,
    &face);

  if (error) {
    std::cerr << "ERROR LOADING FONT! " << std::endl;
  }

  FT_Set_Pixel_Sizes(face, 0, 256);
  //TODO: Make thread which watches this folder?
  path baseFolder = current_path().append("shaders");
  std::vector<path> shaderFileVec;
  shaderFileVec.push_back({ baseFolder.append("textShader.vert") });
  shaderFileVec.push_back({ baseFolder.replace_filename("textShader.frag") });
  Shader _shader(shaderFileVec);
  shaderFileVec.clear();
  shaderFileVec.push_back({ baseFolder.replace_filename("testCompute.comp") });
  Shader _computeShader(shaderFileVec);

  SDLWindow.registerCharHandler(characterHandler);
  SDLWindow.registerBackspaceHandler(backspaceHandler);
  //Initialize SDL with GL context
  if (!SDLWindow.Init()) {
    printf("Error intializing SDLWindow");
    return -1;
  }

  if (!gladLoadGL()) { exit(-1); }
  printf("OpenGL Version %d.%d loaded", GLVersion.major, GLVersion.minor);


  Character dotChar;
  if (!FT_Load_Char(face, '.', FT_LOAD_RENDER))
  {
    bitmap = face->glyph->bitmap;
    dotChar._buffer.resize(bitmap.rows*bitmap.width);
    dotChar._buffer.insert(dotChar._buffer.begin(), &bitmap.buffer[0], &bitmap.buffer[bitmap.rows*bitmap.width - 1]);
    dotChar._advance = face->glyph->advance.x;
    dotChar._size = { face->glyph->bitmap.width, face->glyph->bitmap.rows };
    dotChar._bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
  }

  std::uint32_t texture;
  glGenTextures(1, &texture);

  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, characterAtlas._sizeX, characterAtlas._sizeY, 0, GL_RED, GL_UNSIGNED_BYTE, characterAtlas._buffer.data());

  
  std::uint32_t fboTexture, FBO;
  glGenTextures(1, &fboTexture); 
  glBindTexture(GL_TEXTURE_2D, fboTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glGenFramebuffers(1, &FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, FBO);
  glBindImageTexture(0, fboTexture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA8);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, 0);


  auto timePoint = std::chrono::system_clock::now();


  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, nullptr);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  char test[32];
  test[0] = '|';
  test[1] = '\0';
  
  int frameTic = 0;
  bool dotActive = false;
  glBindTexture(GL_TEXTURE_2D, texture);
  while (SDLWindow.Frame())
  {
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    ++frameTic;
    glClear(GL_COLOR_BUFFER_BIT);
    _shader.Use();
    if (frameTic % 30 == 0) {
      dotActive = !dotActive;
    }
    if (dotActive) {
      characterHandler(test);
    }

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, _characters.size() * 6);

    if (dotActive) {
      backspaceHandler();
    }
    _computeShader.Use();

    glUniform1i(glGetUniformLocation(_computeShader._programID, "tex"), fboTexture);
    glDispatchCompute(SCREEN_WIDTH / 3, SCREEN_HEIGHT/ 3, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glBlitFramebuffer(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    SDLWindow.SwapBuffers();
  }

  return 0;
}