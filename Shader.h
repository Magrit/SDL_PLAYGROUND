#pragma once
#include <string>
#include <chrono>
#include "glad\glad.h"

GLenum fileToEnum(std::experimental::filesystem::v1::path path);

struct ShaderFile
{
  std::experimental::filesystem::v1::path _path;
  std::chrono::system_clock::time_point _lastWriteTime;
  GLenum _shaderType;
};

class Shader
{
public:
  Shader(std::vector<std::experimental::filesystem::v1::path> shaderVec);
  ~Shader();

  bool compileShader();
  std::uint32_t reloadShader(ShaderFile &shader);
  void Use();
  void LinkProgram();

  std::vector<std::pair<ShaderFile, std::uint32_t>> _shaders;

  std::uint32_t _programID;
};

