#include "stdafx.h"
#include "Shader.h"

#include <fstream>
#include <sstream>

Shader::Shader(std::vector<std::experimental::filesystem::v1::path> shaderVec)
{
  for (auto filePair : shaderVec) {
    ShaderFile file;
    file._path = filePair;
    file._lastWriteTime = std::chrono::system_clock::time_point();
    file._shaderType = fileToEnum(file._path);
    _shaders.emplace_back(file, 0);
  }
}

Shader::~Shader()
{
}

bool Shader::compileShader()
{
  return false;
}

std::uint32_t Shader::reloadShader(ShaderFile &shader)
{
  // 1. Retrieve the vertex/fragment source code from filePath
  std::string shaderCodeStr;
  std::ifstream shaderFile;
  // ensures ifstream objects can throw exceptions:
  shaderFile.exceptions(std::ifstream::badbit);
  try
  {
    // Open files
    shaderFile.open(shader._path);
    std::stringstream shaderStream;
    // Read file's buffer contents into streams
    shaderStream << shaderFile.rdbuf();
    // close file handlers
    shaderFile.close();
    // Convert stream into GLchar array
    shaderCodeStr = shaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
  }

  const GLchar *shaderCode = shaderCodeStr.c_str();
  GLuint shaderID, fragment;
  GLint success;
  GLchar infoLog[512];

  shaderID = glCreateShader(shader._shaderType);
  glShaderSource(shaderID, 1, &shaderCode, NULL);
  glCompileShader(shaderID);
  // Print compile errors if any
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::string shaderTypeStr;
    switch (shader._shaderType)
    {
    case GL_VERTEX_SHADER:
      shaderTypeStr = "VERTEX";
      break;
    case GL_FRAGMENT_SHADER:
      shaderTypeStr = "FRAGMENT";
      break;
    case GL_GEOMETRY_SHADER:
      shaderTypeStr = "GEOMETRY";
      break;
    case GL_TESS_EVALUATION_SHADER:
      shaderTypeStr = "TESSELATION_EVALUATION";
      break;
    case GL_TESS_CONTROL_SHADER:
      shaderTypeStr = "TESSELATION_CONTROL";
      break;
    case GL_COMPUTE_SHADER:
      shaderTypeStr = "COMPUTE";
    default:
      shaderTypeStr = "UNKNOWN";
      break;
    }
    std::cout << "ERROR::SHADER::" << shaderTypeStr << "::COMPILATION_FAILED\n" << infoLog << std::endl;
  };
  return shaderID;
}

void Shader::Use()
{
  bool shouldReload = false;
  for (auto &shader : _shaders) {
    auto writeTime = std::experimental::filesystem::v1::last_write_time(shader.first._path);
    if (writeTime > shader.first._lastWriteTime) {
      shouldReload = true;
      shader.first._lastWriteTime = writeTime;
    }
  }
  if (shouldReload) {
    for (auto& shader : _shaders) {
      shader.second = reloadShader(shader.first);
    }

    LinkProgram();
  }
  glUseProgram(_programID);
}

void Shader::LinkProgram()
{
  _programID = glCreateProgram();
  for (auto shader : _shaders) {
    glAttachShader(_programID, shader.second);
  }
  glLinkProgram(_programID);
  // Print linking errors if any
  GLint success;
  GLchar infoLog[512];
  glGetProgramiv(_programID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(_programID, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }
  for (auto shader : _shaders) {
    glDeleteShader(shader.second);
  }
}

GLenum fileToEnum(std::experimental::filesystem::v1::path path) {
  auto extention = path.extension().generic_string();
  if (extention == ".vert") {
    return GL_VERTEX_SHADER;
  }
  else if (extention == ".frag")
  {
    return GL_FRAGMENT_SHADER;
  }
  else if (extention == ".comp")
  {
    return GL_COMPUTE_SHADER;
  }
  return GL_INVALID_ENUM;
}
